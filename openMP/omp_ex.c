#include<stdio.h>
#include<omp.h>

int main(){

    int id;
    int teste = 100;
    
    #pragma omp parallel private(id) num_threads(10) firstprivate(teste)
    {
        id = omp_get_thread_num();
        printf("Thread numero %d = %d\n", id, teste);
        #pragma omp master
        {
            printf("Master\n");
        }
    }
    
    return 0;
    
}
