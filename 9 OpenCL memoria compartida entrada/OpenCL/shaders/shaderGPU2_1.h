#pragma once
#include "stdafx.h"
/**
@file 
@brief 
@author Emilio Gallardo Molina
@date 23/06/2017
*/
class shaderGPU2_1 : public shader//mejora shaderGPU con 3D
{
private: 

public:
	shaderGPU2_1(void);
	~shaderGPU2_1(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};