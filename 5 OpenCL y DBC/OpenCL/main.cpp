// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//



#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char *img3;
	bmpInfoHeader info; 
	img3=LoadBMP(".\\images\\plana.bmp", &info);

	int i,j,blanco;
	int **imagen;
	imagen =(int **) calloc(info.height,sizeof(int *));
	for (int i = 0; i < info.height; i++){
		imagen[i]=(int *) calloc(info.width,sizeof(int));
	}

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
			imagen[i][j]=blanco;//dar el mayor contraste = j%2*256
		}
	}

	
	long int ancho=info.width;
	long int alto=info.height;
	int niveldeGris=256;


	//cargar gif
	/*
	char *path=".\\images\\D3.bmp";
	imagen=leerImagenBMP(path, ancho, alto);//carga en memoria una matriz con la información de la imagen
	*/


	/*if(false){//cambiar imagen por posicion
	ancho=8;
	for (int i = 0; i < ancho; i++)
	{
	for (int j = 0; j < ancho; j++){
	imagen[i][j]=i*ancho+j;
	//printf(" %i\t",imagen[i][j]);
	}
	//printf("\n");
	}
	}*/
	//calcular DF
	DBC c=DBC(imagen, ancho, niveldeGris);


	//mostrar imagen
	system (".\\images\\grafica.bmp");

	system("pause");
	return 0;
}

