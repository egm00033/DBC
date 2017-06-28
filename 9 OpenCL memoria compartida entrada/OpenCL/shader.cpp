#include "stdafx.h"
#include "shader.h"

//creación del programa dependiento de nuestro HW
/**
*params plataforma=[0-2]
	- 0 = Intel 1.0
	- 1 = Intel 1.2
	- 2 = Nvidia
*/
shader::shader(int plataforma, char * dirName)
{
	//device_id = NULL;
	// Cargar codigo del codigoShader
	FILE *fp;
	//char * dirName;

		//dirName="kernel_profundidad_CPU.cl";

	fp = fopen(dirName, "r");
	if (!fp) {
		fprintf(stderr, "Fallo al cargar el kernel.\n");
		exit(1);
	}
	codigoShader = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( codigoShader, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
	if(mostrarDepuracion)printf("codigoShader cargado\n");
	// conseguir informacion de la plataforma y el dispositivo

	ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	if(mostrarDepuracion)printf("num plataformas=%i\n",ret_num_platforms);
	platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);


	/*ret = clGetDeviceIDs( platforms[2], CL_DEVICE_TYPE_ALL, 1, 
	&device_id, &ret_num_devices);
	printf("devices %i\n",platforms);*/


	//mostrar todos las gpu //0=intel-1.0; 1=intel 1.2; 2=cuda
	int idPlatforms;
	int idDispositivo;
	if(mostrarDepuracion){
		for (int idPlatforms=0; idPlatforms<ret_num_platforms; idPlatforms++)
		{
			//cl_device_id devices[100];
			clGetDeviceIDs(platforms[idPlatforms], CL_DEVICE_TYPE_ALL, 100, devices, &ret_num_devices);

			if(mostrarDepuracion)printf("=== %d OpenCL device(s) found on platform:%i\n", ret_num_devices,idPlatforms);
			for (idDispositivo=0; idDispositivo<ret_num_devices; idDispositivo++)
			{
				char buffer[10240];
				cl_uint buf_uint;
				cl_ulong buf_ulong;
				if(mostrarDepuracion)printf("  -- %d --\n", idDispositivo);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
				if(mostrarDepuracion)printf("  DEVICE_NAME = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL);
				if(mostrarDepuracion)printf("  DEVICE_VENDOR = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
				if(mostrarDepuracion)printf("  DEVICE_VERSION = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL);
				if(mostrarDepuracion)printf("  DRIVER_VERSION = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL);
				if(mostrarDepuracion)printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL);
				if(mostrarDepuracion)printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
				if(mostrarDepuracion)printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n", (unsigned long long)buf_ulong);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
				if(mostrarDepuracion)printf("  CL_DEVICE_LOCAL_MEM_SIZE = %llu\n\n", (unsigned long long)buf_ulong);
			}

		}
	}
	idPlatforms=1;
	idPlatforms=plataforma;
	idDispositivo=0;
	clGetDeviceIDs(platforms[idPlatforms], CL_DEVICE_TYPE_ALL, 100, devices, &ret_num_devices);

	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	// Creando el contexto de OpenCL
	context = clCreateContext( NULL, 1, &devices[idDispositivo], NULL, NULL, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	// Crear lista de operadiones para ejecutar
	//(3º argumento) por defecto en orden == 0: CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE ==1 || CL_QUEUE_PROFILING_ENABLE ==2
	command_queue = clCreateCommandQueue(context, devices[idDispositivo], 0, &ret);//tarda lo mismo en la practica
	//printf ("clCreateCommandQueue: CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE %i, CL_QUEUE_PROFILING_ENABLE %i;  ",CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,CL_QUEUE_PROFILING_ENABLE);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	// Crear programa del codigoShader
	program = clCreateProgramWithSource(context, 1, 
		(const char **)&codigoShader, (const size_t *)&source_size, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	// compilar programa
	ret = clBuildProgram(program, 1, &devices[idDispositivo], NULL, NULL, NULL);
	if(ret ==CL_SUCCESS ){
		//printf("respuesta bildProgram = CL_SUCCESS \n");
	}else{
		if(ret == CL_INVALID_PROGRAM ){
			printf("respuesta bildProgram = CL_INVALID_PROGRAM \n");
		}else if(ret == CL_INVALID_VALUE  ){
			printf("respuesta bildProgram = CL_INVALID_VALUE  \n");
		}else if(ret == CL_INVALID_DEVICE   ){
			printf("respuesta bildProgram = CL_INVALID_DEVICE   \n");
		}else if(ret ==CL_INVALID_BINARY  ){
			printf("respuesta bildProgram = CL_INVALID_BINARY  \n");
		}else if(ret == CL_INVALID_BUILD_OPTIONS    ){
			printf("respuesta bildProgram = CL_INVALID_BUILD_OPTIONS   \n");
		}else if(ret == CL_COMPILER_NOT_AVAILABLE    ){
			printf("respuesta bildProgram = CL_COMPILER_NOT_AVAILABLE    \n");
		}else if(ret ==CL_BUILD_PROGRAM_FAILURE  ){
			printf("respuesta bildProgram = CL_BUILD_PROGRAM_FAILURE  \n");
		}else if(ret == CL_INVALID_OPERATION   ){
			printf("respuesta bildProgram = CL_INVALID_OPERATION   \n");
		}else if(ret == CL_OUT_OF_HOST_MEMORY   ){
			printf("respuesta bildProgram = CL_OUT_OF_HOST_MEMORY   \n");
		}else{
			printf("respuesta bildProgram = desconocido \n");
		}
	}

	// Crear kernel de OpenCL
	kernel = clCreateKernel(program, "calcularNdesCPU", &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
}


shader::~shader(void)
{
	cl_int ret_aux;
	ret_aux = clFlush(command_queue);
	ret_aux = clFinish(command_queue);
	ret_aux = clReleaseKernel(kernel);
	ret_aux = clReleaseProgram(program);
	ret_aux = clReleaseCommandQueue(command_queue);
	ret_aux = clReleaseContext(context);
	free(codigoShader);
	free(platforms);
}
