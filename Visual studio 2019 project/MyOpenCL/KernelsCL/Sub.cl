__kernel void Sub(__global int*a, __global int* b,__global int* n)
{
	int i = get_global_id(0);
	if(i >= n[0]) return;
	a[i] -= b[i];
}";