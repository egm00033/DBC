#pragma once
#include "stdafx.h"
class shaderGPU2_0_0 : public shader//mejora shaderGPU subdividiendo el trabajo en grupos de 7 threads utilizando 2 dimensiones
{
private: 

public:
	shaderGPU2_0_0(void);
	~shaderGPU2_0_0(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};