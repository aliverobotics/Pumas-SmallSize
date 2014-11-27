/*
 *  Learning.h
 *  Jander
 *
 *  Created by Adalberto H. Llarena on 6/7/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

/* This library is intended for implementing learning with several kinds of procedures i.e.
   Neural networks. The Robot can request newtork space in order to learn some specific task.
   At execution time, values learned must be loaded an the index of the net must be remembered
   to call back the trained network
*/

#ifndef _LEARNING_
#define _LEARNING_

class Learning
{
public:

	//node for neural net
	struct Nnode {
	      int		    Num;
		  NNet*			Net;
		  Data*		    Train;
		  Data*			Test;
		  Kind_Prepro   Prep;
		  double		Max;  //Upper limit for normalization
		  double		Min;  //Lower limit for normalization
		  double		Threshold;
		  struct Nnode* next;
	};
	
	//first net trained
	struct Nnode* Head;
	
	//Number of nets
	int NetCount;
	
	Learning()
	{
		this->NetCount =0;
		this->Head = NULL;
	}

	~Learning()
	{
		this->freeList();
	}
	
	//frees memory
    void freeList(void);
	//returns the pointer to a given net number
	struct Nnode* Net_Ptr (int numNet);
	//calls show_all_sensors and adds the observation to the list
	int Add_Net(Kind_Nets Kind, int Number_Of_Layers, 
	                      int Layers[], Kind_Functions Kinds[]);
    //Read weights from several files
    int Read_Weights(Nnode* node, char* WeightsFile);
    //Write weights to several files
    int Write_Weights(Nnode* node, char* WeightsFile);
	//Analizes and Prepare Data
	int Analize(Nnode* node, char* Train_File, char* Test_File);
    //Trains the network
    double Train(Nnode* node, char* Train_File, char* Test_File);
	//Test the network with files
	double Test(Nnode* node, char* Test_File);
	//Feeds the net with data and returns the output
	int Try (Nnode* node, double* Data_PTR_In);

	//it must read a configuration file and load all trained networks from file
	int Init(void)
	{
		return 0;
	}
};

//returns the pointer to a given net number
struct Learning::Nnode* Learning::Net_Ptr (int numNet)
{
	struct Nnode * node;
	node = this->Head;
		
	while (node && node->Num!=numNet)
			node = node->next;
				
	return node;
}

int Learning::Analize(Nnode* node, char* Train_File, char* Test_File)
{
	if (node->Net==NULL)
		return -1; //net not created
		
	if (node->Train)
		free (node->Train);
	if (node->Test)
		free (node->Test);
	
	node->Train = new Data;
	node->Test = new Data;
 
	//we read sets
	if (Train_File)
		node->Train->Read(Train_File);
		
	//node->Train->Print();

	if (Test_File)
		node->Test->Read(Test_File);
		
	//we preprocess info
	switch (node->Prep)
	{
		case normalize : node->Train->Normalize(node->Min, node->Max); //we normalize inputs
						 break;
						  
		case threshold: node->Train->Threshold (node->Threshold); //thresholded inputs
						 break;
						  
		case equalize: node->Train->Equalize (node->Min, node->Max); //equalized inputs
					   break;
		case rawdata:
					break;
	}

	//prints the preprocessing
	node->Train->Print_Prepro();
	
	//takes preproccessing of the training set
	node->Test->Assign_Prepro(node->Train);
			
	if(Test_File)
	{
		//we preprocess info
		switch (node->Prep)
		{
			case threshold: node->Test->Threshold (node->Threshold); //thresholded outputs
							break;
							
			case normalize:
			case equalize: node->Test->Preprocess(); //preprocess data
						   break;
			case rawdata:
						break;
		}
	}
	
	node->Train->Print_Prepro();
	
	return 0;
}

double Learning::Train(Nnode* node, char* Train_File, char* Test_File)
{
	if (node->Net==NULL)
		return -1; //net not created
	
	this->Analize(node,Train_File,Test_File);
	
	//if you want to have the normalized sets
	node->Train->Write ("NormalizedTrain.txt");
	node->Test->Write ("NormalizedTest.txt");
	
	fprintf (stdout, "Training ...\n");
	double error = node->Net->Train (node->Train, node->Test);
	fprintf (stdout, "\n*************************************************\n");

	//we clear input data, but keep preprocessing for inputs
	node->Train->Clear_Memory();
	//we clear output data, but keep preprocessing for outputs
	node->Test->Clear_Memory();
	
	return error;
}

