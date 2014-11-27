/**
 * \file		Draw.cpp
 * \author		fjrg76
 * \date		29-09-05
*/


/**
 * Draw a blob in the workspace area. It draws into the buffer no the screen
 *
 * \param x0	Together y0 are the lower-left corner
 * \param y0
 * \param x1	Together y1 are the upper-right corner
 * \param y1
 * \param b		Buffer RGB that points to the workspace area
 * \param rgb	Define the blob's color
*/
void Draw::Blob( int x0, int y0, int x1, int y1, unsigned char *b, RGB rgb )
{
	int addr,s;

	m_X0 = x0;
	m_Y0 = y0;
	m_X1 = x1;
	m_Y1 = y1;
	m_Pix = b;
	m_RGB = rgb;

	for( int col = m_Y0; col < m_Y1; col++ )
	{
		s = col*MAX_RES_X*3;
		for( int row = m_X0; row < m_X1; row++ )
		{
			addr = row*3 + s;
			*(m_Pix+addr) = m_RGB.b;
			*(m_Pix+addr+1) = m_RGB.g;
			*(m_Pix+addr+2) = m_RGB.r;
		}
	}
}

/**
 * Draw a blob in the workspace area. It draws into the buffer no the screen
 *
 * \param p		Together y0 are the lower-left corner. It method implements a blob fixed size
 * \param b		Buffer RGB that points to the workspace area
 * \param rgb	Define the blob's color
*/
void Draw::Blob( Point p, unsigned char *b, RGB rgb )
{
	int addr,s, x0, x1, y0, y1;

	m_X0 = p.x-m_BlobSize/2;
	m_Y0 = p.y-m_BlobSize/2;
	m_X1 = p.x+m_BlobSize/2;
	m_Y1 = p.y+m_BlobSize/2;
	
	m_Pix = b;
	m_RGB = rgb;


	if( m_Y0 < 0 ) m_Y0 = 0;
	if( m_Y1 > MAX_RES_Y ) m_Y1 = m_ResY;

	if( m_X0 < 0 ) m_X0 = 0;
	if( m_X1 > MAX_RES_X ) m_X1 = m_ResX;

	for( int col = m_Y0; col < m_Y1; col++ )
	{
		s = col*MAX_RES_X*3;
		for( int row = m_X0; row < m_X1; row++ )
		{
			addr = row*3 + s;
			*(m_Pix+addr) = m_RGB.b;
			*(m_Pix+addr+1) = m_RGB.g;
			*(m_Pix+addr+2) = m_RGB.r;
		}
	}

}

/**
 * Draw a blob in the workspace area. It draws into the buffer no the screen
 *
 * \param x0	Together y0 are the lower-left corner
 * \param y0
 * \param x1	Together y1 are the upper-right corner
 * \param y1
 * \param b		Buffer RGB that points to the workspace area
 * \param color	Uses a default color \see colors enum
*/
void RLEDrawBlob( int x0, int y0, int x1, int y1, unsigned char *b, int color)
{
	int addr,s,temp;

	m_X0 = x0;
	m_Y0 = y0;
	m_X1 = x1;
	m_Y1 = y1;
	m_Pix = b;
	m_Color = color;

	for( int col = m_Y0; col < m_Y1; col++ )
	{
		s = col*m_ResX*3;
		for( int row = m_X0; row < m_X1; row++ )
		{
			addr = row*3 + s;

			if (m_Color == orange)
			{
				*(m_Pix+addr) = 0;
				*(m_Pix+addr+1) = 0;
				*(m_Pix+addr+2) = 255
			}
			else if (m_Color == yellow)
			{
				*(m_Pix+addr) = 0;
				*(m_Pix+addr+1) = 230;
				*(m_Pix+addr+2) = 255
			}
			else if (m_Color == blue)
			{
				*(m_Pix+addr) = 255;
				*(m_Pix+addr+1) = 0;
				*(m_Pix+addr+2) = 0;
			}
			else if (m_Color == green)
			{
				*(m_Pix+addr) = 100;
				*(m_Pix+addr+1) = 255;
				*(m_Pix+addr+2) = 60;
			}
			else if (m_Color == pink)
			{
				*(m_Pix+addr) = 244;
				*(m_Pix+addr+1) = 4;
				*(m_Pix+addr+2) = 202;
			}
			else if (m_Color == cyan)
			{
				*(m_Pix+addr) = 255
				*(m_Pix+addr+1) = 255;
				*(m_Pix+addr+2) = 70;
			}
			else if (m_Color == 10)
			{
				*(m_Pix+addr) = 57
				*(m_Pix+addr+1) = 172;
				*(m_Pix+addr+2) = 210;
			}
			else if (m_Color == 11)
			{
				*(m_Pix+addr) = 160;
				*(m_Pix+addr+1) = 160;
				*(m_Pix+addr+2) = 160;
			}
		}
	}
}

