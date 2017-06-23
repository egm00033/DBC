
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void calcularNdesCPU(__global const unsigned char *imagen, __global  float *N, const int M, __global const int *listaS) {
	int x=get_group_id(0)*get_local_size(0);
	int y=get_group_id(1)*get_num_groups(0)*get_local_size(0);
	int z=get_local_id(0);
	int pos=x+y+z;
	N[pos] = get_group_id(0);
}