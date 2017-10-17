#include "stdafx.h"
#include "shaderGPU2_0_1.h"

shaderGPU2_0_1::shaderGPU2_0_1(void) : shader(0,"kernel/GPU_2_0_1-2D_mem_entrada.cl")
{

}

//Calcula la sumatoria de N para todas las s
void shaderGPU2_0_1::CalcularN(unsigned char *img3,float *NdeS, int M, int tamListaS,int *listaS){
	//éxito en la creación del programa


	if(ret==0){
		//cl_int ret;
		const int LIST_SIZE =M*M;// Lista de elementos de tamaño MxM
		const int tamgroup=7;//dimension z del cubo
		int tamGrid=listaS[tamListaS-1];//divide el ancho de la matriz
		int dimensiones=2;
		int particinesM=M/tamGrid; //5
		int numgroup=pow((double)particinesM,2);//5*5=25workgroup
		int totalThread=numgroup*tamgroup;//5*5*7=175

		float * salida = (float*) malloc(sizeof(float)*totalThread);//guarda un valor por cada thread

		size_t * global = (size_t*) malloc(sizeof(size_t)*dimensiones);
		size_t * local = (size_t*) malloc(sizeof(size_t)*dimensiones);
		size_t local_item_size = tamgroup; // Grupo de trabajo


		global[0] = particinesM*tamgroup;
		global[1] = particinesM;
		//global[2] = tamgroup;
		local [0] = tamgroup;
		local [1] = 1;
		//local [2] = 1;

		//crear buffers de memoria en el dispositivo por cada vector
		cl_mem entrada_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			LIST_SIZE * sizeof(char), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);
		cl_mem salida_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
			totalThread * sizeof(float), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		cl_mem listaS_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			tamListaS * sizeof(int), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);



		ret=clSetKernelArg(kernel, 2, sizeof(M), &M);//set parametro del kernel
		if(ret!=0)printf("clSetKernelArg(s2), linea: %d, error: %d\n", __LINE__, ret);

		// Copiar cada vector de entrada en su buffer
		ret = clEnqueueWriteBuffer(command_queue, entrada_mem_obj, CL_TRUE, 0,
			LIST_SIZE * sizeof(char), img3, 0, NULL, NULL);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clEnqueueWriteBuffer(command_queue, listaS_mem_obj, CL_TRUE, 0,
			tamListaS * sizeof(int), listaS, 0, NULL, NULL);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		// Establecer argumentos
		ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&entrada_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		/*ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&listaS_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);*/

		ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&salida_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&listaS_mem_obj);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);


		//limite hardware


		tEjecucion = clock();

		ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global, local, 0, NULL, NULL);

		tEjecucion = tEjecucion-clock();
		if(ret==-54){
			printf("(ERROR -54)clEnqueueNDRangeKernel=CL_INVALID_WORK_GROUP_SIZE\n");
			printf("local_item_size(%i) < CL_DEVICE_MAX_WORK_GROUP_SIZE(%i)\n",tamgroup,CL_DEVICE_MAX_WORK_GROUP_SIZE);
			printf("global_work_size(%i) < CL_DEVICE_ADDRESS_BITS(%i)\n",totalThread,CL_DEVICE_ADDRESS_BITS);
		}else if(ret!=0){
			printf("clEnqueueNDRangeKernel=%i\n",ret,ret);
		}else{

			// Copiar el buffer salida en el vector NdeS

			ret = clEnqueueReadBuffer(command_queue, salida_mem_obj, CL_TRUE, 0, totalThread * sizeof(float), salida, 0, NULL, NULL);

			int pos=0;
			for(int i=0; i<tamListaS; i++){

				NdeS[i]=0;
				for(int j=0; j<numgroup; j++){
					//printf("i=%i, salida=%f\n",i*numgroup+j,salida[i*numgroup+j]);
					NdeS[i]+=salida[i*numgroup+j];
				}
				//printf("----------s=%i, N=%f\n",listaS[i],NdeS[i]);
			}

		}


		ret = clReleaseMemObject(entrada_mem_obj);
		ret = clReleaseMemObject(salida_mem_obj);

	}else{
		printf("ERROR ret==0 en CalcularN\n");
		system("pause");
	}
}

shaderGPU2_0_1::~shaderGPU2_0_1(void)
{

}