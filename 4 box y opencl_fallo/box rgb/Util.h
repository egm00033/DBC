#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define MAX_NOMARCH 256

/*
*Util encapsula todas las funciones necesarias para el resto de clases
*
*/


//Estructura para almacenar el resultado de una iteraccion para cualquier tamaño s
typedef struct interpretacion{
	//guarda la sumatoria de el valor maximo-minimo+1 para sxs
	float y; 
	//guarda la relacion del tamaño de la muestra s/ancho de la imagen
	float x;
}interpretacion;



int **leerImagenBMP(char *nome,long &ancho,long &alto);
int **leerImagenMapa(char *nome,long &ancho,long &alto);
