
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void calcularNdes(__global const char *imagen, __global  float *N) {

	N[get_global_id(0)] = get_global_id(0);
	for(int i=0;i<4;i++){
	N[i]=1.2;
	}
}