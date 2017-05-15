
/*get_work_dim(0)	Number of dimensions in use
get_global_size(0)	Number of global work items
get_global_id(0)	Global work item ID value
get_local_size(0)	Number of local work items
get_local_id(0)	Local work item ID
get_num_groups(0)	Number of work groups
get_group_id(0)	Work group ID*/

__kernel void vector_add(__global const int *A, __global  int *B, __global int *C, __global double *n) {
 
    
	int grid = get_global_id(0);//grid=indica en que hilo estamos trabajando
	B[grid] = 0;
	C[grid] = 2000000;

	int s=(int)sqrt((double)get_local_size(0));
	int M=(int)sqrt((double)get_global_size(0)*get_local_size(0));

	int ngrid=M/s;
	int fila = get_global_id(0)/ngrid;
	int col = get_global_id(0)%ngrid;
	
	int pos=fila*M*s+col*s;
	for(int i=0;i<s;i++){
		for (int j=0;j<s;j++){
			B[grid] = max(A[pos+i*M+j],B[grid]);
			C[grid] = min(A[pos+i*M+j],C[grid]);
		}
	}
    n[grid]=(double)(B[grid]-C[grid]+1);

}