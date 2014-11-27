//definition of layer of neurons class
/*
 *  NLayers.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NLAYERS_
#define _NLAYERS_

double INIT_WEIGHT = 1.0; //init weight for neuron not random equal to input

class NLayer
{
public:
	int Number_Neurons;
	int Number_Layer;
	int Number_Inputs;
	Kind_Functions Kind;

	Neuron **Tissue;

	//constructor definition
	NLayer(int aNumber_Layer, int Num_Neurons, Kind_Functions aKind, int Num_Inputs, double* Inputs,
		    int Connections[], double *Outputs, bool random, bool trace)

			//random == false : All weights 1, bias -1
			//          true  : All weights random
	{
		this->Number_Layer = aNumber_Layer;
		this->Number_Neurons = Num_Neurons;
		this->Number_Inputs = Num_Inputs;
		this->Kind = aKind;

		if (trace==true)
		fprintf (stdout,"Layer %d:   %d Neurons, Kind %d, %d Inputs\n",this->Number_Layer,
			                       this->Number_Neurons,
								   this->Kind,
								   Num_Inputs);

	    //we reserve space for the neurons
	    this->Tissue = (Neuron**)malloc(Num_Neurons*sizeof(Neuron*));

		for (int i=0;i<Num_Neurons;i++)
		{
			this->Tissue[i] = new Neuron (aKind, Num_Inputs, Inputs, Outputs+i);

			int numConnections = 0;
			//we put the kind of neuron

			this->Tissue[i]->Assign_Function (aKind);

			if (trace==true)
			fprintf (stdout, "Neuron %d: ",i);

			//we define the inputs according with architecture
			for(int j=0;j<Num_Inputs;j++) //don't forget the bias
			{
			   if (trace==true)
			       fprintf (stdout, " %d",Connections[i*Num_Inputs+j]);

			   if (Connections[i*Num_Inputs+j]==1)
			   {
				   //We connect an input to the Neuron
				   this->Tissue[i]->Inputs[numConnections]=&Inputs[j];
				   numConnections++;
			   }
			}

			if (trace==true)
			fprintf (stdout, " Num_Connections : %d \n",numConnections);

			this->Tissue[i]->Inputs[numConnections]=&Inputs[Num_Inputs]; //bias conencted

			this->Tissue[i]->Number_Inputs = numConnections;

			//we assign the initial weights
			if (random==false) 
				this->Tissue[i]->Set_All_Weights (INIT_WEIGHT, -1.0);
			else 
				this->Tissue[i]->Set_Random_Weights ();

			this->Tissue[i]->Output = &Outputs[i];
		}
	}

	//destructor definition
	~NLayer()
	{
		free (this->Tissue);
	}

	//sets or changes an ctivation function for all neurons in the layer
    void Assign_Function (Kind_Functions aKind)
	{
		for (int i=0;i<this->Number_Neurons;i++)
			this->Tissue[i]->Assign_Function (aKind);
	}
			
	void Apply_Deltas (void)
	{
		for (int i=0;i<this->Number_Neurons;i++)
			//we apply the delta weights
			this->Tissue[i]->Apply_Deltas();		
	}
	
	void Reset_Weights (bool random)
	{
		for (int i=0;i<this->Number_Neurons;i++)

			//we assign the initial weights
			if (random==false) 
				this->Tissue[i]->Set_All_Weights (INIT_WEIGHT, -1.0);
			else 
				this->Tissue[i]->Set_Random_Weights ();
	}

	void Set_Weights (double init_weight, double bias)
	{
		for (int i=0;i<this->Number_Neurons;i++)
			this->Tissue[i]->Set_All_Weights (init_weight, bias);
	}

	void Print_Inputs (void)
	{
		fprintf(stdout,"Layer %d Inputs: \n", this->Number_Layer);

	    //We display the inputs
	    for (int i=0;i<this->Number_Neurons;i++)
	    {
			fprintf(stdout,"Neuron %d: ",i);
			this->Tissue[i]->Print_Inputs();
	    }
	}

    void Print_Weights (void)
	{
	    fprintf(stdout,"Layer %d Weights: \n", this->Number_Layer);

	    for (int i=0;i<this->Number_Neurons;i++)
	    {
		    fprintf(stdout,"Neuron %d: ",i);
		    this->Tissue[i]->Print_Weights();
	    }
	}

    void Print_Outputs (void)
	{
	    fprintf(stdout,"Layer %d Outputs: \n", this->Number_Layer);

	    for (int i=0;i<this->Number_Neurons;i++)
		    this->Tissue[i]->Print_Output();

	    fprintf(stdout,"\n");
	}

	void Clear_Outputs (void)
	{
	    for (int i=0;i<this->Number_Neurons;i++)
		    this->Tissue[i]->Clear_Output();
	}

	void Actualize (bool trace)
	{
		if (trace==true)
		{
			this->Print_Inputs();
			this->Print_Weights();
		}
	
		for (int i=0;i<this->Number_Neurons;i++)
			this->Tissue[i]->Actualize();
			
		if (trace==true)
			this->Print_Outputs();
	}

	void Print(void)
	{
		this->Print_Inputs();
		this->Print_Weights();
		this->Print_Outputs();
	}

	//finds the winning neuron
	int Find_Winner(void)
	{
		int winner = -1;
		double min = 1e+20;
		int j;

		//we find the winner
		for (j=0;j<this->Number_Neurons;j++)
		{
			//we get the euclidean distance
			double value = this->Tissue[j]->Euclidean();
		    if (value<min)
			{
				min = value;
				winner = j;
			}
		}

		return winner;
	}

	//Performs Vector Quantization for current inputs
	//Finds winning neuron and moves all the neurons towards or far from it
	//the net has to be feeded before doing this
	double Vector_Quantization (double atracting_factor)
	{
		int j;
		double min = 1.0e+20; 
		int winner = -1;

		//we find the winner
		winner = this->Find_Winner();

		if (winner>0)
		{
			min = this->Tissue[winner]->Euclidean();
			//we move weights
			for (j=0;j<this->Number_Neurons;j++)
				this->Tissue[j]->VQ(j==winner,atracting_factor,atracting_factor*0.1);

			return min;
		}

		return -1.0;
	}

}; //end class NLayer

#endif
