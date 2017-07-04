#pragma once
#include "stdafx.h"
/**
@file 
@brief 
@author Emilio Gallardo Molina
@date 05/06/2017
*/
class shaderGPU : public shader
{

public:
	shaderGPU(void);
	~shaderGPU(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};