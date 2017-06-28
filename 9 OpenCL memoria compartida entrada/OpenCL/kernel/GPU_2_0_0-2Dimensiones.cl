
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void calcularNdesCPU(__global const unsigned char *imagen, __global  float *N, const int M, __global const int *listaS) {
	//para el vector de salida
	int s=listaS[get_local_id(0)];
	int inicio=get_num_groups(0)*get_num_groups(1)*get_local_id(0);//de 25 en 25 hasta 150
	int fila=get_num_groups(1)*get_group_id(0);
	int colum=get_group_id(1);
	int pos=inicio+fila+colum;

	//para recorrer la imagen
	int tabulador=M/get_num_groups(0);
	int inicioMatriz=0;
	int posMatriz=0;
	int minV,maxV;
	float sPrima=(float)256/((float)M/(float)(s));
	float n=0;

	for (int I = tabulador*get_group_id(0); I < tabulador*get_group_id(0)+tabulador; I+=s)
	{
		for (int J = tabulador*get_group_id(1); J < tabulador*get_group_id(1)+tabulador; J+=s)
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


	N[pos] = n;
}