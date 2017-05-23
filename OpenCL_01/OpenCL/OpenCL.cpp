// OpenCL.cpp: define el punto de entrada de la aplicacion de consola.



#include "stdafx.h"
#include <math.h>

int max(int a, int b){
	if(a>b)
		return a;
	return b;
}
int min(int a, int b){
	if(a<b)
		return a;
	return b;
}

void calcularn(int subS, int M, cl_device_id device_id, char *shader, size_t source_size, int *imagenM,int *imagenm,double *sumn){
	printf("Inicio cacularn s'=%i, M=%i\n",subS,M);
	cl_int ret;
	int LIST_SIZE = M*M;
	size_t local_item_size = subS*subS; 
	int *maximos = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *minimos = (int*)malloc(sizeof(int)*LIST_SIZE);
	const size_t global_item_size = LIST_SIZE/local_item_size; 

	printf("subListTam %i, global_item_size %i, local_item_size %i\n",LIST_SIZE,global_item_size,local_item_size);

	// Creando el contexto de OpenCL
	cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	// Crear lista de operadiones para ejecutar
	//por defecto en orden | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE (4º argumento)
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

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
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	ret = clEnqueueWriteBuffer(command_queue, imagen_mem_objm, CL_TRUE, 0,
		LIST_SIZE * sizeof(int), imagenm, 0, NULL, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	// Crear programa del shader
	cl_program program = clCreateProgramWithSource(context, 1, 
		(const char **)&shader, (const size_t *)&source_size, &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	// compilar programa
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

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
	}


	// Crear kernel de OpenCL
	cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	// Establecer argumentos
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&imagen_mem_objM);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&imagen_mem_objm);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&max_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&min_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&n_mem_obj);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	//añadir para arreglar los problemas de salida
	ret = clSetKernelArg(kernel, 3, sizeof(int), &LIST_SIZE);

	if(global_item_size>=CL_DEVICE_ADDRESS_BITS){
		printf("Excedido el numero de global_work_size = %i, debe ser menor que CL_DEVICE_ADDRESS_BITS=%i\n",global_item_size,CL_DEVICE_ADDRESS_BITS);
	}else if(local_item_size>=CL_DEVICE_MAX_WORK_GROUP_SIZE){
		printf("Excedido el numero de local_item_size, debe ser menor que CL_DEVICE_MAX_WORK_GROUP_SIZE\n");
	}else if(LIST_SIZE!=global_item_size*local_item_size){
		printf("ERROR: global_item_size*local_item_size = %i * %i   != %i\n",global_item_size,local_item_size,global_item_size*local_item_size);
	}else if(global_item_size<local_item_size){
		printf("ERROR: global_item_size<local_item_size = %i < %i  \n",global_item_size,local_item_size);
		//se hace de forma tradicional is local>global
		printf("Iniciando calculo manual\n");

		int sGrid=M/subS;
		for (int global = 0; global < global_item_size; global++)
		{
			int filaGrid=global/sGrid;
			int colGrid=global%sGrid;
			int grid=sGrid*filaGrid+colGrid;

			maximos[grid] = 0;
			minimos[grid] = 2000000;
			for(int local = 0; local < local_item_size; local++){


				int filaLocal=local/subS;
				int colLocal=local%subS;

				int inicio=filaGrid*M*subS+colGrid*subS;

				int pos=inicio+filaLocal*M+colLocal;
				maximos[grid] = max(imagenM[pos],maximos[grid]);
				minimos[grid] = min(imagenm[pos],minimos[grid]);
				printf("actual = %i, max %i, min %i \n",imagenm[pos],maximos[grid],minimos[grid]);

			}
			sumn[grid]=(double)(maximos[grid]-minimos[grid]+1);
			//printf("grid = %i\n",grid);
		}

	}else{
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
			&global_item_size, &local_item_size, 0, NULL, NULL);
		printf("Inicio copiar buffers\n");
		// Copiar el buffer minimos en el vector minimos
		ret = clEnqueueReadBuffer(command_queue, max_mem_obj, CL_TRUE, 0, 
			LIST_SIZE * sizeof(int), maximos, 0, NULL, NULL);
		ret = clEnqueueReadBuffer(command_queue, min_mem_obj, CL_TRUE, 0, 
			LIST_SIZE * sizeof(int), minimos, 0, NULL, NULL);
		ret = clEnqueueReadBuffer(command_queue, n_mem_obj, CL_TRUE, 0, 
			LIST_SIZE * sizeof(double), sumn, 0, NULL, NULL);

	}

	//copiar y Mostrar solucion
	for(int i = 0; i < global_item_size; i++){
		//printf("%d. max = %d. min = %d. n= %f\n", i, maximos[i], minimos[i], sumn[i]);
		imagenM[i]=maximos[i];
		imagenm[i]=minimos[i];
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

	free(maximos);
	free(minimos);
	printf("Fin cacularn s=%i\n",subS);
}

