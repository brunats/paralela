#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/*
compile
$ mpicc -Wall mpi_exercicio_4.c -o mpi_exercicio_4
$ mpi_exercicio_4
*/
double **criaMatriz(int n, int rank){
  double **m_A = NULL;
  double **m_B = NULL;
  double *dados_A = NULL;
  double *dados_B = NULL;
  double i, j;

  m_A = (double**)malloc(n*sizeof(double*));
  m_B = (double**)malloc(n*sizeof(double*));
  dados = (double*)malloc(n*n*sizeof(double));
  dados_B = (double*)malloc(n*n*sizeof(double));

  for(i=0;i<n;i++){
    m_A[i] = (dados_A + i*n);
    m_B[i] = (dados_B + i*n);
  }
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      m_A[i][j] = i*j
      m_A[i][j] = 1/(i*j)
    }
  }
  return m_A, m_B;
}

//Material: https://github.com/leorrodrigues/mpi

int main(int argc, char **argv){

    int rank, size;
    int tag = 0;
    int token;
    MPI_Status status;
    MPI_Datatype diagonal;
    double **matriz_A;
    double **matriz_B;
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    matriz_A, matriz_B = criaMatriz(size, rank);

    MPI_Type_vector(size*size, 1, size+1, MPI_INT, &diagonal);
    MPI_Type_commit(&diagonal);

    MPI_Send(matriz, 1, diagonal, 0, tag, MPI_COMM_WORLD);
    printf("oi\n");
    MPI_Type_free(&diagonal);
    MPI_Finalize();
    return 0;
}
