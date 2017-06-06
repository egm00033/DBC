#pragma once
#include "stdafx.h"
class shader
{
private:
	cl_int ret;
	bool mostrarInfo;
	char *codigoShader;
	size_t source_size;
	cl_device_id devices[100];   
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_program program;
	cl_context context;
	cl_platform_id *platforms;
	cl_command_queue command_queue;
	cl_kernel kernel;
	float subdividirCalculos(const int *vEntrada,const int s,const int M,const int inicio,const int tamanioLista,const float sPrima);
public:
	shader(void);
	~shader(void);
	float CalcularN(int *vEntrada,int M,int s);
	int getRet(){return ret;};
};

