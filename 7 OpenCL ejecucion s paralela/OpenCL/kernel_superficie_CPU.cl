
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
/**
*Calcula la sumatoria de n para cualquier s de una subdivisión de la imagen
*/
__kernel void calcularNdesCPU(__global const unsigned char *imagen, __global  float *vSalida, const int M, __global const int *listaS, const int tamS) {
	int numS=tamS;
	int minV,maxV;
	float sPrima=0.02;
	int s;
	//dividir imagen
	int pos=0;
	int division=M/2;
	int id=get_global_size(0)*get_global_id(0)+get_global_id(1);
	int inicioThread=get_global_id(0)*M*division+get_global_id(1)*division;


	for(int posS = 0; posS < tamS; posS++){

		s=listaS[posS];
		sPrima=(float)256/((float)M/(float)(s));

		float n=0;

		for (int I = 0; I < division; I+=s)
		{
			for (int J = 0; J < division; J+=s)
			{
				int inicio=inicioThread+I*M+J;
				maxV=0;
				minV=500000;
				for (int i = 0; i < s; i++)
				{
					for (int j = 0; j < s; j++)
					{
						pos=min(inicio+i*M+j,M*M-1);
						minV=min(minV,(int)imagen[pos]);
						maxV=max(maxV,(int)imagen[pos]);
					}
				}
				n+=maxV/sPrima-minV/sPrima+1;
			}
		}
		int posSalida=posS*get_global_size(0)*get_global_size(1)+id;
		//vSalida[posSalida]=n;
		vSalida[posSalida]=n;
	}



}