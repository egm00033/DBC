// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.
//



#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int M = 16;
	int s = 2;
	int LIST_SIZE = M*M;// Lista de elementos de tamaño M
	// Vectores de entrada	
	int *entrada = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *imagenM = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *imagenm = (int*)malloc(sizeof(int)*LIST_SIZE);
	double *sumn = (double*)malloc(sizeof(double)*LIST_SIZE);
	int *maximos = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *minimos = (int*)malloc(sizeof(int)*LIST_SIZE);

	for(int i = 0; i < LIST_SIZE; i++) {
		entrada[i] = i;
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



	//bucle para calcular todos los N
	while(s<=M/2){
		system("cls");
		if(s>M/2){
			printf("FIN 1\n");
			system("pause");
			break;
		}
		while(M%s!=0){
			//system("cls");
			printf("no valido s=%i (resto = %i) \n",s,M%s);
			s+=1;
		}	

		if(s>M/2){
			printf("FIN 2\n");
			system("pause");
			break;
		}

		printf("M=%i s= %i \n",M,s);
		bool swap=false;
		int subS=2;
		int subM=M;
		int subListTam=LIST_SIZE;

		//copiar datos de entrada
		for(int i = 0; i < M*M; i++) {
			imagenM[i] = entrada[i];
			imagenm[i] = entrada[i];
			sumn[i]=0;
		}

		do{//bucle iterativo para un S
			if(swap){
				printf("bucle \n");
				for(int i = 0; i < LIST_SIZE; i++) {
					imagenM[i] = maximos[i];
					imagenm[i] = minimos[i];
					//sumn[i]=0;
				}
				
				while(subM%subS!=0&&subS<=subM/2){
					printf("M=%i s= %i => %i\n",subM,subS,subS+1);
					subS+=1;

				}
				if(subS>subM/2){
					printf("analizar\n");
					swap=false;
					break;
				}
				//subM=subM/2;
				
			}

			swap=true;
			printf("iniciando bucle M=%i s= %i\n",subM,subS);
			size_t local_item_size = subS*subS; // Grupo de trabajo de tamaño sxs

			size_t global_item_size = subListTam/local_item_size; 

			printf("subListTam %i, global_item_size %i, local_item_size %i\n",subListTam,global_item_size,local_item_size);

			// Creando el contexto de OpenCL
			cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);

			// Crear lista de operadiones para ejecutar
			//por defecto en orden | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE (4º argumento)
			cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);

			//crear buffers de memoria en el dispositivo por cada vector
			cl_mem imagen_mem_objM = clCreateBuffer(context, CL_MEM_READ_ONLY, 
				LIST_SIZE * sizeof(int), NULL, &ret);
			cl_mem imagen_mem_objm = clCreateBuffer(context, CL_MEM_READ_ONLY, 
				LIST_SIZE * sizeof(int), NULL, &ret);
			cl_mem max_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
				LIST_SIZE * sizeof(int), NULL, &ret);
			cl_mem min_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
				LIST_SIZE * sizeof(int), NULL, &ret);
			cl_mem n_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
				LIST_SIZE * sizeof(double), NULL, &ret);

			// Copiar cada vector de entrada en su buffer
			ret = clEnqueueWriteBuffer(command_queue, imagen_mem_objM, CL_TRUE, 0,
				LIST_SIZE * sizeof(int), imagenM, 0, NULL, NULL);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);
			ret = clEnqueueWriteBuffer(command_queue, imagen_mem_objm, CL_TRUE, 0,
				LIST_SIZE * sizeof(int), imagenm, 0, NULL, NULL);
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
			ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imagen_mem_objM);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);
			ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&imagen_mem_objm);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);

			ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&max_mem_obj);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);

			ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&min_mem_obj);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);
			ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&n_mem_obj);
			printf("respuesta de la linea %d es %d\n", __LINE__, ret);

			//añadir para arreglar los problemas de salida
			//ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);


			if(global_item_size>=CL_DEVICE_ADDRESS_BITS){
				printf("Excedido el numero de global_work_size, debe ser menor que CL_DEVICE_ADDRESS_BITS\n");
			}else if(local_item_size>=CL_DEVICE_MAX_WORK_GROUP_SIZE){
				printf("Excedido el numero de local_item_size, debe ser menor que CL_DEVICE_MAX_WORK_GROUP_SIZE\n");
			}else if(global_item_size%local_item_size!=0){
				printf("global_item_size %% local_item_size = %i %% %i   = %i\n",global_item_size,local_item_size,global_item_size%local_item_size);
			}else{
				ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
					&global_item_size, &local_item_size, 0, NULL, NULL);
				printf("Inicio copiar buffers\n");
				// Copiar el buffer minimos en el vector minimos
				ret = clEnqueueReadBuffer(command_queue, max_mem_obj, CL_TRUE, 0, 
					LIST_SIZE * sizeof(int), maximos, 0, NULL, NULL);
				printf("maximos ok\n");
				ret = clEnqueueReadBuffer(command_queue, min_mem_obj, CL_TRUE, 0, 
					LIST_SIZE * sizeof(int), minimos, 0, NULL, NULL);
				printf("minimos ok\n");
				ret = clEnqueueReadBuffer(command_queue, n_mem_obj, CL_TRUE, 0, 
					LIST_SIZE * sizeof(double), sumn, 0, NULL, NULL);
				printf("suma ok\n");



				//Mostrar solucion
				for(int i = 0; i < global_item_size; i++){
					printf("%d. max = %d. min = %d. n= %f\n", imagenM[i], maximos[i], minimos[i], sumn[i]);
				}
				

			}
			// Clean up
			ret = clFlush(command_queue);
			ret = clFinish(command_queue);
			ret = clReleaseKernel(kernel);
			ret = clReleaseProgram(program);
			ret = clReleaseMemObject(imagen_mem_objM);
			ret = clReleaseMemObject(imagen_mem_objm);
			ret = clReleaseMemObject(max_mem_obj);
			ret = clReleaseMemObject(min_mem_obj);
			ret = clReleaseMemObject(n_mem_obj);
			ret = clReleaseCommandQueue(command_queue);
			ret = clReleaseContext(context);
			//swap=false;
			if(M/s>=subM/subS){
				printf(" M=%i s= %i",M,s);
				printf(" > sM=%i ss= %i\n",subM,subS);
				swap=false;
				printf("condicion fin bucle\n");
			}else{
			//parametros para la siguiente ejecución del bucle
				subListTam=pow((double)(subM/subS),2);
				subM=subM/subS;
			}
		}while(swap==true);
		printf("fin bucle\n");
		double N = 0;
		int tam = pow((double)(M/s),(2));
		/*for(int i = 0; i < subListTam; i++){
			N+=sumn[i];
			printf("%d. max = %d. min = %d. n= %f\n", imagenM[i], maximos[i], minimos[i], sumn[i]);
		}*/
		N=N/(double)(subListTam);
		printf("Mostrado\n");
		printf("M = %i \t S = %i \t N = %f\n",M,s,N);
		system("pause");

		s+=1;
	}
	free(maximos);
	free(minimos);
	free(sumn);
	free(imagenM);
	free(imagenm);

	return 0;
}

