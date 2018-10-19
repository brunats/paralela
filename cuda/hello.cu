#include<stdio.h>

//compilar: nvcc hello.cu -o hello

__device__ const char *STR = "Hello world!";
const char STR_LENGHT = 12;

__global__ void hello(){
    printf("%c\n", STR[threadIdx.x % STR_LENGHT]);
}

int main(void){
    int num_threads = STR_LENGHT;
    int num_blocks = 2;
    dim3 dimBlock(16,16);
    dim3 dimGrid(32,32);
    //hello<<<dimGrid,dimBlock>>>();
    hello<<<1,12>>>();
    cudaDeviceSynchronize();
    
    return 0;
}
