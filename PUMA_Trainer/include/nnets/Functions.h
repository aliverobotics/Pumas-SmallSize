/*
 *  Functions.h
 *  McBrain
 *
 *  Created by Adalberto H. Llarena on 12/2/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef _Functions_
#define _Functions_

 typedef enum    {  logsig=0,
                    hardlim=1,
					purelin=2,
					tansig=3,
					satlin=4,
					ising=5,
					radbas=6
                 } Kind_Functions;
				 

//Activation Functions
double Step_Function (double value, //Input value from Soma
                      double level, //level of comparisson = 0, usually bias
					  double Max_Value, //Maximun value returned
					  double Min_Value //Minimun value returned
					  )
{
	if (value>level) return Max_Value;
	else return Min_Value;
}


double Sigmoid_Function (double value, //Input value from Soma
					     double offset //Y offset
						 ) 
{	
    //sigmoid function
	return  offset + 1/(1+exp(-1*value));
}
	
double Ramp_Function (double value, //Input value from Soma
					  double minlevel, //level minimun for start of ramp
					  double maxlevel, //level maximun for end of ramp
					  double Max_Value, //Maximun value returned
					  double Min_Value //Minimun value returned
					  )
{
	if (value<minlevel) return Min_Value;
	else if (value>maxlevel) return Max_Value;
	     else return Min_Value + (Max_Value-Min_Value)/(maxlevel-minlevel) * value;
}
					  
double Gaussian_Function (double value, //Input value from Soma
						  double k,
						  double s) //deviation = sigma)
{
    return k * exp (-1.0f*value*value/(2.0f*s*s));
}

//functions according to mathlab
double LOGSIG(double value) //Input value from Soma
{
	return (double) (1.0f/(1.0f+exp(-1.0f*value)));
}

double HARDLIM(double value) //Input value from Soma
{
    if (value>0) return (double)1.0;
	else return (double)0.0;
}

double PURELIN(double value) //Input value from Soma
{
	return value;
}

double TANSIG(double value) //Input value from Soma
{
	return (double) (2.0f/(1.0f+exp(-2.0f*value))-1.0f);
}

double SATLIN(double value) //Input value from Soma
{
	if (value<=0) return (double) 0.0;
	else if (0<=value && value<=1) return value;
	     else return (double) 1.0f;
}

double ISING(double value) //Input value from Soma
{
	if (value<0) return (double) -1.0;
	     else return (double) 1.0f;
}

//is identical to tansig
double TANH(double value)
{
   return (double) ( (exp(value)-exp(-1.0f*value))/
					 (exp(value)+exp(-1.0f*value))  );
}

//Radial basis gaussian
double RADBAS(double value) //Input value from Soma
{
	return (double) exp(-1.0f*value*value);
}

double derivate(double x, double value, double (*operation)(double))
{
	if (operation==LOGSIG || operation==HARDLIM)
	   return (double)(value*(1.0f-value));
	else
		if (operation==PURELIN || operation==SATLIN) 
			return (double)1.0f;
		else
			if (operation==TANSIG || operation==ISING) 
				return (double)(value+1.0f)*(1.0f-value);
			else
				if (operation==RADBAS) 
				//derivate = exp(-x^2)(-2x) 
					return (double)(-2.0f*x*value);
				else
					return (double)0.0f; //unknown function
}

#endif
