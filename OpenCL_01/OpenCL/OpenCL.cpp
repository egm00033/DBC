// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//



#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int M = 8;
	const int LIST_SIZE = M*M;// Lista de elementos de tamaño M
	// Vectores de entrada		
	int *imagen = (int*)malloc(sizeof(int)*LIST_SIZE);

	for(int i = 0; i < LIST_SIZE; i++) {
		imagen[i] = i;
	}
	double *sumn = (double*)malloc(sizeof(double)*LIST_SIZE);
	double *N = (double*)malloc(sizeof(double)*LIST_SIZE);

	for (int s = 2; s <= M/2 ; s*=2)
	{
		size_t local_item_size = s*s; // Grupo de trabajo de tamaño sxs


		printf("iniciando\n");



		// Cargar codigo del shader
		FILE *fp;
		char *shader;
		size_t source_size;

		fp = fopen("kernel.cl", "r");
		if (!fp) {
			fprintf(stderr, "Fallo al cargar el kernel.\n");
			exit(1);
		}
		shader = (char*)malloc(MAX_SOURCE_SIZE);
		source_size = fread( shader, 1, MAX_SOURCE_SIZE, fp);
		fclose( fp );
		printf("shader cargado\n");
		// conseguir informacion de la plataforma y el dispositivo
		cl_device_id device_id = NULL;   
		cl_uint ret_num_devices;
		cl_uint ret_num_platforms;


		cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
		cl_platform_id *platforms = NULL;
		platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

		ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clGetDeviceIDs( platforms[1], CL_DEVICE_TYPE_ALL, 1, 
			&device_id, &ret_num_devices);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);
		// Creando el contexto de OpenCL
		cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		// Crear lista de operadiones para ejecutar
		//por defecto en orden | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE (4º argumento)
		cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		//crear buffers de memoria en el dispositivo por cada vector
		cl_mem imagen_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			LIST_SIZE * sizeof(int), NULL, &ret);
		cl_mem max_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
			LIST_SIZE * sizeof(int), NULL, &ret);
		cl_mem min_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
			LIST_SIZE * sizeof(int), NULL, &ret);
		cl_mem n_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
			LIST_SIZE * sizeof(double), NULL, &ret);

		// Copiar cada vector de entrada en su buffer
		ret = clEnqueueWriteBuffer(command_queue, imagen_mem_obj, CL_TRUE, 0,
			LIST_SIZE * sizeof(int), imagen, 0, NULL, NULL);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);



		printf("antes de la construccion\n");
		// Crear programa del shader
		cl_program program = clCreateProgramWithSource(context, 1, 
			(const char **)&shader, (const size_t *)&source_size, &ret);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

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

		printf("Después de la construccion\n");
		// Crear kernel de OpenCL
		cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		// Establecer argumentos
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imagen_mem_obj);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&max_mem_obj);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&min_mem_obj);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);
		ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&n_mem_obj);
		printf("respuesta de la linea %d es %d\n", __LINE__, ret);

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
		ret = clEnqueueReadBuffer(command_queue, max_mem_obj, CL_TRUE, 0, 
			LIST_SIZE * sizeof(int), maximos, 0, NULL, NULL);
		int *minimos = (int*)malloc(sizeof(int)*LIST_SIZE);
		ret = clEnqueueReadBuffer(command_queue, min_mem_obj, CL_TRUE, 0, 
			LIST_SIZE * sizeof(int), minimos, 0, NULL, NULL);
		ret = clEnqueueReadBuffer(command_queue, n_mem_obj, CL_TRUE, 0, 
			LIST_SIZE * sizeof(double), sumn, 0, NULL, NULL);

		printf("Antes de copiar\n");
		//Mostrar solucion

		for(int i = 0; i < global_item_size; i++)//sustituir por global_item_size
			printf("%d. max = %d. min = %d. n= %f\n", imagen[i], maximos[i], minimos[i], sumn[i]);
		printf("Mostrado\n");
		// Clean up
		ret = clFlush(command_queue);
		ret = clFinish(command_queue);
		ret = clReleaseKernel(kernel);
		ret = clReleaseProgram(program);
		ret = clReleaseMemObject(imagen_mem_obj);
		ret = clReleaseMemObject(max_mem_obj);
		ret = clReleaseMemObject(min_mem_obj);
		ret = clReleaseMemObject(n_mem_obj);
		ret = clReleaseCommandQueue(command_queue);
		ret = clReleaseContext(context);
		free(maximos);
		free(minimos);

		printf("M=%i s=%i \n",M,s);
		system("pause");
	}
	free(imagen);
	free(sumn);
	free(N);
	return 0;
}

