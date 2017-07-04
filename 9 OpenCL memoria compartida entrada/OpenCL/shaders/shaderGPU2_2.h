#pragma once
#include "stdafx.h"
/**
@file 
@brief 
@author Emilio Gallardo Molina
@date 27/06/2017
*/
class shaderGPU2_2 : public shader//mejora shaderGPU con memoria compartida
{
private: 

public:
	shaderGPU2_2(void);
	~shaderGPU2_2(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};