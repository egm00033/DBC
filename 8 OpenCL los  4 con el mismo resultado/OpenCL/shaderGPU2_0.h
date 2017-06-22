#pragma once
#include "stdafx.h"
class shaderGPU2_0 : public shader//mejora shaderGPU utilizando la memoria compartida
{
private: 

public:
	shaderGPU2_0(void);
	~shaderGPU2_0(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};