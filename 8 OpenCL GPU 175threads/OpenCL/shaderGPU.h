#pragma once
#include "stdafx.h"
class shaderGPU : public shader
{

public:
	shaderGPU(void);
	~shaderGPU(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};