#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/*
compile
$ mpicc -Wall mpi_exercicio_4.c -o mpi_exercicio_4
$ mpi_exercicio_4
*/

typedef struct duplaMat{
  double ** matA;
  double ** matB;
} duplaMat;

duplaMat criaMatriz(int n, ){
  double **m_A = NULL;
  double **m_B = NULL;
  double *dados_A = NULL;
  double *dados_B = NULL;
  double i, j;

  duplaMat dp;

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
      m_A[i][j] = i*j;
      m_B[i][j] = 1/(i*j);
    }
  }
  dp.matA = m_A;
  dp.matB = m_B
  return dp;
}

//Material: https://github.com/leorrodrigues/mpi
void master(int n){
  int rank, size;
  int tag = 0;
  int token;
  MPI_Status status;
  MPI_Datatype diagonal;
  double **matriz_A;
  double **matriz_B;
  int i, j;
  duplaMat dp;


  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  dp = criaMatriz(size, rank);
  matriz_A = dp.matA;
  matriz_B = dp.matB;

  MPI_Type_vector(size*size, 1, size+1, MPI_INT, &diagonal);
  MPI_Type_commit(&diagonal);

  MPI_Send(matriz, 1, diagonal, 0, tag, MPI_COMM_WORLD);
  printf("oi\n");
  MPI_Type_free(&diagonal);
  MPI_Finalize();
}

int main(int argc, char **argv){
  int rank,size;
  int **matriz_A, **matriz_B;

	if(argc!=2) {
    printf("Erro args\n");
    exit(0);
  }
  int n=atoi(argv[1]);

  MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("Worker %d/%d started\n", rank+1, size);

  if(size<2) {
		printf("Deve possuir no minimo 2 processos\n");
	}

  matriz_A, matriz_B = criaMatriz(int n)

	if (rank == 0) {
		master (n);
	} else {
		slave (rank, &);
	}
	MPI_Finalize();

  return 0;
}
