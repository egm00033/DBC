#pragma once
#include "stdafx.h"
/*
*Util encapsula todas las funciones necesarias para el resto de clases
*
*/
typedef struct interpretacion{
	//guarda la sumatoria de el valor maximo-minimo+1 para sxs
	float y; 
	//guarda la relacion del tamaño de la muestra s/ancho de la imagen
	float x;
}interpretacion;


int **leerImagenBMP(char *nome,long &ancho,long &alto);

