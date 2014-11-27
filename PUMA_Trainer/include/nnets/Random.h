//Neural Network Class
/*
 *  Random.h
 *
 *  Created by Adalberto H. Llarena on 2/24/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RANDOM_
#define _RANDOM_

double Random (double mean, double sigma)
{
	double max = mean + sigma;
	double min = mean - sigma;

	double num = float(rand())/(float)RAND_MAX; //INSERTAR RANDOM

	return (min+(num*(max-min)));
}

int Random_Int (int min, int max)
{
	double hazard = Random (0.5,0.55);

	if (hazard>=1.0) return (max);
	if (hazard<0) return (min);
	return (int) (min + hazard*(float)(max-min+1)); ;
}

#endif
