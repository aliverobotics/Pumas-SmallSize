#include "Vector.h"


Vector::Vector(Punto _p1,Punto _p2)
{      	p1=_p1;
	    p2=_p2;
        this->norma();
        this->setDireccion();
}

Vector::Vector(void)
{

}


Vector::~Vector(void)
{
}

/**Calcula la norma o magnitud del vector*/
void Vector::norma(){
	    x=(p2.x-p1.x);
            y=(p2.y-p1.y);
	    magnitud=sqrt(pow(x,2)+pow(y,2));

	  }


/**
 *Calcula la direccion o angulo del vector
 */
void Vector::setDireccion()
{
        //Cuando el vector cae en el cuadrante I
          if(x>0&&y>0)
           direccion=atan(y/x);
        //Cuando el vector cae en el cuadrante II
          if((x<0&&y>0))
           direccion=atan(y/x)+PI;
        //Cuando el vector cae en el cuadrante III
          if((x<0&&y<0))
           direccion=atan(y/x)+PI;
        //Cuando el vector cae en el cuadrante IV
          if((x>0&&y<0))
           direccion=atan(y/x)+(2*PI);

}

/**
 * Setter and Getter
 */

    double Vector::getMagnitud(){
         return this->magnitud;
        }
	double Vector::getDireccion(){
         return this->direccion;
        }

	double Vector:: getX(){
         return x;
        }
	double Vector::getY(){
         return y;
        }

    Punto Vector::getP1(){
         return p1;
        }

    Punto Vector::getP2(){
         return p2;
        }
