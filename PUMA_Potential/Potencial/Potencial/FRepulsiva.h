#pragma once
#ifndef VECTOR_H
  #include "Vector.h"
#endif

#include "stdio.h"

//Factores de calibracion de la fuerza de atraccion
#define ETHA 2.0
#define D0 8.0 //Umbral de accion de la fuerza de repulsion



/**
 *Permite implementar la fuerza repulsiva de un campo potencial
 */
class FRepulsiva :public Vector
{
public:
	FRepulsiva(Punto p1, Punto p2):Vector(p1,p2){this->calculate();}
       	~FRepulsiva(void);
        /**
         * Permite realizar el calculo de la fuerza repulsiva
         */
        void FRepulsiva::calculate(void);
};
