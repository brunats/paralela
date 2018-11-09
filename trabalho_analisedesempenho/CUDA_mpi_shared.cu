#include <stdio.h>

//compilar: nvcc matrizMultiplicacaoCompartilhada.cu -o matrizMultiplicacaoComp
//for i in `seq 1 10`; do ./matrizMultiplicacaoComp; done

//#define N 64
#define B 16
#define TILE_WIDTH 16

extern double *mA, *mB, *mC;
extern int N, num;

__global__ void matrixMulKernel(double *m, double *n, double *p, int width)
{
    __shared__ double sm[TILE_WIDTH][TILE_WIDTH];
    __shared__ double sn[TILE_WIDTH][TILE_WIDTH];

    int row = blockIdx.y * TILE_WIDTH + threadIdx.y;
    int col = blockIdx.x * TILE_WIDTH + threadIdx.x;

    double pvalue = 0;

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

extern "C" void multiplica() {
	//double *a, *b, *c;
	double *d_a, *d_b, *d_c;
	int size = N;
	dim3 dimen (B, B);
	//cudaEvent_t start, stop;
	
	//cudaEventCreate(&start);
	//cudaEventCreate(&stop);

	cudaMalloc( (void **) &d_a, (size/num)*size*sizeof(double) );
	cudaMalloc( (void **) &d_b, size*size*sizeof(double) );
	cudaMalloc( (void **) &d_c, (size/num)*size*sizeof(double) );

	//a = (double *)malloc( size*size*sizeof(double) );
	//b = (double *)malloc( size*size*sizeof(double) );
	//c = (double *)malloc( size*size*sizeof(double) );

	for( int i = 0; i < (N/num)*N; i++ ) {
		//a[i] = b[i] = i;
		mC[i] = 0;
	}
    
    //cudaEventRecord(start);
    
	cudaMemcpy( d_a, mA, (size/num)*size*sizeof(double), cudaMemcpyHostToDevice );
	cudaMemcpy( d_b, mB, size*size*sizeof(double), cudaMemcpyHostToDevice );

	dim3 grade ((N + B-1)/B, ((N/num) + B-1)/B);
	matrixMulKernel<<<grade, dimen>>>( d_a, d_b, d_c, N );

	cudaMemcpy( mC, d_c, (size/num)*size*sizeof(double), cudaMemcpyDeviceToHost );
	
	
	//cudaEventRecord(stop);
	//cudaEventSynchronize(stop);
    //double milliseconds = 0;
    //cudaEventElapsedTime(&milliseconds, start, stop);
    
    //printf("%f\n", milliseconds/1000.0);
    
    
	//printf( "c[0] = %lf\n", c[0] );
	//printf( "c[%d] = %lf\n",N*N, c[N*N-1] );
	
	/*
	int i;
	for(i=0; i<N*N; i++){
		printf( "c[%d] = %lf\n",i, c[i] );
	}
	*/
	

	//free(a);
	//free(b);
	//free(c);

	cudaFree( d_a );
	cudaFree( d_b );
	cudaFree( d_c );

	//return 0;
} /* end main */
