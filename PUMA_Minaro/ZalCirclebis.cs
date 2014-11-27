using System;

namespace ZalSystem
{
	/// <summary>
	/// Summary description for ZalCircle.
	/// </summary>
	public class ZalCircle
	{
		private float centerX;
		private float centerY;
		private float radius;

		public float CenterX
			{get{return centerX;}}
		public float CenterY
			{get{return centerY;}}
		public float Radius
			{get{return radius;}}
		
		public ZalCircle(float CenterX, float CenterY, float Radius)
		{
			centerX = CenterX;
			centerY = CenterY;
			radius  = Radius;
		}

		public bool IsPointWithin(float X, float Y)
		{
			if( ((X - centerX)*(X - centerX)+(Y - centerY)*(Y - centerY)) <= radius*radius)
				return true;
			else
				return false;
		}

		public float CenterDistance(float X, float Y)
		{
			return (float)Math.Sqrt(Math.Pow((X-centerX),2) + Math.Pow(Y-centerY,2));
		}

		public float[] XYRadiusRatio(float X, float Y)
		{
			return new float[]{X/radius, Y/radius};
		}
		
		public float[] RadialIntersection(float X, float Y)
		{
			double m;
			double m2;
			double b;
			double term1;
			double term2;
			double secX;
			double secY;

			//Calculate the slope
			m = (Y-centerY) / (X-centerX);

			m2 = m * m;

			//Calculate the ...
			b = 0;//Y / (m * X);

			//General Eq solution
			term1 = (centerX+centerY-b) / (m2+1);
			term2 = radius*radius*(m2+1) - m2*((centerX*centerX)+(b-centerY)*(b-centerY)) - 2*centerX*(b-centerY);
			term2 = Math.Sqrt(term2);
			term2 = term2/m2+1;
			
			if(X > centerX)
				secX = term1 + term2;
			else
				secX = term1 - term2;
			secY = (m*secX)+b;

			return new float[2]{(float)secX, (float)secY};
		}
	}
}