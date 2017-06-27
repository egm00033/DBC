
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/

//CLK_LOCAL_MEM_FENCE
//CLK_GLOBAL_MEM_FENCE 
/*__global void funcion(){
	barrier( CLK_GLOBAL_MEM_FENCE ); 

}*/
__kernel void calcularNdesCPU(__global const unsigned char *imagen, __global  float *N, const int M, __global const int *listaS) {
	//para el vector de salida
	int s=listaS[get_local_id(0)];
	int inicio=get_num_groups(0)*get_num_groups(1)*get_local_id(0);//de 25 en 25 hasta 150
	int fila=get_num_groups(1)*get_group_id(0);
	int colum=get_group_id(1);
	int posSalida=inicio+fila+colum;

	//para copiar a memoria compartida
	local unsigned char imagenLocal[128*128];

	int tabulador=M/get_num_groups(0);//numero de filas de la subimagen 640/5=128
	int posMatriz=0;
	int posSubMatriz=0;
	int row=get_local_id(0);
	int col=0;

	for (int I = tabulador*get_group_id(0)+get_local_id(0); I < tabulador*get_group_id(0)+tabulador; I+=get_local_size(0))
	{

		col=0;
		for (int J = tabulador*get_group_id(1); J < tabulador*get_group_id(1)+tabulador; J++)
		{
			posMatriz=I*M+J;
			posSubMatriz=row*tabulador+col;

			imagenLocal[posSubMatriz]=imagen[posMatriz];
			col+=1;
		}
		row+=get_local_size(0);
	}
	barrier( CLK_LOCAL_MEM_FENCE ); 

	//calcular con la memoria compartida

	posSubMatriz=0;
	int minV,maxV;
	float sPrima=(float)256/((float)M/(float)(s));
	float n=0;

	for (int I = 0; I < tabulador; I+=s)
	{
		for (int J = 0; J < tabulador; J+=s)
		{
			posSubMatriz=I*tabulador+J;
			maxV=0;
			minV=500000;
			for (int i = 0; i < s; i++)
			{
				for (int j = 0; j < s; j++)
				{
					posMatriz=min(posSubMatriz+i*tabulador+j,tabulador*tabulador-1);
					minV=min(minV,(int)imagenLocal[posMatriz]);
					maxV=max(maxV,(int)imagenLocal[posMatriz]);
				}
			}
			n+=maxV/sPrima-minV/sPrima+1;
		}
	}
	N[posSalida] = n;
	
	//sumar todos los resultados de s
	/*if(get_group_id(0) == 0 && get_group_id(1) == 0){
		N[get_local_id(0)] =get_local_id(0) ;
	}*/
}