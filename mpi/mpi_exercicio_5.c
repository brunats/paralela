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

duplaMat criaMatriz(int n){
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
int main(int argc, char **argv){
  int rank, size;
  double **matriz_A;
  double **matriz_B;
  int i, j;
  duplaMat dp;
  MPI_Status status;

  if(argc!=2) {
    printf("Erro args\n");
    exit(0);
  }else{
    int n=atoi(argv[1]);
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  dp = criaMatriz(n);

  matriz_A = dp.matA;
  matriz_B = dp.matB;

  if(rank == 0) {
		strcpy(msg,"Hello World!\n");
		for(i=1;i<size;i++) {
			//printf("rank[0] enviando para [%d]\n", i);
			MPI_Send(dp,1,MPI_CHAR,i,tag, MPI_COMM_WORLD);
		}
		for(i=1;i<size;i++) {
			MPI_Recv(recvmsg,12,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&status);
			printf("rank[0] recebendo de [%d]\n", status.MPI_SOURCE);
		}

	} else {
		//printf("rank[%d] esperando de rank[0]\n", rank);
		MPI_Recv(recvmsg,12,MPI_CHAR,0,tag, MPI_COMM_WORLD, &status);
		//printf("rank[%d] recebeu: [%s]\n", rank, recvmsg);
		strcpy (msg, recvmsg);
		//printf("rank(%d) retorna msg p/ rank[0]\n", rank);
		MPI_Send(msg,12,MPI_CHAR,status.MPI_SOURCE, tag, MPI_COMM_WORLD);
  }


  MPI_Finalize();

  return 0;
}
