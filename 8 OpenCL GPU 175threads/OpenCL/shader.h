#pragma once
#include "stdafx.h"
class shader
{
private: 

protected:
	cl_int ret;
	bool mostrarInfo;
	cl_command_queue command_queue;
	char *codigoShader;
	cl_platform_id *platforms;
	cl_kernel kernel;
	cl_program program;
	cl_context context;
	size_t source_size;
	cl_device_id devices[100];   
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	shader(int plataforma, char * dirName);
	~shader(void);

public:
	
};

