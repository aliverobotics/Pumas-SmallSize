#pragma once
#include "Vector.h"

#include "stdio.h"

//Factores de calibracion de la fuerza de atraccion
#define EPSILON_1 1.0
#define EPSILON_2 1.0
#define D1 5.0 //Umbral de accion de la fuerza de atraccion


/**
 *Permite implementar la fuerza atractiva de un campo potencial
 */
class FAtractiva : public Vector
{

public:
	FAtractiva(Punto p1, Punto p2):Vector(p1,p2){calculate();}
    ~FAtractiva(void);
	void FAtractiva::calculate(void);
};

