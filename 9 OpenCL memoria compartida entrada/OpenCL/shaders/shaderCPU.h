#pragma once
#include "stdafx.h"
/**
@file shaderCPU.h
@brief shader que ejecuta un hilo por cada s en la CPU
@author Emilio Gallardo Molina
@date 06/06/2017
*/
class shaderCPU :public shader
{
private: 

public:
	/**
	@brief constructor para las clases que hereda de la clase shader

	@param plataforma utilizada (CPU o GPU)
	@param kernel utilizado
	*/
	shaderCPU(void);
	/**
	@brief libera memoria utilizada
	*/
	~shaderCPU(void);
	/**
	@brief calcula el numero de cajas en una imagen para cada tama�o de ventana

	@param img3 array con el valor de cada pixel de una imagen de entrada
	@param NdeS array que almacena el resultado de la dimensi�n fractal de la imagen para cada tama�o
	@param M ancho de la imagen cuadrada
	@param tamListaS n�mero de ventanas
	@param listaS array con cada tama�o de ventana


	*/
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);

};

