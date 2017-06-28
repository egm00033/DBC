
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/

//CLK_LOCAL_MEM_FENCE
//CLK_GLOBAL_MEM_FENCE 

__kernel void calcularNdesCPU(__global const unsigned char *imagen, __global  float *N, const int M, __global const int *listaS) {
	//para el vector de salida
	//dim 0 para la y
	//dim 1 para la x
	//dim 2 para la z

	int inicio=get_local_size(0)*get_local_size(1)*get_local_id(2);
	int fila=get_local_size(1)*get_local_id(0);
	int colum=get_local_id(1);
	int capa=get_local_id(2);

	int s=listaS[capa];
	int posV=inicio+fila+colum;

	int tabulador=M/get_local_size(0);//numero de filas de la subimagen 640/5=128
	int posMatriz=0;

	int minV,maxV;
	int inicioMatriz;
	float sPrima=(float)256/((float)M/(float)(s));
	float n=0;

	for (int I = tabulador*get_local_id(0); I < tabulador*get_local_id(0)+tabulador; I+=s)
	{
		for (int J = tabulador*get_local_id(1); J < tabulador*get_local_id(1)+tabulador; J+=s)
		{
			inicioMatriz=I*M+J;
			maxV=0;
			minV=500000;
			for (int i = 0; i < s; i++)
			{
				for (int j = 0; j < s; j++)
				{
					posMatriz=min(inicioMatriz+i*M+j,M*M-1);
					minV=min(minV,(int)imagen[posMatriz]);
					maxV=max(maxV,(int)imagen[posMatriz]);
				}
			}
			n+=maxV/sPrima-minV/sPrima+1;
		}
	}

	N[posV] = n;
}