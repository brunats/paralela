#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/*
compile
$ mpicc -Wall mpi_exercicio_4.c -o mpi_exercicio_4
$ mpi_exercicio_4
*/
int **criaMatriz(int n, int rank){
    int **m = NULL;
    int *dados = NULL;
    int i, j;

    m = (int**)malloc(n*sizeof(int*));
    dados = (int*)malloc(n*n*sizeof(int));

    for(i=0;i<n;i++){
        m[i] = (dados + i*n);
    }
    printf("ola\n");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            m[i][j] = (j*10 + i*3)%(rank*2 + 1);
            printf("%i\n", m[i][j]);
        }
        printf("\n");
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
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("size: %i, rank: %i\n", size, rank);
    matriz = criaMatriz(size, rank);
    printf("size: %i, rank: %i\n  git status", size, rank);
    for(i=0;i<size;i++){
        for(j=0;j<rank;j++){
            printf("%i\n",matriz[i][j]);
        }
    }

    MPI_Type_vector(size*size, 1, size+1, MPI_INT, &diagonal);
    MPI_Type_commit(&diagonal);

    MPI_Send(matriz, 1, diagonal, 0, tag, MPI_COMM_WORLD);
    printf("oi\n");
    MPI_Type_free(&diagonal);
    MPI_Finalize();
    return 0;
}
