#pragma once
#include "stdafx.h"
class shaderCPU :public shader
{
private: 



public:
	shaderCPU(void);
	~shaderCPU(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
};

