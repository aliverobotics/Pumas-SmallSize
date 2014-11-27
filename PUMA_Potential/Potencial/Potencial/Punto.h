#ifndef PUNTO_H
#define PUNTO_H
#include "math.h"

/**
 * Implementa la funcionalidad de un punto en un espacio bidimensional
 */
class Punto 
{
      public:
      double x,y;

      /**Constructor */
      Punto(double ax,double ay):x(ax),y(ay){};

      /** Constructor por defecto*/
      Punto(){};

      /**Sobrecarga del operador '*' */
      Punto operator*(double v){x*=v;y*=v;return *this;}

      /**Sobrecarga del operador '-' */
      Punto operator-(Punto q){return Punto(this->x - q.x,this->y-q.y);}

      /**Sobrecarga del operador '+' */
      Punto operator+(Punto q){return Punto(this->x + q.x,this->y + q.y);}

      /**Sobrecarga del operador '/' */
      Punto operator/(double div){return Punto(this->x / div,this->y /div);}

      /**Distancia euclidiana entre dos puntos*/
	  double Punto::euclidianDistance(Punto q){
		  return sqrt(pow((this->x-q.x),2)+pow((this->y-q.y),2));
	  }
      /**Permite desplazar un punto a unidades en x, b unidades en y */
          Punto Punto::desplazar(double a,double b){return Punto(x+=a,y+=b);}

};

#endif


