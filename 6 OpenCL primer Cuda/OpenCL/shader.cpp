#include "stdafx.h"
#include "shader.h"

//creación del programa dependiento de nuestro HW
shader::shader(void)
{
	//device_id = NULL;
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
	printf("num plataformas=%i\n",ret_num_platforms);
	platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);


	/*ret = clGetDeviceIDs( platforms[2], CL_DEVICE_TYPE_ALL, 1, 
	&device_id, &ret_num_devices);
	printf("devices %i\n",platforms);*/


	//mostrar todos las gpu
	int idPlatforms;
	int idDispositivo;
	if(mostrarDepuracion){
		for (int idPlatforms=0; idPlatforms<ret_num_platforms; idPlatforms++)
		{
			//cl_device_id devices[100];
			clGetDeviceIDs(platforms[idPlatforms], CL_DEVICE_TYPE_ALL, 100, devices, &ret_num_devices);

			printf("=== %d OpenCL device(s) found on platform:%i\n", ret_num_devices,idPlatforms);
			for (idDispositivo=0; idDispositivo<ret_num_devices; idDispositivo++)
			{
				char buffer[10240];
				cl_uint buf_uint;
				cl_ulong buf_ulong;
				printf("  -- %d --\n", idDispositivo);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
				printf("  DEVICE_NAME = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL);
				printf("  DEVICE_VENDOR = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
				printf("  DEVICE_VERSION = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL);
				printf("  DRIVER_VERSION = %s\n", buffer);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL);
				printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL);
				printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
				clGetDeviceInfo(devices[idDispositivo], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL);
				printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n\n", (unsigned long long)buf_ulong);
			}

		}
	}
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
	kernel = clCreateKernel(program, "vector_add", &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
}

//permite subdividir la lista de calculos
float shader::subdividirCalculos(const int *vEntrada,const int s,const int M,const int inicio,const int tamanioLista,const float sPrima){
	float sumatoria=0;
	cl_int ret;
	int s2=s*s;
	const int LIST_SIZE =tamanioLista;// Lista de elementos de tamaño MxM
	size_t global_item_size = LIST_SIZE/s2; // numero total de operaciones (tamaño del vector)
	size_t local_item_size = 1; // Grupo de trabajo de tamaño sxs


	if(sqrt((double)global_item_size)-(int)sqrt((double)global_item_size)==0){
		if(mostrarDepuracion)printf("local_item_size=sqrt(global_item_size)\n");
		local_item_size=(int)sqrt((double)global_item_size);
	}else if(global_item_size%9==0){
		if(mostrarDepuracion)printf("local_item_size=global_item_size/9\n");
		local_item_size=global_item_size/9;
	}else if(global_item_size%4==0){
		if(mostrarDepuracion)printf("local_item_size=global_item_size/4\n");
		local_item_size=global_item_size/4;
	}
	


	if(mostrarDepuracion)printf("sPrima = %f / (%f / %f) = %f\n",(float)256,(float)M,(float)s,sPrima);
	if(mostrarDepuracion)printf("global size =%i\t localsize= %i\n",global_item_size,local_item_size);

	//crear buffers de memoria en el dispositivo por cada vector
	cl_mem imagen_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
		LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem max_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
		LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem min_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
		LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem n_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
		LIST_SIZE * sizeof(float), NULL, &ret);


	/*cl_int clSetKernelArg (	
	cl_kernel kernel,
	cl_uint arg_index,
	size_t arg_size,
	const void *arg_value)*/
	ret=clSetKernelArg(kernel, 4, sizeof(s2), &s2);
	if(ret!=0)printf("clSetKernelArg(s2), linea: %d, error: %d\n", __LINE__, ret);

	ret=clSetKernelArg(kernel, 5, sizeof(s2), &sPrima);
	if(ret!=0)printf("clSetKernelArg(s2), linea: %d, error: %d\n", __LINE__, ret);

	// Copiar cada vector de entrada en su buffer
	ret = clEnqueueWriteBuffer(command_queue, imagen_mem_obj, CL_TRUE, 0,
		LIST_SIZE * sizeof(int), vEntrada, 0, NULL, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);


	// Establecer argumentos
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imagen_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&max_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&min_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&n_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	//añadir para arreglar los problemas de salida
	//ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);


	//limite hardware
	if(global_item_size>=CL_DEVICE_ADDRESS_BITS){
		printf("Excedido el numero de global_work_size(%i), debe ser menor que CL_DEVICE_ADDRESS_BITS(%i)\n",global_item_size,CL_DEVICE_ADDRESS_BITS);
	}else if(local_item_size>=CL_DEVICE_MAX_WORK_GROUP_SIZE){
		printf("Excedido el numero de local_item_size(%i), debe ser menor que CL_DEVICE_MAX_WORK_GROUP_SIZE(%i)\n",local_item_size,CL_DEVICE_MAX_WORK_GROUP_SIZE);
		//}else if(LIST_SIZE>=CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE){
		//printf("limite buffer = %i\n",CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE );
	}else{
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
			&global_item_size, &local_item_size, 0, NULL, NULL);

		if(ret==-54){
			printf("s=%i (ERROR -54)clEnqueueNDRangeKernel=CL_INVALID_WORK_GROUP_SIZE\n",s);
			printf("local_item_size(%i) < CL_DEVICE_MAX_WORK_GROUP_SIZE(%i)\n",local_item_size,CL_DEVICE_MAX_WORK_GROUP_SIZE);
			printf("global_work_size(%i) < CL_DEVICE_ADDRESS_BITS(%i)\n",global_item_size,CL_DEVICE_ADDRESS_BITS);
		}else if(ret!=0)printf("clEnqueueNDRangeKernel=%i\n",ret,ret);
		// Copiar el buffer minimos en el vector minimos

		int *maximos = (int*)malloc(sizeof(int)*LIST_SIZE);
		ret = clEnqueueReadBuffer(command_queue, max_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), maximos, 0, NULL, NULL);
		if(ret!=0)printf("copiar max=%i\n",ret);
		int *minimos = (int*)malloc(sizeof(int)*LIST_SIZE);
		ret = clEnqueueReadBuffer(command_queue, min_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), minimos, 0, NULL, NULL);
		float *n = (float*)malloc(sizeof(float)*LIST_SIZE);
		ret = clEnqueueReadBuffer(command_queue, n_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(float), n, 0, NULL, NULL);
		//Mostrar solucion
		if(mostrarMinMax)printf("\nCalculos de min y max resumidos para s=%i\n",s);
		for(int i = 0; i < global_item_size; i++){//sustituir por global_item_size
			sumatoria+=n[i];
			if(mostrarMinMax){
				if(i<3||i>global_item_size-4)
					printf("%d. max = %d. min = %d. n = %f\n", i, maximos[i], minimos[i],n[i]);
			}
		}

		//mostrar mapa de profundidades si se calcula en una llamada a la funcion
		if(mostrarMapaZ){
			if(M==640&&tamanioLista==M*M){
				crearMapaZ(s, sPrima,  M, n);
			}
		}


		// Clean up
		free(maximos);
		free(minimos);
		free(n);
	}
	ret = clReleaseMemObject(imagen_mem_obj);
	ret = clReleaseMemObject(max_mem_obj);
	ret = clReleaseMemObject(min_mem_obj);
	ret = clReleaseMemObject(n_mem_obj);
	//printf("sumatoria = %f \n ",sumatoria);

	return sumatoria;
}

