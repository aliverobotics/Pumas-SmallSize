/** 
 * \file		draw.h
 * \author		fjrg76
 * \date		29-09-05
*/

/**
 * This class implements the methods needed to draw into the working area
 */
 
/* RGB --> ColorDefs.h
Point --> PointDefs.h
byte --> GeneralDefs
*/

class Draw
{
  public:
	Draw( int resx, int resy, int bsize );
	void Blob( int x0, int y0, int x1, int y1, unsigned char *b, RGB rgb );
	void Blob( Point p, unsigned char *b, RGB rgb );
	void Blob( int x0, int y0, int x1, int y1, unsigned char *b, int color);
	void Line( int x0, int y0, int d, float angle, unsigned char *b, int color);
	void Circle( int x0, int y0, int r, unsigned char *b, int color);

  private:
	int		m_X0, m_Y0, m_X1, m_Y1;
	byte	*m_Pix;
	int		m_Radius;
	float	m_Angle;
	int		m_Color;
	RGB		m_RGB
	int		m_BlobSize;
	int		m_ResX, m_ResY;
};