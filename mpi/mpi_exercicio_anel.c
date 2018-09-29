#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
    
    int rank, size;
    int tag = 0;
    int token;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if(rank == 0){
        token = 0;
        MPI_Send(&token, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, size-1, tag, MPI_COMM_WORLD, &status);
        printf("token: %d\n", token);
    }
    else{
        MPI_Recv(&token, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
        token += rank;
        if(rank==size-1){
            MPI_Send(&token, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            printf("%d enviando %d para %d\n", rank, token, 0);
        }
        else{
            MPI_Send(&token, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
            printf("%d enviando %d para %d\n", rank, token, rank+1);
        }
    }
    
    MPI_Finalize();
    return 0;
    
}
