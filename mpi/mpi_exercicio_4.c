#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/*
compile
$ mpicc -Wall mpi_exercicio_4.c -o mpi_exercicio_4
$ mpirun -machinefile maquinas.txt -np 10 mpi_exercicio_4
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
    int i, j;
    MPI_Status status;
    int **matriz;
    int *vetTrabs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank != 0){
        vetTrabs = (int*)malloc((size)*sizeof(int));
        matriz = criaMatriz(size, rank);
        // na mão
        for (i=0; i<size; i++){
            vetTrabs[i] = matriz[i][i];
        }
        MPI_Send(vetTrabs, size, MPI_INT, 0, tag, MPI_COMM_WORLD);

        // com vector
        // MPI_Send(matriz, 1, diagonal, 0, tag, MPI_COMM_WORLD);
    }else{
        int **novamartiz = criaMatriz(size, rank);
        //int *vet = (int*)malloc((size-1)*sizeof(int));
        
        matriz = criaMatriz(size, rank);
        for(i=0;i<size;i++){
            novamartiz[0][i] = matriz[i][i];
        }
        
        for(i=1;i<size;i++){
            MPI_Recv(&novamartiz[i][0], size, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
        }
        printf("NOVA MATRIZ::::\n");
        for (i=0; i<size; i++){
            for (j=0; j<size; j++){
                printf("%d ", novamartiz[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
