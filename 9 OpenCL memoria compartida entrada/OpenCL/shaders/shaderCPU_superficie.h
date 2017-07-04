#pragma once
#include "stdafx.h"
/**
@file 
@brief 
@author Emilio Gallardo Molina
@date 20/06/2017
*/
class shaderCPU_superficie : public shader
{
private:

public:
	shaderCPU_superficie(void);
	~shaderCPU_superficie(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};

