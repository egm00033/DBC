// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//



#include "stdafx.h"

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

	//ancho=40;//borrar
	/*if(false){//cambiar imagen por posicion
		ancho=40;
		for (int i = 0; i < ancho; i++)
		{
			for (int j = 0; j < ancho; j++){
				img3[i*ancho+j]=200;
				//printf(" %i\t",imagen[i][j]);
			}
			//printf("\n");
		}
	}*/
	//calcular DF
	DBC c=DBC(img3, ancho, niveldeGris, _C);

	DBC cp=DBC(img3, ancho, niveldeGris, _CPU_profundidad);

	DBC cs=DBC(img3, ancho, niveldeGris, _CPU_superficie);

	DBC g=DBC(img3, ancho, niveldeGris, _GPU);

	DBC g2=DBC(img3, ancho, niveldeGris, _GPU2_0);

	system("pause");
	free(img3);
	return 0;
}

