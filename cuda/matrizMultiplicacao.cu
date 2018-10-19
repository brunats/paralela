#include <stdio.h>

//compilar: nvcc matrizMultiplicacao.cu -o matrizMultiplicacao

#define N 3
#define B 1

__global__ void matrix_multi(float *a, float *b, float *c) {
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	int i;
	float soma = 0.0;
	if(x < N && y < N){
		for(i=0; i<N; i++){
			soma += a[x * i + y ] * b[x + y * i];
		}
		c[x + y * N] = soma;
	}
}

int main() {
	float *a, *b, *c;
	float *d_a, *d_b, *d_c;
	int size = N;
	dim3 dimen (B, B);

	cudaMalloc( (void **) &d_a, size*size*sizeof(float) );
	cudaMalloc( (void **) &d_b, size*size*sizeof(float) );
	cudaMalloc( (void **) &d_c, size*size*sizeof(float) );

	a = (float *)malloc( size*size*sizeof(float) );
	b = (float *)malloc( size*size*sizeof(float) );
	c = (float *)malloc( size*size*sizeof(float) );

	for( int i = 0; i < N*N; i++ ) {
		a[i] = b[i] = 1;
		c[i] = 0;
	}

	cudaMemcpy( d_a, a, size*size*sizeof(float), cudaMemcpyHostToDevice );
	cudaMemcpy( d_b, b, size*size*sizeof(float), cudaMemcpyHostToDevice );

	dim3 grade ((N + B-1)/B, (N + B-1)/B);
	matrix_multi<<<grade, dimen>>>( d_a, d_b, d_c );

	cudaMemcpy( c, d_c, size*size*sizeof(float), cudaMemcpyDeviceToHost );

	int i;
	for(i=0; i<N*N; i++){
		printf( "c[%d] = %lf\n",i, c[i] );
	}

	free(a);
	free(b);
	free(c);

	cudaFree( d_a );
	cudaFree( d_b );
	cudaFree( d_c );

	return 0;
} /* end main */
