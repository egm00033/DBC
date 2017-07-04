/**
  @file main.cpp
  @brief define el punto de entrada de la aplicacion de consola. 

  @author Emilio Gallardo Molina
  @date 24/04/2017
*/

#include "stdafx.h"

/**
  @brief Carga la imagen en memoria y llama a cada versión u optimización del algoritmo Differential Box Counting.

  @param argc
  @param argv[]

  @returns EXIT_SUCCESS = 0, EXIT_FAILURE <> 0.
  */
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char *img3;
	bmpInfoHeader info; 
	printf("Leyendo imagen: %s \n",path);
	img3=LoadBMP(path, &info);

	int i,j,blanco;


	printf("Imagen size = (%i x %i)\n",info.height,info.width);
	for (i=0; i<info.height; i++)
	{
		for (j=0; j<info.width; j++)
		{
			//printf("%i %i %i \n",img3[(i*info.width+j)*3],img3[(i*info.width+j)*3+1],img3[(i*info.width+j)*3+2]);
			blanco=0;
			blanco+=img3[(i*info.width+j)*3];
			blanco+=img3[(i*info.width+j)*3+1];
			blanco+=img3[(i*info.width+j)*3+2];
			//printf("blanco %i\n",blanco);
			blanco=blanco/3;
			img3[i*info.height+j]=blanco;//dar el mayor contraste = j%2*256
		}
	}


	long int ancho=info.width;
	long int alto=info.height;
	int niveldeGris=256;

	//calcular DF
	DBC(img3, ancho, niveldeGris, _C);

	DBC(img3, ancho, niveldeGris, _CPU_profundidad);

	DBC(img3, ancho, niveldeGris, _CPU_superficie);

	DBC(img3, ancho, niveldeGris, _GPU1);

	DBC(img3, ancho, niveldeGris, _GPU2_0_0);

	DBC(img3, ancho, niveldeGris, _GPU2_0_1);

	DBC(img3, ancho, niveldeGris, _GPU2_1);

	DBC(img3, ancho, niveldeGris, _GPU2_2);

	system("pause");
	free(img3);
}

