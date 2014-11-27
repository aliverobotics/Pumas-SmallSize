#include "images.h"
#include "colores.h"

typedef struct Archivo_ {
	FILE *archvec;
	int activo;
} Archivo;

Archivo crear_archivo(int color);
Archivo crear_archivoh(int color);
Archivo inicio_archivo(Archivo nuevo);
Archivo escribemat_archivo(Archivo nuevo, Matr ** in, int x, int y);
Archivo escribemath_archivo(Archivo nuevo, HSI ** in, int x, int y);
void cerrar_archivo(Archivo nuevo);
void guardar_matriz(Matr ** in, int x, int y, int color);
void guardar_matrizh(HSI ** in, int x, int y, int color);