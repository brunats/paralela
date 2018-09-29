#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int **criaMatriz(int n, int rank){
    int **m = NULL;
    int *dados = NULL;
    int i, j;
    
    m = (int**)malloc(n*sizeof(int*));
    dados = (int*)malloc(n*n*sizeof(int));
    
    for(i=0;i<n;i++){
        m[i] = (dados + i*n);
    }
    
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            m[i][j] = (j*10 + i*3)%(rank*2 + 1);
        }
    }
    
    return m;
}

//Material: https://github.com/leorrodrigues/mpi
int main(int argc, char **argv){
    
    int rank, size;
    int tag = 0;
    int token;
    MPI_Status status;
    MPI_Datatype diagonal;
    int **matriz;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    matriz = criaMatriz(size, rank);
    
    MPI_Type_vector(size*size, 1, size+1, MPI_INT, &diagonal);
    MPI_Type_commit(&diagonal);
    
    MPI_Send(matriz, 1, diagonal, 0, tag, MPI_COMM_WORLD);
    
    
    MPI_Type_free(&diagonal);
    MPI_Finalize();
    return 0;   
}
