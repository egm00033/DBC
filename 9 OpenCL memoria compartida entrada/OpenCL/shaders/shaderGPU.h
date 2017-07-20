#pragma once
#include "stdafx.h"
/**
@file shaderGPU.h
@brief shader sin optimización, un hilo por cada tamaño de s
@author Emilio Gallardo Molina
@date 05/06/2017
*/
class shaderGPU : public shader
{

public:
	/**
	@brief constructor para las clases que hereda de la clase shader

	@param plataforma utilizada (CPU o GPU)
	@param kernel utilizado
	*/
	shaderGPU(void);
	/**
	@brief libera memoria utilizada
	*/
	~shaderGPU(void);
		/**
	@brief calcula el numero de cajas en una imagen para cada tamaño de ventana

	@param img3 array con el valor de cada pixel de una imagen de entrada
	@param NdeS array que almacena el resultado de la dimensión fractal de la imagen para cada tamaño
	@param M ancho de la imagen cuadrada
	@param tamListaS número de ventanas
	@param listaS array con cada tamaño de ventana


	*/
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	/**
	@brief Devuelve el estado de la ejecución: 0=satisfactoria, 0<>error en la ejecución

	@returns array con el valor de cada pixel
	*/
	int getRet(){return ret;};
};