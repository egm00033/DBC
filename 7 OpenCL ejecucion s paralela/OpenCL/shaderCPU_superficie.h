#pragma once
#include "stdafx.h"
class shaderCPU_superficie
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
public:
	shaderCPU_superficie(void);
	~shaderCPU_superficie(void);
	void CalcularN(unsigned char *img3,float *NdeS,int M, int tamListaS,int *listaS);
	int getRet(){return ret;};
};

