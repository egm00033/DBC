#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "Util.h"
#include "DBC.h"
/**
*box rgb.cpp: define el punto de entrada de la aplicación de consola.
*
*
*
*/

int _tmain(int argc, _TCHAR* argv[])
{
	//indica la ruta de la imagen a cargar

	char *path=".\\images\\D3.bmp";
	long int ancho=0;
	long int alto=0;
	int niveldeGris=256;

	//cargar gif
	int **imagen=leerImagenBMP(path, ancho, alto);//carga en memoria una matriz con la información de la imagen


	//cambiar imagen por posicion
	for (int i = 0; i < ancho; i++)
	{
		for (int j = 0; j < ancho; j++)
		imagen[i][j]=i*ancho+j;
	}
	//crea una instancia de Differential Box-Counting
	DBC c=DBC(imagen, ancho, niveldeGris);
	//c.mostrarGrafica();

	system ("pause");

	return EXIT_SUCCESS;/*Indica que el programa ha finalizado con éxito*/
}

