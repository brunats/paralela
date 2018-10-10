#include<stdio.h>
#include<omp.h>

int main(){

    int i, n = 10000;
    double a[10000], b[10000], sum;
    
    for(i=0;i<n;i++){
        a[i] = b[i] = i*1.0;
    }
    sum = 0.0;
    
    #pragma omp parallel for reduction(+:sum) num_threads (8)
    for(i=0;i<n;i++){
        sum = sum + (a[i]*b[i]);
    }
    
    printf("Sum = %f\n", sum);
    
    return 0;
    
}
