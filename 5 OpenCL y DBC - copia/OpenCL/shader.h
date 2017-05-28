#pragma once
#include "stdafx.h"
class shader
{
private:
	bool mostrarInfo;
	char *codigoShader;
	size_t source_size;
	cl_device_id device_id;   
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_program program;
	cl_context context;
	cl_platform_id *platforms;
	cl_command_queue command_queue;
	cl_kernel kernel;
	float subdividirCalculos(int *vEntrada,int s,int M,int inicio,int tamanioLista);
public:
	shader(void);
	~shader(void);
	float getDF(int *vEntrada,int M,int s);
};

