//Neural Network Class
/*
 *  NNets.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NNETS_
#define _NNETS_

#define MAX_LAYERS 30 //Maximun number of layers
#define KOHONENDIST 2

typedef enum {
    ffn =0,
	elman=1,
	jordan=2,
	savage=3,
	hopfield=4,
	vq=5,
	lvq=6,
	rbf=7,
	kohonen=8
	//only a section of the output is feeded back to the inputs, 
	//the number of feededback unit have to be specified as if it 
	//were an extra layer in the list
	//but the number of layers does not have to take it in count
	} Kind_Nets;
	
typedef enum {
    backprop =0,
	widrow =1,
	greville =2,
	error_correction=3,
	hebb=4
	//only a section of the output is feeded back to the inputs, 
	//the number of feededback unit have to be specified as if it 
	//were an extra layer in the list
	//but the number of layers does not have to take it in count
	} Kind_Train;
	
//Neural Network Super-Class
class NNet 
{
public:
	int Number_of_Layers;
	int NUM_INPUTS;
	int NUM_OUTPUTS;
		
	NLayer **Layer;
	double* Internal_PTR;
	double* Internal_State;
	
	//se va
	double* Inputs_PTR;
	double* Outputs_PTR;
	Kind_Nets Kind;
	Kind_Train Train_Method;
	
	double  Learn_Factor;
	double  Momentum;
	double  Min_Error;
	int		Num_Epochs;
	int		Max_Cycles;  //max iterations for hopfield networks
	bool	Batch_Train;
	bool	Trace_Train;
	bool	Trace_Actualize;
	bool	Trace_Error;
	bool	Trace_Output;
	bool	Trace_Input;
	bool	Keep_Order; //keeps the order of the input vectors
	
	int     Separation; //for hopfield and maps
	
	//limit for stopping training if reached
	double  Min_Gradient; 
	
	NNet (Kind_Nets aKind, int init_Num_Layers, int Num_Neuron_Layer[], Kind_Functions Kinds[])
	{
	    int i;
	   
	    this->Number_of_Layers = init_Num_Layers-1;

		if (aKind==elman || aKind==jordan || aKind==savage || aKind==hopfield) 
			//we reserve space for the layers plus an extra
			this->Layer = (NLayer **)malloc(init_Num_Layers*sizeof(NLayer*));
		else
			this->Layer = (NLayer **)malloc(this->Number_of_Layers*sizeof(NLayer*));

		this->NUM_INPUTS = Num_Neuron_Layer[0];
		this->NUM_OUTPUTS = Num_Neuron_Layer[init_Num_Layers-1];
		this->Learn_Factor = 0.5;
		this->Momentum = 0.0;
		this->Min_Error = 0.0005;
		this->Num_Epochs = 500;
		this->Batch_Train = true;
		this->Trace_Train = true;
		this->Trace_Actualize = false;
		this->Trace_Error = false;
		this->Trace_Input = false;
		this->Trace_Output = false;
		this->Kind = aKind;

		if (aKind==vq || aKind==lvq || aKind==rbf || aKind==kohonen)
			this->Min_Gradient = -0.000001;
		else
			//for avoiding premature stop
			this->Min_Gradient = 0.000001;

		this->Keep_Order = false;
		this->Max_Cycles = 10;
		this->Train_Method = backprop;
		this->Separation = 1000;

		int internal=0;
		
		//we count internal data slots
		for(i=0;i<init_Num_Layers;i++)
		{
			internal=internal+Num_Neuron_Layer[i]+1;
			printf ("layer %d size %d\n",i,Num_Neuron_Layer[i]);
		}
			
		//we substract last one from output that has not extended vector
		internal=internal-1; 
		
		//Added FOR SAVAGE
		int extra_units = 0;

//we have to verify this		
/*		if (aKind==elman || aKind==jordan || aKind==savage)
		{
			this->Batch_Train = false; //they need to be trainned with each input vector
			this->Keep_Order = true; //they have to follow the sequence in time
		}
*/		
		switch (aKind) {
		   //if elman we have to add internal state units
		   case elman: internal = internal + Num_Neuron_Layer[1];
		   break;
		   //case jordan we have to add a copy of outputs
		   case jordan: internal = internal + Num_Neuron_Layer[this->Number_of_Layers];
		   break;
		   case savage: 
				{
				  //to avoid the risk of core domping if extra number is not specified
				  //last number contains number of last feedbacked units
				  if(Num_Neuron_Layer[init_Num_Layers]>Num_Neuron_Layer[init_Num_Layers-1] ||
					Num_Neuron_Layer[init_Num_Layers]<0)
					{
						printf ("\nError, recurrent units bad specified or incorrect !\n\n");
						//all units are feeded back
						extra_units = Num_Neuron_Layer[init_Num_Layers-1];
					}
				  else
					    extra_units = Num_Neuron_Layer[init_Num_Layers];
		
				  internal = internal + extra_units;
				}
		   break;
		   case hopfield: internal = internal + Num_Neuron_Layer[0] + 1; //plus bias and feedback all
		   break;
		   case vq:
		   case lvq:
		   case rbf:
		   case kohonen:
		   case ffn: break;
		}
		
		printf ("internal size %d\n",internal);
		    
		this->Internal_PTR=(double*)malloc(internal*sizeof(double));
		
		if (aKind==hopfield)
		{
		    this->Internal_State=(double*)malloc(Num_Neuron_Layer[0]*sizeof(double)); //internal state
			//we set inital state
			for (int i=0; i<Num_Neuron_Layer[0]; i++)
			  *(this->Internal_State+i) = (double)0.0f; //zero for internal at the beginning forcing iteration
		}
		else
		   this->Internal_State=NULL;
		
		//we clear data 
		for (int p=0;p<internal;p++)
		    *(Internal_PTR+p)=(double)0.0f /*p*/;
		
		int numinputs = Num_Neuron_Layer[0];
		switch (aKind)
		{
		    case elman: //we add state units to first layer
					    numinputs = numinputs + Num_Neuron_Layer[1];
			break;
			case jordan: //we add final state to inputs first layer
						numinputs = numinputs + Num_Neuron_Layer[this->Number_of_Layers];
			break;
			case savage: //we add some final state to inputs first layer
						//specified at the end of list
						numinputs = numinputs + extra_units;
			break;
			case vq:
		    case lvq:
		    case rbf:
		    case kohonen:
			case ffn: 
			case hopfield: break;
		}
		
		double* inputs = this->Internal_PTR;
		double* outputs = inputs + numinputs + 1; //for extra bias
		   
		//the first layer only indicates the number of inputs
		//we asign the pointers to layers
		for(i=0;i<this->Number_of_Layers;i++)
		{
			    int *Connections_Layer;

				//we get memory
				Connections_Layer = (int *) malloc (Num_Neuron_Layer[i]
				                                   *Num_Neuron_Layer[i+1]*sizeof(int));

				for (int j=0;j<Num_Neuron_Layer[i]*Num_Neuron_Layer[i+1];j++)
					 Connections_Layer[j] = 1; //we are connecting all inputs
				    
				this->Layer[i] = new
				NLayer 
				    (i+1 //Layer Number i
		           ,Num_Neuron_Layer[i+1] //Contains n Neurons
				   ,Kinds[i] //Kind
				   ,numinputs //num. Inputs + bias (internally asigned)
				   ,inputs //start of inputs
				   ,Connections_Layer
				    //Matrix of conectivity, all conected
				   ,outputs //Outputs for layer one
				   ,1 //random weights
				   ,false //trace
				   );
				
				if (aKind!=hopfield)
					//asign value for bias input only once from here
					*(inputs+numinputs) = (double)(1.0);
				else
				{
				   *(inputs+numinputs) = (double)(0.0); //zeroed bias
				   //we fix all weigths of layer to 1.0f an bias 0
				   this->Layer[0]->Set_Weights ((double)0.0,(double)0.0);
				}
				   
				//inputs next layer
				numinputs = Num_Neuron_Layer[i+1];  
					
				//comprobation
				//printf ("ya imprimi\n");
				//this->Layer[i]->Print();
				   
				//the final output is the same of the last layer
				if (i<this->Number_of_Layers-1)
				{
					inputs = outputs; //the inputs of next layer are the outputs of this layer
					outputs = inputs + Num_Neuron_Layer[i+1] + 1;
				}

				//we free memory
				free (Connections_Layer);

		}//for layers
		
		this->Inputs_PTR=Internal_PTR;
		this->Outputs_PTR=outputs;
		
		//we add extra layer for elman
		//hopfield is a special case of jordan with all with feedback
		if (aKind==elman || aKind==jordan || aKind==savage || aKind==hopfield) {
		
			int *Connections_FeedBack;

			//we get memory
			Connections_FeedBack = (int *) malloc (numinputs*numinputs*sizeof(int));

			double *input_addr = NULL;
			double *output_addr = NULL;
			
			switch (aKind)
			{
				case elman: {
							//we have same units than first layer
							numinputs = Num_Neuron_Layer[1];
							//the starting inputs are outputs from 1st layer
							input_addr = this->Inputs_PTR+Num_Neuron_Layer[0]+1
										+Num_Neuron_Layer[1];  //outputs 1st layer
							}
				break;
				case jordan: {
							//we have same units than last layer
							numinputs = Num_Neuron_Layer[this->Number_of_Layers];
							//the starting inputs are outputs from 1st layer
							input_addr = this->Outputs_PTR; //outputs Last layer
							}
				break;
				case savage: {
							//we have certain units of last layer
							numinputs = extra_units;
							//the starting inputs are outputs from 1st layer
							input_addr = this->Outputs_PTR+
							             Num_Neuron_Layer[this->Number_of_Layers]
										 -extra_units; //are the n last
							; //outputs Last layer
							}
				break;
				case hopfield: {
							//we have same units than last layer
							numinputs = Num_Neuron_Layer[this->Number_of_Layers];
							//the starting inputs are outputs from 1st layer
							input_addr = this->Outputs_PTR; //outputs Last layer
							}
				break;
				case vq:
				case lvq:
				case rbf:
				case kohonen:
				case ffn: break;
			}
			
			output_addr = this->Inputs_PTR;
			
			if (aKind!=hopfield)
			   //the start of inputs is next inputs of networks
			   output_addr = output_addr+Num_Neuron_Layer[0];  
			
			//next first n inputs from exterior to 1st layer
			
			for (int i=0;i<numinputs;i++) //we have to connect feedback layer outputs
			  for (int j=0;j<numinputs;j++) //to state layer
				 if (i==j)
					 Connections_FeedBack [i*numinputs+j] = 1; //we connect only same to same neuron
				 else
					 Connections_FeedBack [i*numinputs+j] = 0; //we disconnect inputs from other hidden units
					 
			this->Layer[this->Number_of_Layers] = new
			NLayer 	   (this->Number_of_Layers+1 //Last Layer 
					   ,numinputs //Contains same neurons than feedbacked layer
					   ,purelin //Kind
					   ,numinputs // same number of inputs due to feedback
					   ,input_addr //start of inputs = outputs 1st layer
					   ,Connections_FeedBack //Matrix of conectivity
					   ,output_addr
					   ,0 //random weights
					   ,false //trace
					   );
					   
			//we fix all weigths of layer to 1.0f an bias 0
			this->Layer[this->Number_of_Layers]->Set_Weights ((double)1.0,(double)0.0);

			//we free memory
			free (Connections_FeedBack);
						   				
		} //if elman of jordan
	}

	~NNet()
	{
	 if (this->Internal_PTR)
		free(this->Internal_PTR);
	 if (this->Internal_State)
		free (this->Internal_State);
	 if (this->Layer)
		free (this->Layer);
	}
	
	//initialization previous to activation and training
	void Init(void)
	{
		int internal = this->NUM_INPUTS; //inputs from outside world
		
		switch (this->Kind) {
		   //if elman we have to add internal state units
		   case elman: internal = internal + this->Layer[0]->Number_Neurons;
		   break;
		   //case jordan we have to add a copy of outputs
		   case jordan: internal = internal + 
					  this->Layer[this->Number_of_Layers-1]->Number_Neurons;
		   case hopfield: internal = internal + 
					  this->Layer[this->Number_of_Layers-1]->Number_Neurons;
		   break;
		   case savage: internal = internal + 
					  this->Layer[this->Number_of_Layers]->Number_Neurons;
		   break;
		   case vq:
		   case lvq:
		   case rbf:
		   case kohonen:
		   case ffn: break;
		}
		
		//we clear the inputs
		for (int p=0;p<internal;p++)
		    *(Internal_PTR+p)=(double)0.0f;
			
		//printf ("initializing ******************* %d inputs \n", internal);
	}
	
	void Apply_Deltas(void)
	{
		//we actualize all layers
		for(int i=0;i<this->Number_of_Layers;i++)
			this->Layer[i]->Apply_Deltas();
	}
	
	void Actualize ()
	{
		if (this->Kind!=vq && this->Kind!=lvq && this->Kind!=rbf && this->Kind!=kohonen)
			//we actualize all layers
			for(int i=0;i<this->Number_of_Layers;i++)
				this->Layer[i]->Actualize(this->Trace_Actualize);
		else
		{
				this->Layer[0]->Clear_Outputs(); //we clear the layer outputs
				//we find the winner and actualize its output
				*this->Layer[0]->Tissue[this->Layer[0]->Find_Winner()]->Output=1.0;
				
				//if rbf missing
		}
			
		//extra layer for elman or jordan
		if (this->Kind==elman || this->Kind==jordan || this->Kind ==savage || this->Kind==hopfield)
		   this->Layer[this->Number_of_Layers]->Actualize(this->Trace_Actualize);
		   
		//comprobation
		 //  this->Layer[0]->Print_Inputs();
	}

	void Print (void)
	{
		//we print all layers
		for(int i=0;i<this->Number_of_Layers;i++)
		{
			this->Layer[i]->Print();
		}
	}
	
	void Print_Inputs (void)
	{
		int j;
		
		//we print input data
		printf ("[ ");
		for (j=0;j<this->NUM_INPUTS-1;j++)
			printf ("%3.2f ,", *(this->Inputs_PTR+j) );
		printf ("%3.2f ]", *(this->Inputs_PTR+this->NUM_INPUTS-1) );
	}
	
	void Print_Outputs (void)
	{
		int j;
		
		//we print output data
		printf (": [ ");
		for (j=0;j<this->NUM_OUTPUTS-1;j++)
			printf ("%3.2f ,", *(this->Outputs_PTR+j) );
		printf ("%3.2f ]", *(this->Outputs_PTR+this->NUM_OUTPUTS-1) );
	}
	
	//prints the data
	void Print_Hop_Outputs(void)
	{
		//we print output data
		for (int j=0;j<this->NUM_OUTPUTS;j++)
		{
			if (*(this->Outputs_PTR+j)>0)
			printf ("*");
			else
			printf ("0");
			
			if ((j+1)%this->Separation ==0)
			   printf ("\n");
		}
	}
	
	void Print_Hop_Inputs(void)
	{
		//we print output data
		for (int j=0;j<this->NUM_INPUTS;j++)
		{
			if (*(this->Inputs_PTR+j)>0)
			printf ("*");
			else
			printf ("0");
			
			if ((j+1)%this->Separation ==0)
			   printf ("\n");
		}
	}

	void Reset_Weights (bool random)
	{
		//we actualize all layers
		for(int i=0;i<this->Number_of_Layers;i++)
		{
			this->Layer[i]->Reset_Weights(random);
		}
	}

	//fills the net inputs with data
	void Feed (double* Data_PTR)
	{
		int j;
					   
		//We fill the inputs with data
		for (j=0;j<this->NUM_INPUTS;j++)
			*(this->Inputs_PTR+j) = *(Data_PTR + j);
		
		if (this->Trace_Input==true)
		//we print input data
			this->Print_Inputs();
	}
	
	bool Stable_State(void)
	{
		bool Stable=true;
		//we copy internal state
		for (int i=0;i<this->NUM_INPUTS;i++)
		   if (*(this->Internal_State+i) != *(this->Outputs_PTR+i))
			  Stable = false;
		
		return Stable;
	}
	
	void Xfer_State(void)
	{
		//we copy internal state
		for (int i=0;i<this->NUM_INPUTS;i++)
		 *(this->Internal_State+i) = *(this->Outputs_PTR+i);
	}
	
	//returns the result of the net
	void Response(double* Data_PTR)
	{
		int j;
			
		//We fill the outputs with data
		for (j=0;j<this->NUM_OUTPUTS;j++)
			 *(Data_PTR + j) = *(this->Outputs_PTR+j);
			
		//we print output data
		if (this->Trace_Output==true)
			this->Print_Outputs();
	}
	
	//Feeds the net with data and returns the output
	void Try (double* Data_PTR)
	{
		this->Feed(Data_PTR);
		this->Actualize();
   
		if (this->Kind==hopfield) //we have to iterate until a stable state
		{
			int cycles = 0;
			
			printf ("\nInitial Result :\n");
			this->Print_Hop_Outputs ();
			printf ("\nEnergy %4.2f :\n", this->Energy());
			
			//we llop until a stable state or eigth cicles
			do 
			{
				Xfer_State();
				this->Actualize();
				printf ("\nResult  no %d:\n",cycles+1);
				this->Print_Hop_Outputs ();
				printf ("\nEnergy %4.2f :\n", this->Energy());
				cycles++;
				
			} while (this->Stable_State()==false && cycles < this->Max_Cycles);
		}

		this->Response(Data_PTR+this->NUM_INPUTS);
	}
		
	//Calculates error between network output & data for a given row
	double Get_Error (double* Inputs_PTR)
	{
		double error = 0.0;
		int j;
		
		//we feed the data
		this->Feed (Inputs_PTR);							 
		
		//We activate the network
		this->Actualize();
		
		if (this->Trace_Output==true) 
			fprintf (stdout, " : [ ");
				 
		for (j=0;j<this->NUM_OUTPUTS;j++)
		{
		    //we take the result of the net
			double Output= *(this->Outputs_PTR+j);
			double Desired= *(Inputs_PTR + this->NUM_INPUTS + j);
			
		if (this->Trace_Output==true) 
			     fprintf (stdout, "(%2.1f->:%2.1f)", Output, Desired);
				 
			if (j<(this->NUM_OUTPUTS-1) && this->Trace_Output==true) 
			     fprintf (stdout, ",");
				 
			//here we have to insert different metrics
		    error = error + (Output-Desired)*(Output-Desired);
		}
		
		if (this->Trace_Output==true) 
			fprintf (stdout, "] ");
		
		if (this->Trace_Error==true) 
			fprintf (stdout, "Error : %4.2f \n", error);
									
		return error;
	}
	
	double Test (Data *Input_Data)
	{
	    double tot_error = 0.0;
		int i;
				
		//if they have different sizes
		if (Input_Data->NUM_ROWS == 0)
			return -1; //different sizes
		//Columns do not match number of data columns
		if (Input_Data->NUM_COLUMNS != (this->NUM_INPUTS + this->NUM_OUTPUTS))
		    return -2; //inputs do not match
				
		//we initialize the net
		this->Init();
		
		for (i=0;i<Input_Data->NUM_ROWS;i++)
		{
			if (this->Trace_Error==true) 
				fprintf (stdout, "[%4d] ", i);
			
			//we add the vector's error
			tot_error = tot_error +  
						this->Get_Error(Input_Data->Data_PTR + i*Input_Data->NUM_COLUMNS)
										/float(Input_Data->NUM_ROWS);
		}
					
		if (this->Trace_Error==true)
			fprintf (stdout, "\nMedia Error: %4.2f\n", tot_error);
			
		return tot_error;
	}
		
	//replace activation for non derivables
	void Replace_Non_Derivables (void)
	{
		//we verify derivability fot all layers
		for(int i=0;i<this->Number_of_Layers;i++)
		{
			switch (this->Layer[i]->Kind)
			{
				case hardlim : {		
								if (this->Trace_Train==true)
								   printf ("Kind hardlim changed to logsig for layer %d\n",i);
								   
								this->Layer[i]->Assign_Function(logsig);
								}
				break;
				case satlin  : {
							    if (this->Trace_Train==true)
								   printf ("Kind satlin changed to logsig for layer %d\n",i);
								   
								this->Layer[i]->Assign_Function(logsig);
								}
				break;
				case ising  : {
							    if (this->Trace_Train==true)
								   printf ("Kind ising changed to tansig for layer %d\n",i);
								   
								this->Layer[i]->Assign_Function(tansig);
								}
				break;
				
				case logsig: break;
				case purelin: break;
				case tansig: break;
				case radbas: break;
			}
		}
	}

	//we restore the original kind
	void Restore_Non_Derivables (void)
	{
		//we verify derivability fot all layers
		for(int i=0;i<this->Number_of_Layers;i++)
		{
			switch (this->Layer[i]->Kind)
			{
				case hardlim : {		
								   if (this->Trace_Train==true)
								      printf ("Kind hardlim restored for layer %d\n",i);
								   
								   this->Layer[i]->Assign_Function(hardlim);
							    }
				break;
				case satlin  : {		
								   if (this->Trace_Train==true)
								      printf ("Kind satlin restored for layer %d\n",i);
								   
								   this->Layer[i]->Assign_Function(satlin);
								}
				break;
				case ising  : {
							    if (this->Trace_Train==true)
								   printf ("Kind ising restored for layer %d\n",i);
								   
								this->Layer[i]->Assign_Function(ising);
								}
				break;
				
				case logsig: break;
				case purelin: break;
				case tansig: break;
				case radbas: break;
			}
		}
	}
	
	//Performs Weigth Actualization
	void Greville_Adjust_Weights (double* Results)
	{		
		for (int j=0;j<this->Layer[this->Number_of_Layers-1]->Number_Neurons;j++)
		{
			//we get the neuron
			Neuron* aNeuron_ptr = this->Layer[this->Number_of_Layers-1]->Tissue[j];
										
			//we calculate its delta & correct weights
			aNeuron_ptr->Greville_Adjust_Weights 
				(*(Results+j), //desired output
								//remember that the desired output must be -1 or 1
				 this->Learn_Factor, //learning factor
				 this->Momentum,    //acceleration factor
				 !this->Batch_Train); //If applies changes
		}
	}
	
	//Performs Weigth Actualization
	void Widrow_Adjust_Weights (double* Results)
	{
		//we adjust only the last layer
		for (int j=0;j<this->Layer[this->Number_of_Layers-1]->Number_Neurons;j++)
		{
			//we get the neuron
			Neuron* aNeuron_ptr = this->Layer[this->Number_of_Layers-1]->Tissue[j];

			//we calculate its delta & correct weights
			aNeuron_ptr->Widrow_Adjust_Weights 
				(*(Results+j), //desired output
								//remember that the desired output must be -1 or 1
				 this->Learn_Factor, //learning factor
				 this->Momentum,    //acceleration factor
				 !this->Batch_Train); //If applies changes
		}
	}

	//Performs Weigth Actualization
	void Error_Correction_Adjust_Weights (double* Results)
	{
		//we adjust only the last layer
		for (int j=0;j<this->Layer[this->Number_of_Layers-1]->Number_Neurons;j++)
		{
			//we get the neuron
			Neuron* aNeuron_ptr = this->Layer[this->Number_of_Layers-1]->Tissue[j];

			//we calculate its delta & correct weights
			aNeuron_ptr->Error_Correction_Adjust_Weights 
				(*(Results+j), //desired output
								//remember that the desired output must be -1 or 1
				 this->Learn_Factor, //learning factor
				 this->Momentum,    //acceleration factor
				 !this->Batch_Train); //If applies changes
		}
	}
		
	//Gets the energy function for a hoppfiend network
	double Energy (void)
	{
		double suma = 0.0;
		
		for (int i=0;i<this->NUM_INPUTS;i++)
			for (int j=0;j<this->NUM_INPUTS;j++)
			{
				//we get the neuron
				Neuron* aNeuron_ptr = this->Layer[0]->Tissue[i];

				//we calculate its delta & correct weights
				suma = suma + aNeuron_ptr->Weights[j] * 
				       *(this->Outputs_PTR+j) * *(this->Outputs_PTR+i);
			}
			
		return -0.5 * suma;
	}

	//Puts an Identity Mattrix to the weights
	//only for hoppfield networs => square mattrix
	int Use_Identity (void)
	{	
		for (int i=0;i<this->NUM_INPUTS;i++)
			for (int j=0;j<this->NUM_INPUTS;j++)
			{
				//we get the neuron
				Neuron* aNeuron_ptr = this->Layer[0]->Tissue[i];

				//we set the weight
				if (i==j) aNeuron_ptr->Weights[j] = 1.0;
				else aNeuron_ptr->Weights[j] = 0.0;
			}
			
		return 0;
	}
	
	//Trains the net with Hebb for hopfield networks
	int Hebb_Train (Data *Input_Data)
	{
		//if they have different sizes
		if (Input_Data->NUM_ROWS == 0)
			return -1; //different sizes
			
		//Columns do not match number of inputs
		if (Input_Data->NUM_COLUMNS != this->NUM_INPUTS)
		    return -2; //inputs do not match	
	
		for (int i=0;i<this->NUM_INPUTS;i++)
			for (int j=0;j<this->NUM_INPUTS;j++)
			{
				double suma = 0.0;
				
				for (int m=0;m<Input_Data->NUM_ROWS;m++)
					suma = suma + Input_Data->Give_Data(m,i)*
								  Input_Data->Give_Data(m,j);
								  
				//we get the neuron
				Neuron* aNeuron_ptr = this->Layer[0]->Tissue[i];

				//we calculate its delta & correct weights
				aNeuron_ptr->Weights[j] = suma /* / (double)this->NUM_INPUTS */;
			}
			
		return 0;
	}

	double Train (Data *Train_Data, Data *Test_Data)
	{
	    double tot_error = 0.0;
		double last_error = 0.0;
		
		int epoch = 0;
		double tmp_momentum = this->Momentum;
				
		if (this->Kind!=lvq && this->Kind!= vq && this->Kind!=rbf && this->Kind!=kohonen)
		{
			//there are no data
			if (Train_Data->NUM_ROWS == 0)
				return -1; //different sizes

			//Columns do not match number of outputs
			if (Train_Data->NUM_COLUMNS != (this->NUM_INPUTS+this->NUM_OUTPUTS))
				return -2; //size do not match	
		}
			
		//we replace non derivable functions
		if (this->Train_Method == backprop)
			this->Replace_Non_Derivables();
		
		if (this->Train_Method == hebb)
		    return this->Hebb_Train (Train_Data);
		else
		{
			do
			{
				last_error = tot_error;
				tot_error = 0;
				int i, index;
				
				if (this->Batch_Train==false && this->Keep_Order==false)
					//we generate the order of reading randomly
					Generate_Order(Train_Data->NUM_ROWS);

				for (i=0;i<Train_Data->NUM_ROWS;i++)
				{
					if (this->Batch_Train==false && this->Keep_Order==false) 
						 index = Give_Order(i); //we take rows randomly
					else index = i; //it does not matter the order or its important to conserve it
					
					if (this->Kind!=lvq && this->Kind!= vq && this->Kind!=rbf && this->Kind!=kohonen)
					//we add the vector's error
					tot_error = tot_error +  
								this->Get_Error(Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS)
											   /float(Train_Data->NUM_ROWS);	
											 
					//if the net is a hopfield network we must load inputs again
					//because outputs overwite them, or if is lvq or vq we must feed for 1st time
					if (this->Kind==hopfield || this->Kind==vq  || this->Kind==lvq 
						                     || this->Kind==rbf || this->Kind==kohonen)
						//we feed the data again
						this->Feed (Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS);	

					if (this->Kind==vq)
						if (epoch>=this->Layer[0]->Number_Neurons)
							tot_error = tot_error +  this->Layer[0]->Vector_Quantization(
							this->Learn_Factor*(this->Num_Epochs-epoch)/this->Num_Epochs)
							/float(Train_Data->NUM_ROWS);
						else
							//we set the first n weights to input vectors
							for (int p=0;p<this->Layer[0]->Tissue[epoch]->Number_Inputs;p++)
								this->Layer[0]->Tissue[epoch]->Weights[p]=
								*this->Layer[0]->Tissue[epoch]->Inputs[p];

					else if (this->Kind==lvq)
						if (epoch>=this->Layer[0]->Number_Neurons)
							tot_error = tot_error + this->LVQ  (//desired output address
						    Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS + this->NUM_INPUTS)
							/float(Train_Data->NUM_ROWS);
						else
							//we set the first n weights to input vectors
							for (int p=0;p<this->Layer[0]->Tissue[epoch]->Number_Inputs;p++)
								this->Layer[0]->Tissue[epoch]->Weights[p]=
								*this->Layer[0]->Tissue[epoch]->Inputs[p];

					else if (this->Kind==kohonen)
						tot_error = tot_error + this->Kohonen(
						this->Learn_Factor*(this->Num_Epochs-epoch)/this->Num_Epochs)
						/float(Train_Data->NUM_ROWS);

					else if (this->Train_Method==backprop)
						this->Back_Propagation (//desired output address
						Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS + this->NUM_INPUTS); 

					else if (this->Train_Method==widrow)
						this->Widrow_Adjust_Weights (//desired output address
						Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS + this->NUM_INPUTS); 

					else if (this->Train_Method==greville)
						this->Greville_Adjust_Weights (//desired output address
						Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS + this->NUM_INPUTS); 

					else if (this->Train_Method==error_correction)
						this->Error_Correction_Adjust_Weights (//desired output address
						Train_Data->Data_PTR + index*Train_Data->NUM_COLUMNS + this->NUM_INPUTS); 

				}//for numrows
				
				if (this->Batch_Train==true)
					this->Apply_Deltas(); //at the end we recalculate weights
						
				if (this->Trace_Train==true && epoch%10==0)
					fprintf (stdout, "Epoch %5d -> Media Error: %5.4f Gradient = %f ", 
									  epoch, tot_error, fabs(tot_error-last_error));
				epoch++;
				
				//we modfy momentum until final epoch = 0
				this->Momentum*= 0.7;
				
				if (epoch%10==0)
				if (Test_Data)
					printf (" Test Error: %f\n",this->Test (Test_Data));
				else
					printf ("\n");
			
			} while (tot_error>this->Min_Error && epoch<this->Num_Epochs && 
					 (fabs(tot_error-last_error)>this->Min_Gradient)
					 );

			this->Momentum = tmp_momentum; 
			
			if (this->Trace_Train==true)
			{
			   if (fabs(tot_error-last_error)<=this->Min_Gradient)
				  printf ("Minimun Gradient Reached = %5.4f \n", this->Min_Gradient);
			   if (epoch=this->Num_Epochs)
				  printf ("Maximun Epochs Reached = %4d \n", this->Num_Epochs);
			   if (tot_error<=this->Min_Error)
				  printf ("Minimun Error Reached = %5.4f \n", this->Min_Error);
			}

			//we restore non derivable functions
		//	if (this->Train_Method == backprop)
				this->Restore_Non_Derivables();
		}
		
		return tot_error;
	}
	

	//returns the weights of a given layer
	int Get_Weights (Data* Weights, int Numlayer)
	{
		if (Numlayer>this->Number_of_Layers)
			return -1;
		
		//we get dimensions
		int rows = this->Layer[Numlayer-1]->Number_Neurons; //plus bias
		int columns = this->Layer[Numlayer-1]->Number_Inputs+1;
		
	    //we clean data
		Weights->Free_PTRS();
		Weights->NUM_ROWS = rows;
		Weights->NUM_COLUMNS = columns;
		Weights->Get_Memory();

		//We read the inputs weight
		for (int i=0;i<rows;i++)
			for (int j=0;j<columns;j++)
			   Weights->Set_Data (i,j, (double)
				    this->Layer[Numlayer-1]->Tissue[i]->Weights[j]);

		return 0;
	}
	
	//write the given layer weights to disk
	int Write_Weights (char* FileName, int NumLayer)
	{
		Data D;
		D.Precision = 3;
		
		//we get the dat
		if (this->Get_Weights (&D,NumLayer)==0)
		
		    //we write to disk
		    return D.Write(FileName);
		
		//layer problem
		else return -3;
	}
	
	//fixes the weights to content of data
	int Set_Weights (Data* Weights, int Numlayer)
	{
		if (Numlayer>this->Number_of_Layers)
			return -1;	

		//we get dimensions
		int rows = this->Layer[Numlayer-1]->Number_Neurons;
		int columns = this->Layer[Numlayer-1]->Number_Inputs+1;
		
	    //we check data dimensions
		if (Weights->NUM_ROWS != rows || Weights->NUM_COLUMNS != columns)
			return -2;
		
		//We read the inputs weight
		for (int i=0;i<rows;i++)
			for (int j=0;j<columns;j++)
			   this->Layer[Numlayer-1]->Tissue[i]->Weights[j]=
					 Weights->Give_Data (i,j);
			
		return 0;
	}
	
	//reads weights from a file
	int Read_Weights (char* FileName, int NumLayer)
	{
		Data D;
		
		//we read input file
		if (D.Read(FileName)==0)
		    return this->Set_Weights (&D,NumLayer);
		
		//file problem
		else return -3;
	}
				 
	//Performs BackPropagation one time from Last_Layer to first layer
	//this has to be specified because elman and jordan nets have the last layer
	//with feedback and there is not  backprop on that layer
	void Back_Propagation (double* Results)
	{
		//only for debugging
		//fprintf (stdout, "\nLayer %2d Deltas: ", this->Number_of_Layers);
		
		//we adjust first the last layer
		int j;
		
		for (j=0;j<this->Layer[this->Number_of_Layers-1]->Number_Neurons;j++)
		{
			//we get the neuron
			Neuron* aNeuron_ptr = this->Layer[this->Number_of_Layers-1]->Tissue[j];

			//we calculate its delta & correct weights
			aNeuron_ptr->Last_Delta_Adjust_Weights 
				(*(Results+j), //desired output
								//remember that the desired output must be 0 or 1
				 this->Learn_Factor, //learning factor
				 this->Momentum,    //acceleration factor
				 !this->Batch_Train); //If applies changes
				
			//only for debugging
			//fprintf (stdout, "%d: %2.3f ",j, aNeuron_ptr->Delta);
		}

		//then we adjust inner values for all remaining layers
		for (j=(Number_of_Layers-2);j>=0;j--)
		{
			//only for debugging
			//fprintf (stdout, "\nLayer %2d Deltas: ", j+1);

			//we actualize all neurons from layer j
			for (int i=0;i<this->Layer[j]->Number_Neurons;i++)
			{
				//we get the sum of deltas for i next layer
				double sum = 0;
				for (int l=0;l<this->Layer[j+1]->Number_Neurons;l++)
				{
					//we get the l th neuron
					Neuron* aNeuron_ptr = this->Layer[j+1]->Tissue[l];
					//suma = suma + delta (j+1) * Wil (j+1)
					sum = sum + aNeuron_ptr->Delta * aNeuron_ptr->Weights[i];
				}

				//we adjust inner layers weights
				this->Layer[j]->Tissue[i]->Inner_Delta_Adjust_Weights
				                           (sum, //sum of deltas next layer
										   this->Learn_Factor, //Learning Factor
										   this->Momentum,    //acceleration factor
										   !this->Batch_Train); //aplyes changes each vector

				//only for debugging
				//fprintf (stdout, "%d: delta %2.3f sum  %2.3f ",i, 
				//			  this->Layer[j]->Tissue[i]->Delta,sum);
			}
		}

		//only for debugging 
		//fprintf (stdout, "\n");
	}

	//Performs Learning Vector Quantization for current inputs
	//Finds winning neuron and moves all the neurons towards or far from it
	//the net has to be feeded before doing this
	double LVQ (double* Results)
	{
		int winner = -1;

		winner = this->Layer[0]->Find_Winner();

		if (winner==(int)Results[0]) //its the correct class
		//we move weights for the winner
		{
			this->Layer[0]->Tissue[winner]->VQ(true,this->Learn_Factor,0);
			return 0.0;
		}
		else
		//we penalty only the winner
		{
		    this->Layer[0]->Tissue[winner]->VQ(true,-1.0*this->Learn_Factor,0);
			return 1.0;
		}
	}

	//Performs KohonenLearning for current inputs
	//Finds winning neuron and moves all the neurons towards it
	//the net has to be feeded before doing this
	double Kohonen (double actualize_factor)
	{
		int j;
		int winner = -1;
		int renwin, colwin, ren, col;

		//we find the winner
		winner = this->Layer[0]->Find_Winner();

		colwin = (winner+1)%this->Separation;
		renwin = (winner+1)/this->Separation;

		int start = winner - KOHONENDIST * 4 *this->Separation;
		if (start<0) 
			start = 0;

		int end = winner + KOHONENDIST * 4 * this->Separation;
		if (end>=this->Layer[0]->Number_Neurons)
			start = this->Layer[0]->Number_Neurons;

		//we actualize all neurons accondingly with their grid location
		for (j=0;j<this->Layer[0]->Number_Neurons;j++)
		{
			col = (j+1)%this->Separation;
			ren = (j+1)/this->Separation;

			//we get the distance on the grid
			double distance = sqrt((double)(colwin-col)*(colwin-col)+
				                   (double)(renwin-ren)*(renwin-ren))/10.0;

			//we actualize neurons aconding with euclidean distance
			//all neurons are the winner
			this->Layer[0]->Tissue[j]->VQ(
				//depending its distance radial
				true, KOHONENDIST * exp(-distance*distance) * actualize_factor, 0);
		}

		return 1.0;
	}
};

#endif
