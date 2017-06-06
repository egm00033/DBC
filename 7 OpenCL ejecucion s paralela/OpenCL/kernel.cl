
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void calcularNdes(__global const char *imagen, __global  float *N, const int M) {
	//global_id=[0 : M/2-1]
	int s=get_global_id(0)+2;
	float sPrima=(float)256/((float)M/(float)(s));
	//sPrima=1;
	int minV,maxV;
	float n=0;

	int particiones=M/s;
	int inicio=0;
	for(int I=0;I<particiones;I++){//filas imagen
		for(int J=0;J<particiones;J++){//columnas imagen
			inicio=I*M*s+J*s;
			minV=500;
			maxV=-1;
			for(int i=0;i<s;i++){//filas del grid
				for(int j=0;j<s;j++){//columnas del grid
					minV=min(minV,(int)imagen[inicio+i*M+j]);
					maxV=max(maxV,(int)imagen[inicio+i*M+j]);

				}
			}
			n+=maxV/sPrima-minV/sPrima+1;
		}
	}

	N[get_global_id(0)] = n;
}