//test the net with a test set
double Learning::Test(Nnode* node, char* Test_File)
{
	if (node->Net==NULL)
		return -1; //net not created
		
	if (node->Train==NULL)
		return -1;  //it has not been trained
 
	//we read set
	node->Test->Read(Test_File);	
	
	//we preprocess info
	switch (node->Prep)
	{
		case threshold: node->Test->Threshold (node->Threshold); //thresholded outputs
						break;
						
		case normalize:
		case equalize: node->Test->Preprocess(); //preprocess data
					   break;
		case rawdata:
					break;
	}
	
	
	fprintf (stdout, "\n*************************************************\n");
	fprintf (stdout, "Testing ...\n");
	
	double error;
	node->Net->Trace_Error = false;
	node->Net->Trace_Output = false;
	node->Net->Trace_Input = false;
	error = node->Net->Test (node->Test);
	printf ("error %4.2f\n",error);
   
	fprintf (stdout, "\n*************************************************\n");

	//we clear output data, but keep preprocessing for outputs
	node->Test->Clear_Memory();
			
	return error;
}
 
//Feeds the net with data and returns the output
int Learning::Try (Nnode* node, double* Data_PTR)
{
	if (node->Net==NULL)
	   return -1; //network not trained
	
	switch (node->Prep)
	{
		case normalize :
		case equalize  : {	node->Train->Preprocess(Data_PTR);
						  } break;
						  
		case threshold : { node->Train->Threshold(Data_PTR, node->Threshold); }
						  break;
		case rawdata:
					break;
	}
	
	//we try the net
	node->Net->Try(Data_PTR);
	
	if (node->Prep!=rawdata)
		node->Train->Postprocess(Data_PTR);

	return 0;
}

int Learning::Write_Weights(Nnode* node, char* WeightsFile)
{
	if (node->Net==NULL)
	   return -1; //network not trained
	   
	fprintf (stdout, "\nWriting training to disk... \n");
	//we write training to disk
	for (int i=1;i<=node->Net->Number_of_Layers;i++)
	{	
		char string[80];
		char buffer[20];
	   
		buffer[0]='0'+i/10;
		buffer[1]='0'+div(i,10).rem;
		buffer[2]='\0';
		   
		strcpy( string, WeightsFile );
		strcat( string, buffer );
		strcat( string, ".txt" );
		
		node->Net->Write_Weights (string, i);
	}
	
	return 0;
}

int Learning::Read_Weights(Nnode* node, char* WeightsFile)
{
	if (node->Net==NULL)
	   return -1;
	   
	fprintf (stdout, "\nReading training from disk... \n");
	//we read training from disk
	for (int i=1;i<=node->Net->Number_of_Layers;i++)
	{	
		char string[80];
		char buffer[20];
	   
		buffer[0]='0'+i/10;
		buffer[1]='0'+div(i,10).rem;
		buffer[2]='\0';
		   
		strcpy( string, WeightsFile );
		strcat( string, buffer );
		strcat( string, ".txt" );
		
		node->Net->Read_Weights (string, i);
	}
	
	return 0;
}

//calls show_all_sensors and adds the observation to the list
int Learning::Add_Net(Kind_Nets Kind, int Number_Of_Layers, int Layers[], Kind_Functions Kinds[])
{
  struct Nnode* node;

  //we get the memory
  node = (Nnode*)malloc(sizeof(*node));

  if (!node) {
	return -1;
  }
  
  //we create the network
  node->Net = new NNet(Kind,Number_Of_Layers,Layers,Kinds);
  
  this->NetCount++;
  
  node->Train = NULL;
  node->Test = NULL;
  node->Prep = normalize;
  node->Max  = 1.0;
  node->Min  = 0.0;
  node->Num  = this->NetCount;
  node->Threshold = 0.0;
	   
  node->next = this->Head;
  this->Head = node;
  
  return NetCount;
}

//frees memory
void Learning::freeList(void)
{
  struct Nnode *last;
  struct Nnode *next;
  static int count = 0;

  last = this->Head;
  if (last)
	next = last->next;
  else
	next = NULL;

  while (last) {
	count++;
	
  free(last);
  last = next;  
  
  if (last)
	next = last->next;
  else
	next = NULL;
  }

  fprintf(stdout,"\nTotal networks deleted =  %3d\n", count); 
  
  this->NetCount = 0;

  return;
}

#endif
