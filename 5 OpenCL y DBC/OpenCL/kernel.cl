
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void vector_add(__global const int *A, __global  int *B, __global int *C,__global float *N, int s2,float sPrima) {

	B[get_global_id(0)] = 0;
	C[get_global_id(0)] = 500000;
	int pos=0;
	pos=get_global_id(0)*s2;
	int ultimo=(get_global_size(0)-1)*s2;//evita que se desborde el vector
	pos=min(pos,ultimo);
	
	for(int i=0;i<s2;i++){
		B[get_global_id(0)] = max(B[get_global_id(0)],A[pos+i]);
		C[get_global_id(0)] = min(C[get_global_id(0)],A[pos+i]);
	}
	N[get_global_id(0)]=((B[get_global_id(0)]/sPrima)-(C[get_global_id(0)]/sPrima)+1);
}