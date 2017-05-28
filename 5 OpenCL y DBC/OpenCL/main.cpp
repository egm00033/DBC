// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//



#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *path=".\\images\\D3.bmp";
	long int ancho=0;
	long int alto=0;
	int niveldeGris=256;
	

	//cargar gif
	int **imagen=leerImagenBMP(path, ancho, alto);//carga en memoria una matriz con la información de la imagen




	printf("imagen leida\n");
	
	if(false){//cambiar imagen por posicion
		ancho=8;
		for (int i = 0; i < ancho; i++)
		{
			for (int j = 0; j < ancho; j++){
				imagen[i][j]=i*ancho+j;
				//printf(" %i\t",imagen[i][j]);
			}
			//printf("\n");
		}
	}

	DBC c=DBC(imagen, ancho, niveldeGris);


	system("pause");
	return 0;
}

