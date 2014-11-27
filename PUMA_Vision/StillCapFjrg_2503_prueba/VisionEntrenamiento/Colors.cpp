#include "stdafx.h"
#include "colors.h"

#define MIN(x, y) ( (x)<(y) ? (x) : (y) )
#define MAX(x, y) ( (x)>(y) ? (x) : (y) )
#define RINT(x) ( (x)-floor(x)>0.5? ceil(x) : floor(x) )



extern float acost[2000];

/*==================================================================================
*						RGB2HSI()
==================================================================================*/

/** Converts from RGB to HSI
*
* \param myrgb	Color in RGB
*
* \return Color in HSI
*/
HSI RGB2HSI(RGB myrgb){
	int theta, m, iTemp;
	float temp, fTheta;
	HSI myhsi;
	
#ifndef _ARC_COS_BY_TABLE
	
	/* 17-mar-06 
	 *
	 * Implenta la función arcCos por tabla
	 */
	temp = ((2*myrgb.r-myrgb.g-myrgb.b)) / (2*sqrt((float)((myrgb.r-myrgb.g)*(myrgb.r-myrgb.g)+(myrgb.r-myrgb.b)*(myrgb.g-myrgb.b))));
	temp = 1000.0*(1.0+temp);
	iTemp = (int)temp;
	fTheta = acost[iTemp];
	theta = (int)( fTheta * 180.0/3.141592 );
	
#else
	temp = (2*myrgb.r-myrgb.g-myrgb.b) / (2*sqrt((float)((myrgb.r-myrgb.g)*(myrgb.r-myrgb.g)+(myrgb.r-myrgb.b)*(myrgb.g-myrgb.b))) );
	fTheta = acos( temp );
	fTheta *= 180.0/3.141592;
	theta = (int)fTheta;
	
	theta = (int)( acos( (2*myrgb.r-myrgb.g-myrgb.b) / (2*sqrt((float)((myrgb.r-myrgb.g)*(myrgb.r-myrgb.g)+(myrgb.r-myrgb.b)*(myrgb.g-myrgb.b))) )) * 57.29);	
#endif // _ARC_COS_BY_TABLE

	//theta = (int)( acos( (2*-myrgb.b) / (2*sqrt(+(-myrgb.b)*(-myrgb.b)) )) * 57.29);
	
	myhsi.h = myrgb.b <= myrgb.g ? theta : (360-theta);

	m = myrgb.r <= myrgb.g ? myrgb.r : myrgb.g;
	m = m <= myrgb.b ? m : myrgb.b;
	m = m <= 0 ? 1 : m;

	temp = (765 / (float)(myrgb.r+myrgb.g+myrgb.b) );
	temp = temp * (float)m;
	myhsi.s = 255- (int)temp;

	//myhsi.s = (int)( 255 - (765 / (myrgb.r+myrgb.g+myrgb.b) ) * m );

	myhsi.i = (int)( (myrgb.r+myrgb.g+myrgb.b) / 3 );
	return myhsi;
}

/*
HSI Rgb2Hsi( RGB rgb )
{
	HSI hsi;
	int theta = (int)( acos( (2*rgb.r-rgb.g-rgb.b) / (2*sqrt((double)((rgb.r-rgb.g)*(rgb.r-rgb.g)+(rgb.r-rgb.b)*(rgb.g-rgb.b))) )) * 57.29);
		
	hsi.h = rgb.b <= rgb.g ? theta : (360-theta);

	int m = rgb.r <= rgb.g ? rgb.r : rgb.g;
	m = m <= rgb.b ? m : rgb.b;
	m = m <= 0 ? 1 : m;

	float temp = (765 / (float)(rgb.r+rgb.g+rgb.b) );
	temp = temp * (float)m;
	hsi.s = 255- (int)temp;
	
	hsi.i = (int)( (rgb.r+rgb.g+rgb.b) / 3 );

	return hsi;
}*/