double calcularN(int s, int M,cl_device_id device_id, char *shader,  size_t source_size, int *entrada){
	printf("Inicio cacularN M= %i, s= %i\n",M,s);
	size_t global_item_size=0;
	int LIST_SIZE=M*M;
	int subListTam=LIST_SIZE;
	int *imagenM = (int*)malloc(sizeof(int)*LIST_SIZE);
	int *imagenm = (int*)malloc(sizeof(int)*LIST_SIZE);
	double *sumn = (double*)malloc(sizeof(double)*LIST_SIZE);

	int subS=2;
	int subM=M;
	//subdividir en grid menores


	//vectores de entrada
	for(int i = 0; i < M*M; i++) {
		imagenM[i] = entrada[i];
		imagenm[i] = entrada[i];
		sumn[i]=0;
	}
	bool swap=true;
	do{
		//bucle iterativo para un S'
		for (subS=2 ; subS < s; subS++)
		{
			if(M%subS==0&&s%subS==0&&CL_DEVICE_ADDRESS_BITS>pow(double(M/subS),2)){
				break;
			}
		}

		subM=M;


		//es necesario subdividir?
		//if((M*M)/(subS*subS)>CL_DEVICE_ADDRESS_BITS||(subS*subS>CL_DEVICE_MAX_WORK_GROUP_SIZE)){
		if(true){
			printf("-------------------------------------subdividir obligatorio\n");
			if(M%2==0&&subS%2==0){
				int Msubdivision=M/2;
				for (int i = 0; i < 4; i++)
				{
					int inicio=(i/2)*M*Msubdivision+(i%2)*Msubdivision;
					int desplazamiento=Msubdivision;
					for(int j = 0; j < Msubdivision*Msubdivision; j++) {
						imagenM[j] = entrada[inicio+M*(j/Msubdivision)+(j%Msubdivision)];
						imagenm[j] = entrada[inicio+desplazamiento];
						printf("pos= %i, tabla= %i\n",j,inicio+M*(j/Msubdivision)+(j%Msubdivision));
						sumn[i]=0;
					}
					calcularn(subS, Msubdivision, device_id, shader,  source_size, imagenM, imagenm,sumn);
				}
				subM=subM/2;
			}else{

				printf("no se puede subdividir por 2\n");
			}
		}else{
			calcularn(subS, subM, device_id, shader,  source_size, imagenM, imagenm,sumn);
		}


		//swap=false;
		M=M/subS;
		s=s/subS;
		if(M/s>=subM/subS){
			swap=false;
		}else{
			//parametros para la siguiente ejecución del bucle
			subListTam=pow((double)(subM/subS),2);
			//subM=subM/subS;
			swap=true;
		}
	}while(swap==true);
	double N = 0;


	int tam = M;
	tam*=tam;
	for(int i = 0; i < tam; i++){
		N+=sumn[i];
		//printf("%i. max = %d. min = %d. n= %f\n", i, imagenM[i], imagenm[i], sumn[i]);
	}
	N=N/(double)(tam);

	free(sumn);
	free(imagenM);
	free(imagenm);
	return N;
}

int _tmain(int argc, _TCHAR* argv[])
{

	int M = 8;
	int s = 2;
	int LIST_SIZE = M*M;// Lista de elementos de tamaño M
	// Vectores de entrada	
	int *entrada = (int*)malloc(sizeof(int)*LIST_SIZE);

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

	// conseguir informacion de la plataforma y el dispositivo
	cl_device_id device_id = NULL;   
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;


	cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platforms*sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

	ret = clGetDeviceIDs( platforms[1], CL_DEVICE_TYPE_ALL, 1, 
		&device_id, &ret_num_devices);
	if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);



	//bucle para calcular todos los N
	while(s<=M/2){
		system("cls");

		while(M%s!=0&&s<=M/2){
			//system("cls");
			s+=1;
		}	


		//<<<<<<<<<<<<<<<<<------inicio
		double N = calcularN(s, M, device_id, shader, source_size, entrada);
		//fin------------------->

		printf("M = %i \t S = %i \t N = %f\n",M,s,N);
		system("pause");
		s+=1;
	}
	system("pause");

	return 0;
}