//Calcula la sumatoria de n para un tamaño s dado
float shader::CalcularN(int *vEntrada, int M,int s){
	float N=0;
	if(ret==0){
		float sPrima=(float)256/((float)M/(float)(s));//con nivel de gris =950 da un resultado optimo
		//float sPrima=(float)256/((float)s/(float)(M));
		M=M/s*s;//se ajusta la lista de elementos para evitar desbordarse

		//if(mostrarDepuracion)
		printf("\niniciando s=%i M=%i\n",s,M);
		int tam_lista=M*M;
		int s2=s*s;
		int nGrid=M/s;
		int particiones=1;


		//calcular el num particiones; comprueba si se excede el tamaño de global
		if(mostrarDepuracion)printf("lista de operaciones=%i : limite del dispositivo=%i\n",tam_lista,CL_DEVICE_ADDRESS_BITS);
		while((tam_lista/particiones)/s2>CL_DEVICE_ADDRESS_BITS){
			particiones+=1;
			while(nGrid%particiones!=0)
				particiones+=1;
		}
		//si no tiene espacion nuestro dispositivo para ejecutar lodos los global_group a la vez
		if(mostrarDepuracion)printf("Numero de particiones para s=%i : particion=%i\n",s,particiones);
		if(particiones==1){
			N+=subdividirCalculos(vEntrada, s,M,0,tam_lista,sPrima);
		}else{
			//subdividir workgroup
			int tamSubLista=tam_lista/particiones;
			int *subLista = (int *) malloc(tamSubLista * sizeof(int));

			//for
			for (int p = 0; p < particiones; p++)
			{
				int inicio=p*tamSubLista;
				if(mostrarDepuracion)printf("Iniciando s=%i particion=%i\n",s,p);
				memcpy(subLista, vEntrada + inicio,tamSubLista*sizeof(int));

				N+=subdividirCalculos(subLista, s,M,inicio,tamSubLista,sPrima);
			}

			//fin for
			free(subLista);
		}
	}else{
		printf("ERROR ret==0 en s==%i\n",s);
		system("pause");
	}
	return N;
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
