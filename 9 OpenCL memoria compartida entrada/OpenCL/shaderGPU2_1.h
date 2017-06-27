#pragma once
#include "stdafx.h"
class shaderGPU2_1 : public shader//mejora shaderGPU con memoria compartida
{
private: 

public:
	shaderGPU2_1(void);
	~shaderGPU2_1(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};