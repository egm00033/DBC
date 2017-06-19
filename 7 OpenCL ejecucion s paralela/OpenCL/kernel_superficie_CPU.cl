
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void calcularNdesCPU(__global const unsigned char *imagen, __global  float *vSalida, const int M, __global const int *listaS, const int tamS) {
	//salida = numero de hilos(globalId)*numerodeSs
	int tamSalida=get_global_size(0)*tamS;
	int numS=tamS;
	int pos;
	/*for (int i = 0; i < numS; i++){
		pos=numS*get_global_id(0)+i;
		
		vSalida[pos] = pos;
	}*/
	vSalida[0]=11111;
	vSalida[1]=11111;
	vSalida[2]=11111;
	vSalida[3]=11111;
	vSalida[4]=11111;
	vSalida[5]=11111;
	vSalida[6]=11111;
	vSalida[7]=11111;
	vSalida[8]=11111;
	vSalida[9]=11111;
	vSalida[10]=11111;
	vSalida[11]=11111;
	
}