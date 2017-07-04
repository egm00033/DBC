#pragma once
#include "stdafx.h"
/**
@file 
@brief 
@author Emilio Gallardo Molina
@date 06/06/2017
*/
class shaderCPU :public shader
{
private: 



public:
	shaderCPU(void);
	~shaderCPU(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
};

