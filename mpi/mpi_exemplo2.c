#include<mpi.h>
#include<stdio.h>
#include<string.h>

/*
compilar: mpicc -Wall mpi_ex2.c -o ex2
executar: mpirun -machinefile maquinas.txt -np 10 ex2
*/

int main(int argc, char **argv){
    
    int rank, size;
    int tag = 0;
    MPI_Status status;
    char msg[20];
    int i;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if(rank == 0){
        strcpy(msg, "Hello world\n");
        for(i=0;i<size;i++){
            printf("0 enviando 20 para %d\n", i);
            MPI_Send(msg, 20, MPI_CHAR, i, tag, MPI_COMM_WORLD);
        }
    }
    else{
        printf("%d está esperando\n", rank);
        MPI_Recv(msg, 20, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
        printf("Mensagem recebida: %s\n", msg);
    }
    
    MPI_Finalize();
    return 0;
    
}
