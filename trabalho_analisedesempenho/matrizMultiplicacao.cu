#include <stdio.h>

//compilar: nvcc matrizMultiplicacao.cu -o matrizMultiplicacao
//for i in `seq 1 10`; do ./matrizMultiplicacao; done

#define N 64
#define B 32

__global__ void matrix_multi(float *a, float *b, float *c) {
	int y = blockIdx.x * blockDim.x + threadIdx.x;
	int x = blockIdx.y * blockDim.y + threadIdx.y;
	int i;
	float soma = 0.0;
	if(x < N && y < N){
		for(i=0; i<N; i++){
			soma += a[x * N + i ] * b[y + N * i];
		}
		c[y + x * N] = soma;
	}
}

int main() {
	float *a, *b, *c;
	float *d_a, *d_b, *d_c;
	int size = N;
	dim3 dimen (B, B);
	cudaEvent_t start, stop;
	
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaMalloc( (void **) &d_a, size*size*sizeof(float) );
	cudaMalloc( (void **) &d_b, size*size*sizeof(float) );
	cudaMalloc( (void **) &d_c, size*size*sizeof(float) );

	a = (float *)malloc( size*size*sizeof(float) );
	b = (float *)malloc( size*size*sizeof(float) );
	c = (float *)malloc( size*size*sizeof(float) );

	for( int i = 0; i < N*N; i++ ) {
		a[i] = b[i] = i;
		c[i] = 0;
	}
    
    cudaEventRecord(start);
    
	cudaMemcpy( d_a, a, size*size*sizeof(float), cudaMemcpyHostToDevice );
	cudaMemcpy( d_b, b, size*size*sizeof(float), cudaMemcpyHostToDevice );

	dim3 grade ((N + B-1)/B, (N + B-1)/B);
	matrix_multi<<<grade, dimen>>>( d_a, d_b, d_c );

	cudaMemcpy( c, d_c, size*size*sizeof(float), cudaMemcpyDeviceToHost );
	
	
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    
    printf("%f\n", milliseconds/1000.0);
    
    
	//printf( "c[0] = %lf\n", c[0] );
	//printf( "c[%d] = %lf\n",N*N, c[N*N-1] );
	
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
