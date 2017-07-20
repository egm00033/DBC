#pragma once
#include "stdafx.h"
/**
@file shaderGPU2_0_0.h
@brief shader con 2Dimensiones
@author Emilio Gallardo Molina
@date 20/06/2017
*/
class shaderGPU2_0_0 : public shader//mejora shaderGPU subdividiendo el trabajo en grupos de 7 threads utilizando 2 dimensiones
{
private: 

public:
	/**
	@brief constructor para las clases que hereda de la clase shader

	@param plataforma utilizada (CPU o GPU)
	@param kernel utilizado
	*/
	shaderGPU2_0_0(void);
	/**
	@brief libera memoria utilizada
	*/
	~shaderGPU2_0_0(void);
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