//definition of neuron's class
/*
 *  Neuron.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _Neuron_
#define _Neuron_

#define DYNAMIC 1 //creates all dymamically for exactly number of specified inputs
				  //0 is used when exploring creating new connections on line

double SIGMA = 1.0; //Standard deviation for random neuron's weight

class Neuron
{
public:
    Kind_Functions Kind;
	int Number_Inputs;			   //Number of inputs defined by user (without bias)

#if DYNAMIC
	double *Weights;			   //you have to add bias
	double **Inputs;			   //Adresses of inputs
	double *Delta_Weights;		   //stores total delta for an epoch
#else
	#define MAX_INPUTS 100 //Maximun number of inputs for the neuron
	double Weights[MAX_INPUTS+1];  //you have to add bias
	double *Inputs[MAX_INPUTS+1];  //Adresses of inputs
	double Delta_Weights[MAX_INPUTS+1]; //stores total delta for an epoch
#endif

	double *Output;				   //Adress of output
	double (*Somatic)(double);     //Somatic Function
	double (*Activation)(double);  //Activation Function
	double Delta; //Used for backpropagation

	double dendritic_result;	   //stores value calculated for dentritic function
	double somatic_result;	       //stores value feeded to activation, i.e. x to calculate derivate
			
	int Number_Deltas;			   //Number of times we have accumulated deltas for making the average

	void Assign_Function (Kind_Functions anActivation)
	{
	   switch (anActivation) {
	      case purelin : this->Activation=PURELIN;
		  break;
		  case logsig  : this->Activation=LOGSIG;
		  break;
		  case tansig  : this->Activation=TANSIG;
		  break;
		  case hardlim : this->Activation=HARDLIM;
		  break;
		  case satlin  : this->Activation=SATLIN;
		  break;
		  case ising   : this->Activation=ISING;
		  break;
		  case radbas  : this->Activation=RADBAS;
		  break;
		}
	}
	
	void Set_All_Weights (double Value, double Bias)
	{
		for (int i=0;i<this->Number_Inputs;i++)
		   this->Weights[i] = Value;

		this->Weights[Number_Inputs] = Bias; //bias weight
	}
	
    //receives pointer to activation function
    Neuron(Kind_Functions aKind, int aNumber_Inputs, double* Start_Input, double* anOutput)
	{
	   Number_Inputs = aNumber_Inputs; 
	   this->Kind = aKind;
	   this->Output = anOutput;

#if DYNAMIC
	   //we reserve space for the weights
	   this->Weights = (double*)malloc((this->Number_Inputs+1)*sizeof(double));

	   //we reserve space for the deltas
	   this->Delta_Weights = (double*)malloc((this->Number_Inputs+1)*sizeof(double));

	   //we reserve space for the inputs
	   this->Inputs = (double**)malloc((this->Number_Inputs+1)*sizeof(double*));

	   //we clean input_ptrs
	   for (int i=0;i<=aNumber_Inputs;i++)
#else
	  //we clean all
	   for (int i=0;i<=MAX_INPUTS;i++)
#endif
	   {
		   this->Inputs[i] = Start_Input+i;
		   this->Weights[i] = 0.0;
		   this->Delta_Weights[i] = 0.0;
	   }
	   
	   //we clean weights
	   this->Set_All_Weights (0, 0);
	   
	   *this->Output = 0.0;
	   this->Delta=0;
	   this->Somatic=PURELIN;
	   
	   this->Assign_Function (this->Kind);
	   this->Number_Deltas = 0;
	}
	
	~Neuron()
	{
		free (this->Weights);
		free (this->Delta_Weights);
		free (this->Inputs);
	}

	//finds the euclidean distance between inputs and weights 
	double Neuron::Euclidean (void);
	//sets an special activation function
	void Set_Activation (double (*anActivation)(double));
	//dot ptoduct
	double Dendritic (void);
	//actualizes the neuron values
	void Actualize (void);
	//sets weights randomly
	void Set_Random_Weights (void);
	//prints the inputs
	void Print_Inputs (void);
	//prints the weights
	void Print_Weights (void);
	//prints the output
	void Print_Output (void);
	//clears the output
	void Clear_Output (void);
	//applyes deltas fon an epoch
	void Apply_Deltas (void);
	
	//this method aproximates the pseudo-inverse mattrix using Greville Method
	//_         _		         m      _  T      _m  _m
 	//W(i)new = W(i)old + lf * (d(i) - (W(i)old * X ) X
	void Greville_Adjust_Weights (double desired_output, 
								double learning_factor,
								double momentum,
								//for accelerating network
								bool apply);
								//applyes deltas
	
	void Error_Correction_Adjust_Weights (double desired_output, 
			//remember that the output & desired output must be 0 or 1
								double learning_factor,
								//usually 2
								double momentum,
								//for accelerating network
								bool apply);
								//applyes deltas
	
	//it applies for linear Activations
	void Widrow_Adjust_Weights (double desired_output, 
			//remember that the desired output must be -1 or 1
								double learning_factor,
								//usually 2
								double momentum,
								//for accelerating network
								bool apply);
								//applyes deltas

	//calculates the delta & modify weights for the last layer
	//returns its delta
	void Last_Delta_Adjust_Weights (double desired_output, 
			//remember that the desired output must be 0 or 1
								double learning_factor,
								//usually 2
								double momentum,
								//for accelerating network
								bool apply);
								//applyes deltas

	//calculates the delta & modify weights for inner layer
	void Inner_Delta_Adjust_Weights (double Sum_Deltas,  
	                             //it has to have the sum of delta next layer by next weights of neuron i
							     //must have specific factor
								 double learning_factor,
								//usually 2
								double momentum,
								//for accelerating network
								bool apply);
								//applyes deltas

	//modify weights toward inputs
	void VQ						(bool winner,  
							    //must have specific factor for atracting
								double atracting_factor,
								//factor for repulsion, if not the winner
								double repulsive_factor);
								//usualli 0.1 * atracting
};
	
//sets an special activation function
void Neuron::Set_Activation (double (*anActivation)(double))
{
	this->Activation = anActivation;
}

//dot ptoduct
double Neuron::Dendritic (void)
{
	double Response = 0;
	
	for (int i=0;i<=this->Number_Inputs;i++)
		Response = Response + 
		this->Weights[i] * *this->Inputs[i];
		
	return Response;
}

//finds the euclidean distance between inputs and weights 
double Neuron::Euclidean (void)
{
	int i;
	double result = 0;
	for (i=0;i<this->Number_Inputs;i++)
		result += (this->Weights[i]-*this->Inputs[i])*
		          (this->Weights[i]-*this->Inputs[i]);

	result = sqrt (result);
	return result;
}

//actualizes the neuron values
void Neuron::Actualize (void)
{
	//we get somatic value
	this->dendritic_result = this->Dendritic();
	this->somatic_result   = this->Somatic(this->dendritic_result);
	*this->Output = this->Activation(this->somatic_result);
}

//sets weights randomly
void Neuron::Set_Random_Weights (void)
{
	for (int i=0;i<this->Number_Inputs;i++)
	   this->Weights[i] = Random (0,SIGMA);

	this->Weights[Number_Inputs] = -1.0; //bias weight
}

//prints the inputs
void Neuron::Print_Inputs (void)
{
	//We display the inputs
	fprintf (stdout, "%d\n",this->Number_Inputs);
	
	for (int i=0;i<=this->Number_Inputs;i++)
		 fprintf (stdout,"%d: %2.3f ",i,*this->Inputs[i]);

	fprintf (stdout,"\n");
}

void Neuron::Print_Weights (void)
{
	//We display the weights
	for (int i=0;i<=this->Number_Inputs;i++)
		 fprintf (stdout,"%d: %2.3f ",i,this->Weights[i]);
	fprintf (stdout,"\n");
}

void Neuron::Print_Output (void)
{
	//We display the output
	fprintf (stdout,"%2.3f ",*this->Output);
}

void Neuron::Clear_Output (void)
{
	//We clear the output
	*this->Output=0.0;
}

void Neuron::Apply_Deltas (void)
{
   if (this->Number_Deltas>0) //if we have stored deltas			
   for (int i=0;i<=this->Number_Inputs;i++)
   {
	   this->Weights[i]=
			 //we sum the average of deltas
			 this->Weights[i]+(this->Delta_Weights[i]/(double)this->Number_Deltas);
			 
	   this->Delta_Weights[i] = 0;
   }
   this->Number_Deltas = 0;
}

//this method aproximates the pseudo-inverse mattrix using Greville Method
//_         _		         m      _  T      _m  _m
//W(i)new = W(i)old + lf * (d(i) - (W(i)old * X ) X
void Neuron::Greville_Adjust_Weights (double desired_output, 
							double learning_factor,
							double momentum,
							//for accelerating network
							bool apply)
							//applyes deltas
//it uses dot product for calculating the output
{
	double delta = 0;
	
	//we compute c(d-f)
	delta = learning_factor*(desired_output-this->dendritic_result)
			 + momentum * this->Delta;
	//it does not take the output, it only uses dot product ignorig activation f
	//be careful, if denditic() is not dot product recode this function
			
	for (int i=0;i<=this->Number_Inputs;i++)
		//DW = Dw + c * delta * W
		this->Delta_Weights[i]=this->Delta_Weights[i]+
							   delta* *(this->Inputs[i]);
						 
	//we increase number of deltas stored
	this->Number_Deltas++;
	
	//we add delta to weights
	if (apply==true)
		this->Apply_Deltas();

	this->Delta=delta;
}

void Neuron::Error_Correction_Adjust_Weights (double desired_output, 
		//remember that the output & desired output must be 0 or 1
							double learning_factor,
							//usually 2
							double momentum,
							//for accelerating network
							bool apply)
							//applyes deltas

//this method only applies to linnearly separables input vector sets 
//does not substitute activation for sigmoid and corrects only when 
//difference is 1 or -1, with desired output in 0 or 1
{
	double delta = 0;

	//we implement (d-f)
	delta = learning_factor*(desired_output-*this->Output)
		  + momentum * this->Delta;
		  
	for (int i=0;i<=this->Number_Inputs;i++)
		//DW = Dw + c * delta * W
		this->Delta_Weights[i]=this->Delta_Weights[i]+
							 delta* *(this->Inputs[i]);
						 
	//we increase number of deltas stored
	this->Number_Deltas++;

	//we add delta to weights
	if (apply==true)
		this->Apply_Deltas();

	this->Delta=delta;
}

//it applies for linear Activations
void Neuron::Widrow_Adjust_Weights (double desired_output, 
		//remember that the desired output must be -1 or 1
							double learning_factor,
							//usually 2
							double momentum,
							//for accelerating network
							bool apply)
							//applyes deltas
{
	double delta = 0;

	//we implement (d-f)
	delta = learning_factor*(desired_output-*this->Output)
		  + momentum * this->Delta;
		  
	for (int i=0;i<=this->Number_Inputs;i++)
		//DW = Dw + c * delta * W
		this->Delta_Weights[i]=this->Delta_Weights[i]+
						 delta* *(this->Inputs[i]);
						 
	//we increase number of deltas stored
	this->Number_Deltas++;

	//we add delta to weights
	if (apply==true)
		this->Apply_Deltas();

	this->Delta=delta;
}

//calculates the delta & modify weights for the last layer
//returns its delta
void Neuron::Last_Delta_Adjust_Weights (double desired_output, 
		//remember that the desired output must be 0 or 1
							double learning_factor,
							//usually 2
							double momentum,
							//for accelerating network
							bool apply)
							//applyes deltas
{
	double delta = 0;

	//we implement (d-f) * df/ds
	delta = learning_factor*(desired_output-*this->Output)
		  * derivate(this->somatic_result,*this->Output, this->Activation) 
		  * derivate(this->dendritic_result,this->somatic_result,this->Somatic) 
		//uncomment only if its not purelin otherwise is =1
		  + momentum * this->Delta;
		  
	//we move in the direction of gradient accumulating deltas
	for (int i=0;i<=this->Number_Inputs;i++)
		//DW = Dw + c * delta * W
		this->Delta_Weights[i]=this->Delta_Weights[i]+
							   delta* *(this->Inputs[i]);
						 
	//we increase number of deltas stored
	this->Number_Deltas++;

	//we add delta to weights
	if (apply==true)
		this->Apply_Deltas();

	this->Delta=delta;
}

//calculates the delta & modify weights for inner layer
void Neuron::Inner_Delta_Adjust_Weights (double Sum_Deltas,  
							 //it has to have the sum of delta next layer by next weights of neuron i
							 //must have specific factor
							 double learning_factor,
							//usually 2
							double momentum,
							//for accelerating network
							bool apply)
							//applyes deltas
{
	double delta = 0;

	//we implement delta = derivate of activation funtion * Sum (delta layer j+1 * w[i] layer j+1)
	delta = learning_factor
		  * derivate(this->somatic_result,*this->Output, this->Activation) 
		  * derivate(this->dendritic_result,this->somatic_result,this->Somatic) 
		  //uncomment only if its not purelin otherwise is =1
			* Sum_Deltas
			+ momentum * this->Delta;

	//we move in the direction of gradient
	for (int i=0;i<=this->Number_Inputs;i++)
		//DW = Dw + c * delta *  W
		this->Delta_Weights[i]=this->Delta_Weights[i]+
							   delta* *(this->Inputs[i]);

	//we increase number of deltas stored
	this->Number_Deltas++;
	
	//we add delta to weights
	if (apply==true)
		this->Apply_Deltas();
		
	this->Delta=delta;
}

//modify weights toward	inputs
void Neuron::VQ			   (bool winner,  
							//must have	specific factor	for	atracting
							double atracting_factor,
							//factor for repulsion,	if not the winner
							double repulsive_factor)
							//usualli 0.1 *	atracting
{
	double delta = 0;

	if (winner==true)
		//we move weights toward inputs
		delta = atracting_factor;
	else
		//we move weights far from the inputs
		delta = -1.0 * repulsive_factor;

	//we move in the direction of delta
	for (int i=0;i<=this->Number_Inputs;i++)
		//W = w + c *  (x(t)-w(t))
		this->Weights[i]=this->Weights[i]+
							   delta* 
							   (*this->Inputs[i]-this->Weights[i]);

}

int DemoNeuron (void) {
	
	double inputs[3] = { 1.0, 2.0, 1.0 };
	double output[1] = { 0 };
	
	Neuron aNeuron(logsig, 2, inputs, output);
	aNeuron.Set_Random_Weights();
	
	aNeuron.Print_Inputs();
	aNeuron.Print_Weights();
	
	for (int i=0;i<7;i++)
	{
	   switch (i) {
	      case 0: aNeuron.Activation = PURELIN;
		  break;
		  case 1: aNeuron.Activation = LOGSIG;
		  break;
		  case 2: aNeuron.Activation = TANSIG;
		  break;
		  case 3: aNeuron.Activation = HARDLIM;
		  break;
		  case 4: aNeuron.Activation = ISING;
		  break;
		  case 5: aNeuron.Activation = SATLIN;
		  break;
		  case 6: aNeuron.Activation = RADBAS;
		  break;
	   }
	   aNeuron.Actualize();
	   fprintf (stdout, "%d: ",i);
	   aNeuron.Print_Output();	
	}
	
    return 0;
}

#endif
