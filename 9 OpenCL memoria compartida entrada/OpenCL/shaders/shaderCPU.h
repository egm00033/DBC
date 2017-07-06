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
	shaderCPU(void);
	~shaderCPU(void);
	/**
	@brief 

	@param 
	@param
	@param
	@param
	@param

	@return 
	*/
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
};

