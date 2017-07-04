#pragma once
#include "stdafx.h"
/**
@file shaderCPU_superficie.h
@brief shader con 4 hilos en la CPU que cada uno calcula el valor de N para un cuarto de la imagen 
	en un bucle que recorre todos los tamaños de s.
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

