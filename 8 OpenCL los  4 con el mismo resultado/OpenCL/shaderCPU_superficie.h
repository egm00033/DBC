#pragma once
#include "stdafx.h"
class shaderCPU_superficie : public shader
{
private:

public:
	shaderCPU_superficie(void);
	~shaderCPU_superficie(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS,int particiones);
	int getRet(){return ret;};
};

