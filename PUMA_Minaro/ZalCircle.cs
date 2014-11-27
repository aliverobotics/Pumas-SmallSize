using System;

namespace ZalSystem
{
	/// <summary>
	/// Summary description for ZalCircle.
	/// </summary>
	public class ZalCircle
	{
		private float _centerX;
		private float _centerY;
		private float _radius;

		public float CenterX
		{
			get{return _centerX;}
			set{_centerX = value;}
		}
		public float CenterY
		{
			get{return _centerY;}
			set{_centerY = value;}
		}
		public float Radius
		{
			get{return _radius;}
			set{_radius = value;}
		}
		
		public ZalCircle(float CenterX, float CenterY, float Radius)
		{
			_centerX = CenterX;
			_centerY = CenterY;
			_radius  = Math.Abs(Radius);
		}

		public bool IsPointWithin(double X, double Y)
		{
			if( ( ((X - _centerX)*(X - _centerX))+((Y - _centerY)*(Y - _centerY)) ) <= (_radius*_radius))
				return true;
			else
				return false;
		}

		public float CenterDistance(float X, float Y)
		{
			return (float)Math.Sqrt(Math.Pow((X - _centerX),2) + Math.Pow(Y - _centerY, 2));
		}

		public float[] XYRadiusRatio(float X, float Y)
		{
			return new float[]{Math.Abs(X) / _radius, Math.Abs(Y) / _radius};
		}
		
		public float[] RadialIntersection(float X, float Y)
		{
			double m;
			double m2;
			double term2;
			double secX;
			double secY;
			
			X = X - _centerX;
			Y = Y - _centerY;

			//Calculate the slope
			if(X == 0)
			{
				if(Y < 0)
				{
					return new float[2]{0, -_radius};
				}else
					return new float[2]{0, _radius};
			}
			m = Y / X;
			m2 = m * m;

			term2 = _radius * _radius * (m2 + 1);
			term2 = Math.Sqrt(term2);
			term2 = term2 / (m2 + 1);
		
			secX = term2;
			secY = (m * secX);

			if(X < 0)
			{
				secX *= -1;
				secY *= -1;
			}
			
			return new float[2]{(float)(_centerX+secX), (float)(_centerY+secY)};
		}
	}
}