//---------------------------------------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>

#include "stdio.h"
#include "Punto.h"
#include "FAtractiva.h"
#include "FRepulsiva.h"
#ifndef VECTOR_H
  #include "Vector.h"
#endif


//---------------------------------------------------------------------------

int  _tmain(int argc, char* argv[])
{
       /**Prueba de la clase Vector*/

        /*printf("Vector 1 \n");
        Punto p1(1.0,1.0);
        Punto p2(4.0,4.0);
        Vector vect1(p1,p2);
        printf("Componente en X: %f \n",vect1.getX());
        printf("Componente en Y: %f \n",vect1.getY());
        printf("Magnitud: %f \n",vect1.getMagnitud());
        printf("Direccion (rad): %f \n",vect1.getDireccion());

        printf("Vector 2 \n");
        Punto p3(1.0,1.0);
        Punto p4(-1.0,1.0);
        Vector vect2(p3,p4);
        printf("Componente en X: %f \n",vect2.getX());
        printf("Componente en Y: %f \n",vect2.getY());
        printf("Magnitud: %f \n",vect2.getMagnitud());
        printf("Direccion (rad): %f \n",vect2.getDireccion());

        printf("Suma de 1 y 2 \n");
        Vector vect3=vect1+(vect2);

        printf("Componente en X: %f \n",vect3.getX());
        printf("Componente en Y: %f \n",vect3.getY());
        printf("Magnitud: %f \n",vect3.getMagnitud());
        printf("Direccion (rad): %f \n",vect3.getDireccion());
        printf("coordenadas: %f,%f \n",vect3.getP1().x,vect3.getP1().y);
        printf("coordenadas: %f,%f \n",vect3.getP2().x,vect3.getP2().y);

        printf("Resta de 1 y 2 \n");
        Vector vect4=vect1-(vect2);

        printf("Componente en X: %f \n",vect4.getX());
        printf("Componente en Y: %f \n",vect4.getY());
        printf("Magnitud: %f \n",vect4.getMagnitud());
        printf("Direccion (rad): %f \n",vect4.getDireccion());
        printf("coordenadas: %f,%f \n",vect4.getP1().x,vect4.getP1().y);
        printf("coordenadas: %f,%f \n",vect4.getP2().x,vect4.getP2().y);
        scanf("Presione una tecla para continuar %s");  */

        /*Prueba de la clase FAtractiva*/
        printf("FUERZA ATRACTIVA \n");
        Punto p1(1.0,1.0);
        Punto p2(5.0,4.0);
        FAtractiva f1(p1,p2);
        printf("Componente en X: %f \n",f1.getX());
        printf("Componente en Y: %f \n",f1.getY());
        printf("Magnitud: %f \n",f1.getMagnitud());
        printf("Direccion (rad): %f \n",f1.getDireccion());
        printf("coordenadas: %f,%f \n",f1.getP2().x,f1.getP2().y);

        printf("FUERZA REPULSIVA \n");
        Punto p3(1.0,1.0);
        Punto p4(2.0,2.0); //Centro de masa del obstaculo
        FRepulsiva f2(p3,p4);
        printf("Componente en X: %f \n",f2.getX());
        printf("Componente en Y: %f \n",f2.getY());
        printf("Magnitud: %f \n",f2.getMagnitud());
        printf("Direccion (rad): %f \n",f2.getDireccion());
        printf("coordenadas: %f,%f \n",f2.getP2().x,f2.getP2().y);

        printf("FUERZA RESULTANTE \n");
        Vector vect4=f1+(f2);
        printf("Componente en X: %f \n",vect4.getX());
        printf("Componente en Y: %f \n",vect4.getY());
        printf("Magnitud: %f \n",vect4.getMagnitud());
        printf("Direccion (rad): %f \n",vect4.getDireccion());
        printf("coordenadas: %f,%f \n",vect4.getP1().x,vect4.getP1().y);
        printf("coordenadas: %f,%f \n",vect4.getP2().x,vect4.getP2().y);
        scanf("Presione una tecla para continuar %s");

        return 1;
}
//---------------------------------------------------------------------------