/**
 * Draw a line in the workspace
*/
void drawLine( int x0, int y0, int d, float angle, unsigned char *b, int color)
{
	int newX, newY;
	float ang = angle * 3.14159 / 180;
	newX = (int)(d * cos((float)ang) + x0);
	newY = (int)(d * sin((float)ang) + y0);
	int x = x0;
	int y = y0;
	float m;

	m_X0 = x0;
	m_Y0 = y0;
	m_X1 = x1;
	m_Y1 = y1;
	m_Pix = b;
	m_RGB = rgb;

	while (x < newX){  
		m = tan((float)ang);
		y = m * (x-m_X0) + m_Y0;
		if ( x-m_PointThick >= 0 && x-m_PointThick <= m_ResX &&
			y-m_PointThick >= 0 && y-m_PointThick <= m_ResY &&
			x+m_PointThick >= 0 && x+m_PointThick <= m_ResX &&
			y+m_PointThick >= 0 && y+m_PointThick <= m_ResY )
	//	if ( x-m_PointThick >= 0 && y-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y+m_PointThick <= m_ResY)
			RLEDrawBlob(x-m_PointThick, y-m_PointThick,x+m_PointThick,y+m_PointThick,b,color);
		x++;
	}
	x=m_X0;
	while (x > newX){
		m = tan((float)ang);
		y = m * (x-m_X0) + m_Y0;
		if ( x-m_PointThick >= 0 && x-m_PointThick <= m_ResX &&
			y-m_PointThick >= 0 && y-m_PointThick <= m_ResY &&
			x+m_PointThick >= 0 && x+m_PointThick <= m_ResX &&
			y+m_PointThick >= 0 && y+m_PointThick <= m_ResY )
	//	if ( x-m_PointThick >= 0 && y-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y+m_PointThick <= m_ResY)
			RLEDrawBlob(x-m_PointThick, y-m_PointThick,x+m_PointThick,y+m_PointThick,b,color);
		x--;
	}
	while (y < newY){  
		m = tan((float)ang);
		x = (y-m_Y0) / m + m_X0;
		if ( x-m_PointThick >= 0 && x-m_PointThick <= m_ResX &&
			y-m_PointThick >= 0 && y-m_PointThick <= m_ResY &&
			x+m_PointThick >= 0 && x+m_PointThick <= m_ResX &&
			y+m_PointThick >= 0 && y+m_PointThick <= m_ResY )
	//	if ( x-m_PointThick >= 0 && y-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y+m_PointThick <= m_ResY)
			RLEDrawBlob(x-m_PointThick, y-m_PointThick,x+m_PointThick,y+m_PointThick,b,color);
		y++;
	}
	y=m_Y0;
	while (y > newY){  
		m = tan((float)ang);
		x = (y-m_Y0) / m + m_X0;
		if ( x-m_PointThick >= 0 && x-m_PointThick <= m_ResX &&
			y-m_PointThick >= 0 && y-m_PointThick <= m_ResY &&
			x+m_PointThick >= 0 && x+m_PointThick <= m_ResX &&
			y+m_PointThick >= 0 && y+m_PointThick <= m_ResY )
	//	if ( x-m_PointThick >= 0 && y-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y+m_PointThick <= m_ResY)
			RLEDrawBlob(x-m_PointThick, y-m_PointThick,x+m_PointThick,y+m_PointThick,b,color);
		y--;
	}
}

/**
 * Draw a circle in the workspace
 */
