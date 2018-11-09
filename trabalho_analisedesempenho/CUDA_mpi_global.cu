#include <stdio.h>

//compilar: nvcc matrizMultiplicacao.cu -o matrizMultiplicacao
//for i in `seq 1 10`; do ./matrizMultiplicacao; done

//#define N 64
#define B 16

extern double *mA, *mB, *mC;
extern int N, num;

__global__ void matrix_multi(double *a, double *b, double *c, int N) {
	int y = blockIdx.x * blockDim.x + threadIdx.x;
	int x = blockIdx.y * blockDim.y + threadIdx.y;
	int i;
	double soma = 0.0;
	if(x < N && y < N){
		for(i=0; i<N; i++){
			soma += a[x * N + i ] * b[y + N * i];
		}
		c[y + x * N] = soma;
	}
}

extern "C" void multiplica() {
	//double *c;
	double *d_a, *d_b, *d_c;
	int size = N;
	dim3 dimen (B, B);
	//int num = 2;
	//cudaEvent_t start, stop;
	
	//cudaEventCreate(&start);
	//cudaEventCreate(&stop);

	cudaMalloc( (void **) &d_a, (size/num)*size*sizeof(double) );
	cudaMalloc( (void **) &d_b, size*size*sizeof(double) );
	cudaMalloc( (void **) &d_c, (size/num)*size*sizeof(double) );

	//a = (double *)malloc( (size/num)*size*sizeof(double) );
	//b = (double *)malloc( size*size*sizeof(double) );
	//c = (double *)malloc( (size/num)*size*sizeof(double) );

	/*
	for( int i = 0; i < N*N; i++ ) {
		//a[i] = b[i] = i;
		b[i] = i;
	}*/
	for(int i = 0; i < (N/num)*N; i++){
	    //a[i] = i;
	    mC[i] = 0;
	}
    
    //cudaEventRecord(start);
    
	cudaMemcpy( d_a, mA, (size/num)*size*sizeof(double), cudaMemcpyHostToDevice );
	cudaMemcpy( d_b, mB, size*size*sizeof(double), cudaMemcpyHostToDevice );

	//dim3 grade (((N/num) + B-1)/B, (N + B-1)/B);
	dim3 grade ((N + B-1)/B, ((N/num) + B-1)/B);
	matrix_multi<<<grade, dimen>>>( d_a, d_b, d_c , N );

	cudaMemcpy( mC, d_c, (size/num)*size*sizeof(double), cudaMemcpyDeviceToHost );
	
	/*
	cudaEventRecord(stop);
	cudaEventSynchronize(stop);
    double milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    
    printf("%f\n", milliseconds/1000.0);
    */
    
	//printf( "c[0] = %lf\n", c[0] );
	//printf( "c[%d] = %lf\n",N*N, c[N*N-1] );
	
	/*
	int i;
	for(i=0; i<(N/num)*N; i++){
		printf( "c[%d] = %lf\n",i, c[i] );
	}
	*/
	

	//free(a);
	//free(b);
	//free(c);

	cudaFree( d_a );
	cudaFree( d_b );
	cudaFree( d_c );

}
