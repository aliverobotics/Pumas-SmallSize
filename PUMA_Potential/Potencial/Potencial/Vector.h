#pragma once
#ifndef VECTOR_H
 #define VECTOR_H
#endif

#include "Punto.h"
#include "math.h"

#define PI 3.141592653589

/**
 *Permite implementar los atributos y metodos de un Vector
 */
class Vector
{
	
protected:
        Punto p1,p2;     //Almacenan las coordenadas del vector en el plano.
        double x,y;      //Almacena la magnitud de las componentes en X y Y del vector.
        double magnitud; //Almacena la magnitud o norma del vector
        double direccion;//Almacena el angulo o direccion del vector
public:
/** Constructor cuyos parametros define la posicion del robot en el plano*/
	Vector(Punto _p1,Punto _p2);

/**Constructor por defecto*/
	Vector(void);
/** Metodo Destructor 

	~Vector(void);
/**Calcula la norma o magnitud del vector*/
	void Vector::norma();

/**Calcula la direccion o angulo del vector*/
        void Vector::setDireccion();

/** Setter and Getter*/
    double Vector::getMagnitud(void);
	double Vector::getDireccion(void);
	double Vector::getX(void);
	double Vector::getY(void);
    Punto Vector::getP1(void);
    Punto Vector::getP2(void);

	void Vector::setMagnitud(double magnitud);
	void Vector::setDireccion(double direccion);
	void Vector::setX(double x);
	void Vector::setY(double y);

/**Sobrecarga del operador '*' para multiplicar por un escalar*/
        Vector operator*(double fac){x*=fac;y*=fac;return *this;}

/**Sobrecarga del operador '-' se aplica el metodo del paralelogramo*/
	Vector operator-(Vector q){
	    Punto pt1=p1;
            q.p2=q.p2.desplazar(q.x*-2,q.y*-2); //Dar sentido negativo al vector q
            Punto pt2=q.p2.desplazar(x,y);   //Poner q al final del vector a restar
            return Vector(pt1,pt2);
	 }

/**Sobrecarga del operador '+' se aplica el metodo del paralelogramo*/
	Vector operator+(Vector q){
	  Punto pt1=p1;
      Punto pt2=q.p2.desplazar(x,y);
      return Vector(pt1,pt2);
	}

/**Sobrecarga del operador '/' para dividir por un escalar */
	Vector operator/(double div){this->x/=div;this->y/=div; return *this;}



};
