#pragma once
#include "stdafx.h"
/**
@file shader.h
@brief declaraci�n clase abstracta shader de la que heredaran todas las dem�s clases

@author Emilio Gallardo Molina
@date 15/05/2017
*/
class shader
{
private: 

protected:
	cl_int ret;//Almacena el estado de la ejecuci�n
	bool mostrarInfo;//permite depurar la ejecuci�n
	cl_command_queue command_queue; //lista de operaciones a ejecutar en el dispositivo
	char *codigoShader;//Codigo del kernel a ejecutar
	cl_platform_id *platforms; //array con las plataformas disponibles Ej: OpenCL1_0, OpenCL1_2, CUDA,... y cada una con los dispositivos disponibles
	cl_kernel kernel;//Asigna el c�digo compilado a un dispositivo
	cl_program program;//Carga el c�digo fuente 
	cl_context context;//Selecciona el dispositivo en el que se ejecutar�n las instrucciones
	size_t source_size;
	cl_device_id devices[100];   //Almacena todos los dispositivos que se pueden utilizar para ejecutar el c�digo
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