void drawCircle( int x0, int y0, int r, unsigned char *b, int color)
{
	m_X0 = x0;
	m_Y0 = y0;
	m_X1 = m_X1;
	m_Y1 = y1;
	m_Pix = b;
	m_RGB = rgb;

	int x = m_X0, m_X1, x2;
	int y = m_Y0, y1, y2;
	int rCuad = pow(r,2);
	while ( pow(x-m_X0,2) <= rCuad )
	{
		y1 = sqrt( rCuad - pow(x-m_X0,2) ) + m_Y0;
		y2 = -1*sqrt( rCuad - pow(x-m_X0,2) ) + m_Y0;
		if (x-m_PointThick >= 0 && x-m_PointThick <= m_ResX &&
			y1-m_PointThick >= 0 && y1-m_PointThick <= m_ResY &&
			x+m_PointThick >= 0 && x+m_PointThick <= m_ResX &&
			y1+m_PointThick >= 0 && y1+m_PointThick <= m_ResY &&
			y2-m_PointThick >= 0 && y2-m_PointThick <= m_ResY &&
			y2+m_PointThick >= 0 && y2+m_PointThick <= m_ResY )
		{
//		if ( x-m_PointThick >= 0 && y1-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y1+m_PointThick <= m_ResY && y2+m_PointThick <= m_ResY){
			RLEDrawBlob(x-m_PointThick, y1-m_PointThick,x+m_PointThick,y1+m_PointThick,b,color);
			RLEDrawBlob(x-m_PointThick, y2-m_PointThick,x+m_PointThick,y2+m_PointThick,b,color);
		}
		x++;
	}
	x = m_X0;
	while ( pow(x-m_X0,2) <= rCuad )
	{
		y1 = sqrt( rCuad - pow(x-m_X0,2) ) + m_Y0;
		y2 = -1*sqrt( rCuad - pow(x-m_X0,2) ) + m_Y0;
		if (x-m_PointThick >= 0 && x-m_PointThick <= m_ResX &&
			y1-m_PointThick >= 0 && y1-m_PointThick <= m_ResY &&
			x+m_PointThick >= 0 && x+m_PointThick <= m_ResX &&
			y1+m_PointThick >= 0 && y1+m_PointThick <= m_ResY &&
			y2-m_PointThick >= 0 && y2-m_PointThick <= m_ResY &&
			y2+m_PointThick >= 0 && y2+m_PointThick <= m_ResY )
		{
//		if ( x-m_PointThick >= 0 && y1-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y1+m_PointThick <= m_ResY && y2+m_PointThick <= m_ResY){
			RLEDrawBlob(x-m_PointThick, y1-m_PointThick,x+m_PointThick,y1+m_PointThick,b,color);
			RLEDrawBlob(x-m_PointThick, y2-m_PointThick,x+m_PointThick,y2+m_PointThick,b,color);
		}
		x--;
	}
// respecto a y
	y = m_Y0;
	while ( pow(y-m_Y0,2) <= rCuad )
	{
		m_X1 = sqrt( rCuad - pow(y-m_Y0,2) ) + m_X0;
		x2 = -1*sqrt( rCuad - pow(y-m_Y0,2) ) + m_X0;
		if (y-m_PointThick >= 0 && y-m_PointThick <= m_ResY &&
			m_X1-m_PointThick >= 0 && m_X1-m_PointThick <= m_ResX &&
			y+m_PointThick >= 0 && y+m_PointThick <= m_ResY &&
			m_X1+m_PointThick >= 0 && m_X1+m_PointThick <= m_ResX &&
			x2-m_PointThick >= 0 && x2-m_PointThick <= m_ResX &&
			x2+m_PointThick >= 0 && x2+m_PointThick <= m_ResX )
		{
//		if ( x-m_PointThick >= 0 && y1-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y1+m_PointThick <= m_ResY && y2+m_PointThick <= m_ResY){
			RLEDrawBlob(m_X1-m_PointThick, y-m_PointThick,m_X1+m_PointThick,y+m_PointThick,b,color);
			RLEDrawBlob(x2-m_PointThick, y-m_PointThick,x2+m_PointThick,y+m_PointThick,b,color);
		}
		y++;
	}
	y = m_Y0;
	y = m_Y0;
	while ( pow(y-m_Y0,2) <= rCuad )
	{
		m_X1 = sqrt( rCuad - pow(y-m_Y0,2) ) + m_X0;
		x2 = -1*sqrt( rCuad - pow(y-m_Y0,2) ) + m_X0;
		if (y-m_PointThick >= 0 && y-m_PointThick <= m_ResY &&
			m_X1-m_PointThick >= 0 && m_X1-m_PointThick <= m_ResX &&
			y+m_PointThick >= 0 && y+m_PointThick <= m_ResY &&
			m_X1+m_PointThick >= 0 && m_X1+m_PointThick <= m_ResX &&
			x2-m_PointThick >= 0 && x2-m_PointThick <= m_ResX &&
			x2+m_PointThick >= 0 && x2+m_PointThick <= m_ResX )
		{
//		if ( x-m_PointThick >= 0 && y1-m_PointThick>= 0 && x+m_PointThick <= m_ResX && y1+m_PointThick <= m_ResY && y2+m_PointThick <= m_ResY){
			RLEDrawBlob(m_X1-m_PointThick, y-m_PointThick,m_X1+m_PointThick,y+m_PointThick,b,color);
			RLEDrawBlob(x2-m_PointThick, y-m_PointThick,x2+m_PointThick,y+m_PointThick,b,color);
		}
		y--;
	}
}