#pragma once
#include "stdafx.h"
/**
@file shaderGPU2_0_1.h
@brief shader con 2Dimensiones y memoria compartida en la lectura
@author Emilio Gallardo Molina
@date 20/06/2017
*/
class shaderGPU2_0_1 : public shader//mejora shaderGPU con memoria compartida
{
private: 

public:
	shaderGPU2_0_1(void);
	~shaderGPU2_0_1(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};