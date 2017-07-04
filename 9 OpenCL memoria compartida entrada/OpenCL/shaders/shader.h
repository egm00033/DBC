#pragma once
#include "stdafx.h"
/**
@file shader.h
@brief declaración clase abstracta shader de la que heredaran todas las demás clases

@author Emilio Gallardo Molina
@date 15/05/2017
*/
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

	/**
	@brief constructor para las clases que hereden de ella

	@param plataforma utilizada (CPU o GPU)
	@param kernel utilizado
	*/
	shader(int plataforma, char * dirName);
	/**
	@brief libera memoria utilizada
	*/
	~shader(void);

public:

};

