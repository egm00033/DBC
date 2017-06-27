#include "stdafx.h"
#include "shaderCPU.h"

//creación del programa dependiento de nuestro HW
shaderCPU::shaderCPU() : shader(1,"kernel/kernel_profundidad_CPU.cl")
{
	
}

//Calcula la sumatoria de n para un tamaño s dado
void shaderCPU::CalcularN(unsigned char *img3,float *NdeS, int M, int tamListaS,int *listaS){
	//éxito en la creación del programa
	if(ret==0){
		cl_int ret;
		const int LIST_SIZE =M*M;// Lista de elementos de tamaño MxM
		const int tamS=tamListaS;

		size_t global_item_size = tamS; // desde s=2 hasta s=M/2 si % == 0
		size_t local_item_size = 1; // Grupo de trabajo

		if(true)printf("global size =%i\t localsize= %i\n",global_item_size,local_item_size);

		//crear buffers de memoria en el dispositivo por cada vector
		cl_mem entrada_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			LIST_SIZE * sizeof(char), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		cl_mem salida_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
			tamS * sizeof(float), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		cl_mem listaS_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			tamS * sizeof(int), NULL, &ret);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);



		ret=clSetKernelArg(kernel, 2, sizeof(M), &M);//set parametro del kernel
		if(ret!=0)printf("clSetKernelArg(s2), linea: %d, error: %d\n", __LINE__, ret);

		// Copiar cada vector de entrada en su buffer
		ret = clEnqueueWriteBuffer(command_queue, entrada_mem_obj, CL_TRUE, 0,
			LIST_SIZE * sizeof(char), img3, 0, NULL, NULL);
		if(ret!=0)printf("respuesta de la linea %d es %d\n", __LINE__, ret);

		ret = clEnqueueWriteBuffer(command_queue, listaS_mem_obj, CL_TRUE, 0,
			tamS * sizeof(int), listaS, 0, NULL, NULL);
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

			ret = clEnqueueReadBuffer(command_queue, salida_mem_obj, CL_TRUE, 0, tamS * sizeof(float), NdeS, 0, NULL, NULL);
		}



		ret = clReleaseMemObject(entrada_mem_obj);
		ret = clReleaseMemObject(salida_mem_obj);

	}else{
		printf("ERROR ret==0 en CalcularN\n");
		system("pause");
	}
}

shaderCPU::~shaderCPU(void)
{
}
