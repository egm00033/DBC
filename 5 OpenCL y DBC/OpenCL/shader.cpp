#include "stdafx.h"
#include "shader.h"


shader::shader(void)
{
	cl_int ret;
	device_id = NULL;
	// Cargar codigo del codigoShader
	FILE *fp;
	fp = fopen("kernel.cl", "r");
	if (!fp) {
		fprintf(stderr, "Fallo al cargar el kernel.\n");
		exit(1);
	}
	codigoShader = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( codigoShader, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );
	printf("codigoShader cargado\n");
	// conseguir informacion de la plataforma y el dispositivo

	ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clGetDeviceIDs( platforms[1], CL_DEVICE_TYPE_ALL, 1, 
		&device_id, &ret_num_devices);
	printf("devices %i\n",platforms);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	// Creando el contexto de OpenCL
	context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	
	// Crear lista de operadiones para ejecutar
	//por defecto en orden | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE (4º argumento)
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	
	printf("antes de la construccion\n");
	// Crear programa del codigoShader
	program = clCreateProgramWithSource(context, 1, 
		(const char **)&codigoShader, (const size_t *)&source_size, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	// compilar programa
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(ret ==CL_SUCCESS ){
		printf("respuesta bildProgram = CL_SUCCESS \n");
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
	kernel = clCreateKernel(program, "vector_add", &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
}
float shader::getDF(int **entradaOpencl, int M,int s){
	cl_int ret;
	printf("iniciando\n");

	// Vectores de entrada
	int i;
	

	const int LIST_SIZE = M*M;// Lista de elementos de tamaño MxM
	size_t local_item_size = s*s; // Grupo de trabajo de tamaño sxs
	int *imagen = (int*)malloc(sizeof(int)*LIST_SIZE);

	for(i = 0; i < LIST_SIZE; i++) {
		imagen[i] = entradaOpencl[s-2][i];
		if(i%local_item_size==0)
			printf("\n");
		printf("col %i = %i\t",i%s,imagen[i]);
		
	}



	//crear buffers de memoria en el dispositivo por cada vector
	cl_mem imagen_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
		LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem max_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
		LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem min_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
		LIST_SIZE * sizeof(int), NULL, &ret);

	// Copiar cada vector de entrada en su buffer
	ret = clEnqueueWriteBuffer(command_queue, imagen_mem_obj, CL_TRUE, 0,
		LIST_SIZE * sizeof(int), imagen, 0, NULL, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);




	// Establecer argumentos
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imagen_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&max_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&min_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	//añadir para arreglar los problemas de salida
	//ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);

	printf("Antes de la ejecucion\n");
	// ejecutar el kernel OpenCL en la lista
	size_t global_item_size = LIST_SIZE/local_item_size; // numero total de operaciones (tamaño del vector)

	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
		&global_item_size, &local_item_size, 0, NULL, NULL);
	printf("Después de la ejecucion\n");
	// Copiar el buffer minimos en el vector minimos
	int *maximos = (int*)malloc(sizeof(int)*LIST_SIZE);
	ret = clEnqueueReadBuffer(command_queue, max_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), maximos, 0, NULL, NULL);
	int *minimos = (int*)malloc(sizeof(int)*LIST_SIZE);
	ret = clEnqueueReadBuffer(command_queue, min_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), minimos, 0, NULL, NULL);


	printf("tam=%i\n",global_item_size);

	//Mostrar solucion
	for(i = 0; i < global_item_size; i++)//sustituir por global_item_size
		printf("%d. max = %d. min = %d\n", imagen[i], maximos[i], minimos[i]);
	printf("tam=%i\n",global_item_size);
	// Clean up
	
	ret = clReleaseMemObject(imagen_mem_obj);
	ret = clReleaseMemObject(max_mem_obj);
	ret = clReleaseMemObject(min_mem_obj);
	
	free(imagen);
	free(maximos);
	free(minimos);

	return 0;
}

shader::~shader(void)
{
	cl_int ret;
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(codigoShader);
	free(platforms);
}
