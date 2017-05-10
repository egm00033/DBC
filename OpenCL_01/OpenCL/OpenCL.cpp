// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//

 

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("iniciando\n");

    // Vectores de entrada
    int i;
    const int LIST_SIZE = 16;// Lista de elementos de tama�o MxM
	size_t local_item_size = 4; // Grupo de trabajo de tama�o sxs
    int *A = (int*)malloc(sizeof(int)*LIST_SIZE);
    for(i = 0; i < LIST_SIZE; i++) {
        A[i] = i;
    }
 
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
	//por defecto en orden | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE (4� argumento)
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	//crear buffers de memoria en el dispositivo por cada vector
    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
            LIST_SIZE * sizeof(int), NULL, &ret);
    cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            LIST_SIZE * sizeof(int), NULL, &ret);
 
    // Copiar cada vector de entrada en su buffer
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
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

	printf("Despu�s de la construccion\n");
    // Crear kernel de OpenCL
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
	printf("respuesta de la linea %d es %d\n", __LINE__, ret);
 
    // Establecer argumentos
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	printf("respuesta de la linea %d es %d\n", __LINE__, ret);

    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	printf("respuesta de la linea %d es %d\n", __LINE__, ret);

    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
	printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	//a�adir para arreglar los problemas de salida
	//ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);
 
	printf("Antes de la ejecucion\n");
    // ejecutar el kernel OpenCL en la lista
    size_t global_item_size = LIST_SIZE; // numero total de operaciones (tama�o del vector)
    
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
 printf("Despu�s de la ejecucion\n");
    // Copiar el buffer C en el vector C
 int *B = (int*)malloc(sizeof(int)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(int), B, 0, NULL, NULL);
    int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
    ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
            LIST_SIZE * sizeof(int), C, 0, NULL, NULL);
 printf("Antes de copiar\n");
    //Mostrar solucion
    for(i = 0; i < LIST_SIZE; i++)
        printf("%d + %d = %d\n", A[i], B[i], C[i]);
 printf("Mostrado\n");
    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(A);
    free(B);
    free(C);

	system("pause");
    return 0;
}

