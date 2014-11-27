#include "FAtractiva.h"

FAtractiva::~FAtractiva(void)
{
}
/**
 * Permite calcular el valor de la fuerza de atracciòn
 */
void FAtractiva::calculate(void)
{
   double distancia=this->p1.euclidianDistance(this->p2);

   if(distancia<=D1)
     {x=EPSILON_1*x;
      y=EPSILON_1*y;
     }
   else
     {x=(EPSILON_1/distancia)*x;
      y=(EPSILON_1/distancia)*y;
     }
    //Actualizar algunos componentes del vector fuerza
       magnitud=sqrt(pow(x,2)+pow(y,2));
       p2.x=p1.x+x;
       p2.y=p1.x+y;
       this->setDireccion();

}

