#include <stdio.h>

//compilar: nvcc matrizMultiplicacaoCompartilhada.cu -o matrizMultiplicacaoComp
//for i in `seq 1 10`; do ./matrizMultiplicacaoComp; done

#define N 64
#define B 32
#define TILE_WIDTH 32

__global__ void matrix_multi(float *a, float *b, float *c) {
	int y = blockIdx.x * blockDim.x + threadIdx.x;
	int x = blockIdx.y * blockDim.y + threadIdx.y;
	int i;
	float soma = 0.0;
	if(x < N && y < N){
		for(i=0; i<N; i++){
			soma += a[x * N + i ] * b[y + N * i];
		}
		c[x + y * N] = soma;
	}
}

__global__ void matrixMulKernel(float *m, float *n, float *p, int width)
{
    __shared__ float sm[TILE_WIDTH][TILE_WIDTH];
    __shared__ float sn[TILE_WIDTH][TILE_WIDTH];

    int row = blockIdx.y * TILE_WIDTH + threadIdx.y;
    int col = blockIdx.x * TILE_WIDTH + threadIdx.x;

    float pvalue = 0;

    for (int i = 0; i < width/TILE_WIDTH; i++) {
        sm[threadIdx.y][threadIdx.x] = m[row * width + (i * TILE_WIDTH + threadIdx.x)];
        sn[threadIdx.y][threadIdx.x] = n[col + (i * TILE_WIDTH + threadIdx.y) * width];
        __syncthreads();

        for (int k = 0; k < TILE_WIDTH; k++) {
            pvalue += sm[threadIdx.y][k] * sn[k][threadIdx.x];
        }
        __syncthreads();

    }
    p[row * width + col] = pvalue;
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
	matrixMulKernel<<<grade, dimen>>>( d_a, d_b, d_c, N );

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
