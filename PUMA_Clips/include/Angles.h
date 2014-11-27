/*
 *  Angles.h
 *  Jander
 *
 *  Created by Adalberto Llarena on 5/12/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef _ANGLES_
#define _ANGLES_

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

//gives the maximun
double Max (double ma, double mb)
{
	if (ma>mb)
		return ma;
	else
		return mb;
}

//gives the minimun
double Min (double ma, double mb)
{
	if (ma<mb)
		return ma;
	else
		return mb;
}

/***************************************************/
/*                Extra Stuff                      */
/***************************************************/

/* Convert from radians to degrees */
float rad2deg( float rad ) {
	return( rad*180.0/M_PI );
}

/* Convert from degrees to radians */
float deg2rad( float deg ) {
	return( deg*M_PI/180.0 );
}

/* ¬¨¬¨  its obvious */
float normalizeAngle( float angle ) {
   
   while( angle < -M_PI ) angle += (2.0*M_PI);
   while( angle >  M_PI ) angle -= (2.0*M_PI);
			
   return( angle );
}

// funcion que devuelve el angulo de un vector en radianes
float Angulo(float VX, float VY)
{
	if (fabs(VX)<0.1 && fabs(VY)<0.1) return 0;
	if (VX==0)
		if (VY >0)
		   return M_PI/2.0; // pi/2 radianes = 90o
		else
		   return 1.5 * M_PI; //-pi / 2
	else
		if (VY==0)
		   if (VX >0)
		      return 0; // 0 radianes = 0o
		   else
			  return M_PI; //pi
		else
		{
		   double Theta = atan(VY/VX);

		   if (VY<0)
			   if (VX<0)
				   return M_PI + Theta; //3er cuadrante
			   else
				   return 2*M_PI + Theta; //4o cuadrante
		   else
			   if (VX<0)
				   return M_PI + Theta; //2o cuadrante
			   else
				   return Theta; //1er cuadrante
		}
}

#endif
