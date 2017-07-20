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
	cl_int ret;//Almacena el estado de la ejecución
	bool mostrarInfo;//permite depurar la ejecución
	cl_command_queue command_queue; //lista de operaciones a ejecutar en el dispositivo
	char *codigoShader;//Codigo del kernel a ejecutar
	cl_platform_id *platforms; //array con las plataformas disponibles Ej: OpenCL1_0, OpenCL1_2, CUDA,... y cada una con los dispositivos disponibles
	cl_kernel kernel;//Asigna el código compilado a un dispositivo
	cl_program program;//Carga el código fuente 
	cl_context context;//Selecciona el dispositivo en el que se ejecutarán las instrucciones
	size_t source_size;
	cl_device_id devices[100];   //Almacena todos los dispositivos que se pueden utilizar para ejecutar el código
	cl_uint ret_num_devices; //Numero de dispositivos
	cl_uint ret_num_platforms; //Numero de plataformas

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

