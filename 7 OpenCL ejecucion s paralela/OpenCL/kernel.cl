
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void calcularNdesGPU(__global const char *imagen, __global  float *N, const int M) {

	int s=get_global_id(0)+2;
	int minV,maxV;
	float sPrima=(float)256/((float)M/(float)(s));
	float n=0;

	int inicio=0;
	int pos=0;
	for (int I = 0; I < M; I+=s)
	{
		for (int J = 0; J < M; J+=s)
		{
			inicio=I*M+J;
			maxV=0;
			minV=256;
			for (int i = 0; i < s; i++)
			{
				for (int j = 0; j < s; j++)
				{
					pos=min(inicio+i*M+j,M*M-1);
					//if(max<vEntrada[pos])max=vEntrada[pos];
					//if(min>vEntrada[pos])min=vEntrada[pos];
					
					minV=min(minV,(int)imagen[pos]);
					maxV=max(maxV,(int)imagen[pos]);
				}
			}
			n+=maxV/sPrima-minV/sPrima+1;
		}
	}
	N[get_global_id(0)] = n;
}