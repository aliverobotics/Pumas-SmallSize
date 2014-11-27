typedef struct Matri_ {
	float d;
} Matri;
typedef struct Matrixyf_ {
	float x;
	float y;
} Matrixyf;
typedef struct Matrixyi_ {
	int x;
	int y;
} Matrixyi;
Matri **alloc_Mem2df(int x, int y);
Matrixyf **alloc_Mem2xyf(int x, int y);
Matrixyi **alloc_Mem2xyi(int x, int y);
void libera_matrizd(Matri **m,int row);
void libera_matrizxyf(Matrixyf **m,int row);
void libera_matrizxyi(Matrixyi **m,int row);
int puntomedioentero(int num);
int redondeofi(float cord);
Matri ** distancia_centro(Matri ** dista,int x, int y, int centrox, int centroy);
float distancia_mayor(Matri ** dista,int x, int y);
Matri ** distancia_normalizada(Matri ** dista, Matri ** distnorm,int x, int y, float distmay);
Matrixyi ** quitar_distorsion(Matri ** dista, Matri ** distnorm, Matrixyi ** coordi, int x, int y, int centrox, int centroy, float a, float b, float c, float distmay);
Matrixyi ** medio_coord(Matrixyi ** coordi, int x, int y);
Matrixyi ** distorsionj(int x, int y);
Matrixyi ** distorsionj1(int x, int y, int centrox, int centroy, float a, float b, float c);