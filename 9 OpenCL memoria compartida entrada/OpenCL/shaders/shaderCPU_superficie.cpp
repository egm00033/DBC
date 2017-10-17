#include "stdafx.h"
#include "shaderCPU_superficie.h"

//primera implementación con dimension 2d
shaderCPU_superficie::shaderCPU_superficie(void) : shader(1,"kernel/kernel_superficie_CPU.cl")
{
	
}

//Calcula la sumatoria de n para un tamaño s dado
void shaderCPU_superficie::CalcularN(unsigned char *img3,float *NdeS, int M, int tamListaS,int *listaS){
	//éxito en la creación del programa
	if(ret==0){
		cl_int ret;
		int particiones=M/listaS[tamListaS-1];
//particiones=veces en las que se subdivide M
		size_t global_item_size = particiones*particiones; // subdivisiones de cada superficie
		size_t local_item_size = 1; // Grupo de trabajo
		size_t * global = (size_t*) malloc(sizeof(size_t)*particiones);
		size_t * local = (size_t*) malloc(sizeof(size_t)*particiones);
		global[0] = particiones; global[1] = particiones;
		local [0] = 1; local [1] = 1;

		const int LIST_SIZE =M*M;// Lista de elementos de tamaño MxM
		const int tamS=tamListaS;

		int tamSalida=tamListaS*global_item_size;
		tamSalida=tamListaS*global_item_size;
		float *vSalida=(float*) calloc(tamSalida,sizeof(float));

		for(int i=0;i<tamSalida;i++){
			vSalida[i]=(-1)*i;
		}

		if(false)printf("global size =%i\t localsize= %i\n",global_item_size,local_item_size);

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
			ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global, local, 0, NULL, NULL);

			if(ret==-54){
				printf("(ERROR -54)clEnqueueNDRangeKernel=CL_INVALID_WORK_GROUP_SIZE\n");
				printf("local_item_size(%i) < CL_DEVICE_MAX_WORK_GROUP_SIZE(%i)\n",local_item_size,CL_DEVICE_MAX_WORK_GROUP_SIZE);
				printf("global_work_size(%i) < CL_DEVICE_ADDRESS_BITS(%i)\n",global_item_size,CL_DEVICE_ADDRESS_BITS);
			}else if(ret!=0)printf("clEnqueueNDRangeKernel=%i\n",ret,ret);

			// Copiar el buffer salida en el vector NdeS
			for(int i=0;i<tamSalida;i++){
				vSalida[i]=tamSalida-i;
			}
			tEjecucion = clock();

			ret = clEnqueueReadBuffer(command_queue, salida_mem_obj, CL_TRUE, 0, tamSalida*sizeof(float), vSalida, 0, NULL, NULL);

			tEjecucion = tEjecucion-clock();
			

			//printf("salida copiado\n");
			for(int i=0;i<tamSalida;i++){
				//printf("%i = %f\n",i,vSalida[i]);
				NdeS[i/global_item_size]+=vSalida[i];
			}
			/*for(int i=0;i<tamS;i++){
				printf("NdeS[%i] = %f\n",listaS[i],NdeS[i]);
			}*/

		}



		ret = clReleaseMemObject(entrada_mem_obj);
		ret = clReleaseMemObject(salida_mem_obj);
		free(vSalida);
		free(global);
		free(local);
	}else{
		printf("ERROR ret==0 en CalcularN\n");
		system("pause");
	}

}

shaderCPU_superficie::~shaderCPU_superficie(void)
{

}
