#include "stdafx.h"
#include "shaderCPU_superficie.h"

//creación del programa dependiento de nuestro HW
shaderCPU_superficie::shaderCPU_superficie(void)
{
	//device_id = NULL;
	// Cargar codigo del codigoShader
	FILE *fp;
	char * dirName;

		dirName="kernel_superficie_CPU.cl";

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


	//mostrar todos las gpu
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
				if(mostrarDepuracion)printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n\n", (unsigned long long)buf_ulong);
			}

		}
	}
	idPlatforms=1;
	//idPlatforms=plataforma;
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

//Calcula la sumatoria de n para un tamaño s dado
void shaderCPU_superficie::CalcularN(unsigned char *img3,float *NdeS, int M, int tamListaS,int *listaS){
	//éxito en la creación del programa
	if(ret==0){
		cl_int ret;
		size_t global_item_size = 4; // subdivisiones de cada superficie
		size_t local_item_size = 1; // Grupo de trabajo

		const int LIST_SIZE =M*M;// Lista de elementos de tamaño MxM
		const int tamS=tamListaS;

		int tamSalida=tamListaS*global_item_size;
		float *vSalida=(float*) calloc(tamSalida,sizeof(float));

		for(int i=0;i<tamSalida;i++){
				vSalida[i]=(-1)*i;
			}

		if(true)printf("global size =%i\t localsize= %i\n",global_item_size,local_item_size);

		//crear buffers de memoria en el dispositivo por cada vector
		cl_mem entrada_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			LIST_SIZE * sizeof(char), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		cl_mem salida_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
			tamSalida * sizeof(float), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		cl_mem listaS_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			tamS * sizeof(int), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);


		// Copiar cada vector de entrada en su buffer
		ret = clEnqueueWriteBuffer(command_queue, entrada_mem_obj, CL_TRUE, 0,
			LIST_SIZE * sizeof(char), img3, 0, NULL, NULL);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clEnqueueWriteBuffer(command_queue, listaS_mem_obj, CL_TRUE, 0,
			tamS * sizeof(int), listaS, 0, NULL, NULL);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clEnqueueWriteBuffer(command_queue, salida_mem_obj, CL_TRUE, 0,
			tamSalida * sizeof(float), vSalida, 0, NULL, NULL);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		// Establecer argumentos de entrada
		//variables
		ret=clSetKernelArg(kernel, 2, sizeof(M), &M);//set parametro del kernel
		if(ret!=0)printf("clSetKernelArg(ancho), linea: %d, error: %d\n", __LINE__, ret);

		ret=clSetKernelArg(kernel, 4, sizeof(tamS), &tamS);//profundidad de la piramide
		if(ret!=0)printf("clSetKernelArg(profundidad), linea: %d, error: %d\n", __LINE__, ret);

		//arrays
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&entrada_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&salida_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&listaS_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);


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
				printf("(ERROR -54)clEnqueueNDRangeKernel=CL_INVALID_WORK_GROUP_SIZE\n");
				printf("local_item_size(%i) < CL_DEVICE_MAX_WORK_GROUP_SIZE(%i)\n",local_item_size,CL_DEVICE_MAX_WORK_GROUP_SIZE);
				printf("global_work_size(%i) < CL_DEVICE_ADDRESS_BITS(%i)\n",global_item_size,CL_DEVICE_ADDRESS_BITS);
			}else if(ret!=0)printf("clEnqueueNDRangeKernel=%i\n",ret,ret);

			// Copiar el buffer salida en el vector NdeS
			printf("copiando vSalida\n");
			for(int i=0;i<tamSalida;i++){
				vSalida[i]=tamSalida-i;
			}

			ret = clEnqueueReadBuffer(command_queue, salida_mem_obj, CL_TRUE, 0, tamSalida, vSalida, 0, NULL, NULL);

			printf("salida copiado\n");
			for(int i=0;i<tamSalida;i++){
				printf("%i = %f\n",i,vSalida[i]);
			}

			//copiar de salida a nds sumando el valor de cada grupo

		}



		ret = clReleaseMemObject(entrada_mem_obj);
		ret = clReleaseMemObject(salida_mem_obj);
		free(vSalida);
	}else{
		printf("ERROR ret==0 en CalcularN\n");
		system("pause");
	}
	
}

shaderCPU_superficie::~shaderCPU_superficie(void)
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
