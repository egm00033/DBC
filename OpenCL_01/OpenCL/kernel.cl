
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void vector_add(__global const int *A, __global  int *B, __global int *C) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
	B[i] = 0;
	C[i] = 256;
	int desplazamiento=get_group_id(0)*get_local_size(0);
    // Do the operation
	for (int a=0;a<get_local_size(0);a++){
		B[i] = max(A[a+i*get_local_size(0)],B[i]);
		C[i] = min(A[a+i*get_local_size(0)],C[i]);
	}
    

	//C[i] = get_local_size(0); 
}