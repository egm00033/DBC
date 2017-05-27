
/*get_work_dim	Number of dimensions in use
get_global_size	Number of global work items
get_global_id	Global work item ID value
get_local_size	Number of local work items
get_local_id	Local work item ID
get_num_groups	Number of work groups
get_group_id	Work group ID*/
__kernel void vector_add(__global const int *A, __global  int *B, __global int *C) {

		double a;
		B[get_global_id(0)] = get_global_id(0);
		C[get_global_id(0)] = get_local_id(0);
		C[get_global_id(0)]=0;
		for (int i=0;i<32000;i++){
			for (int j=0;j<32000;j++){
			
				for (int h=0;h<32000;h++){
					for (int k=0;k<32000;k++){
						C[get_global_id(0)]+=1;
					}
					C[get_global_id(0)]/=32000;
				}
				C[get_global_id(0)]/=32000;
			}
			C[get_global_id(0)]/=32000;
		}

}