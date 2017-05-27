
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void vector_add(__global const int *A, __global  int *B, __global int *C) {

	B[get_global_id(0)] = 0;
	C[get_global_id(0)] = 500000;

	for(int i=0;i<get_local_size(0);i++){
		B[get_global_id(0)] = max(B[get_global_id(0)],A[get_global_id(0)*get_local_size(0)+i]);
		C[get_global_id(0)] = min(C[get_global_id(0)],A[get_global_id(0)*get_local_size(0)+i]);
	}
}