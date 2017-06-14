// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//



#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char *img3;
	bmpInfoHeader info; 
	img3=LoadBMP(path, &info);

	int i,j,blanco;


	printf("tamaño %i ancho alto %i %i\n",info.imgsize,info.height,info.width);
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


	if(false){//cambiar imagen por posicion
		ancho=8;
		for (int i = 0; i < ancho; i++)
		{
			for (int j = 0; j < ancho; j++){
				img3[i*ancho+j]=i*ancho+j;
				//printf(" %i\t",imagen[i][j]);
			}
			//printf("\n");
		}
	}
	//calcular DF
	DBC c=DBC(img3, ancho, niveldeGris);
	miPrograma = _CPU;
	c=DBC(img3, ancho, niveldeGris);



	system("pause");
	free(img3);
	return 0;
}

