#include "FRepulsiva.h"


FRepulsiva::~FRepulsiva(void)
{
}

/**
 * Permite calcular el valor de la fuerza de repulsion
 */
void FRepulsiva::calculate(void)
{
   double distancia=this->p1.euclidianDistance(this->p2);

   if(distancia<D0)
     {x=(-1)*ETHA*((1/distancia)-(1/D0))*(1/pow(distancia,2))*(x/distancia);
      y=(-1)*ETHA*((1/distancia)-(1/D0))*(1/pow(distancia,2))*(y/distancia);
     }
   else
     {
      x=0;
      y=0;
     }
    //Actualizar algunos componentes del vector fuerza
       magnitud=sqrt(pow(x,2)+pow(y,2));
       p2.x=p1.x+x;
       p2.y=p1.x+y;
       this->setDireccion();


}
