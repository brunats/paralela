#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

#define TAM 2048

/*
compile
$ mpicc -Wall MPI_multiplicacao.c -o mult
$ mpirun -machinefile maquinas_f306.txt -np 10 mult
$ for i in `seq 1 10`; do mpirun -machinefile maquinas_f306.txt -np 16 mult; done
*/
typedef struct matrizes{
    double **m_A;
    double **m_B;
    double **m_C;
}matrizes;

void printaMatriz(double **m, int n){
    int i, j;
    for (i=0; i<n; i++){
        for (j=0; j<n; j++){
            printf("%6.2lf ", m[i][j]);
        }
        printf("\n");
    }
}

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

matrizes criaMatriz(int n){
  double **m_A = NULL;
  double **m_B = NULL;
  double **m_C = NULL;
  double *dados_A = NULL;
  double *dados_B = NULL;
  double *dados_C = NULL;
  int i, j;
  matrizes mat;

  m_A = (double**)malloc(n*sizeof(double*));
  m_B = (double**)malloc(n*sizeof(double*));
  m_C = (double**)malloc(n*sizeof(double*));
  dados_A = (double*)malloc(n*n*sizeof(double));
  dados_B = (double*)malloc(n*n*sizeof(double));
  dados_C = (double*)malloc(n*n*sizeof(double));

  for(i=0;i<n;i++){
    m_A[i] = (dados_A + i*n);
    m_B[i] = (dados_B + i*n);
    m_C[i] = (dados_C + i*n);
  }

  for(i=0;i<n;i++){
      for(j=0;j<n;j++){
          m_A[i][j] = (i+1.0)*(j+1.0);
          m_B[i][j] = 1.0/((i+1.0)*(j+1.0));
          m_C[i][j] = 0.0;
      }
  }

  mat.m_A = m_A;
  mat.m_B = m_B;
  mat.m_C = m_C;
  return mat;
}

//Material: https://github.com/leorrodrigues/mpi

int main(int argc, char **argv){

    int rank, size;
    int tag = 0;
    MPI_Status status;
    int bloco;
    
    int i, j, k;
    double soma;
    matrizes ma;
    double start_time, end_time;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    bloco = TAM/size;
    if(rank == 0){
        double **matriz_A;
        double **matriz_B;
        double **matriz_C;
        
        ma = criaMatriz(TAM);
        matriz_A = ma.m_A;
        matriz_B = ma.m_B;
        matriz_C = ma.m_C;
        
        //printf("Matriz A\n");
        //printaMatriz(matriz_A, TAM);
        //printf("Matriz B\n");
        //printaMatriz(matriz_B, TAM);
        //printf("Matriz C\n");
        //printaMatriz(matriz_C, TAM);
        
        start_time = wtime();
        //mestre envia
        for(i=1;i<size;i++){
            MPI_Send(matriz_B[0], (TAM)*(TAM), MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            MPI_Send(matriz_A[i*bloco], TAM*bloco, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
        
        //mestre calcula primeira linha
        for(i=0;i<bloco;i++){
            for(j=0;j<TAM;j++){
                soma = 0.0;
                for(k=0; k<TAM; k++)
                    soma += (matriz_A[i][k]*matriz_B[k][j]);
                matriz_C[i][j] = soma;
            }
        }
        
        //mestre recebe
        for(i=1;i<size;i++){
            MPI_Recv(&matriz_C[i*bloco][0], TAM*bloco, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
        }
        end_time = wtime();
        printf ("%f\n", end_time - start_time);
        //printf("Matriz C\n");
        //printaMatriz(matriz_C, TAM);
        
    
    }else{
        double **mB = (double**)malloc((TAM)*(TAM)*sizeof(double*));
        double *dadosB = (double*)malloc((TAM)*(TAM)*sizeof(double));
        double *dadosA = (double*)malloc((TAM*bloco)*sizeof(double));
        double *dadosC = (double*)malloc((TAM*bloco)*sizeof(double));
        double **linhasA = (double**)malloc((bloco)*sizeof(double*));
        double **linhasC = (double**)malloc((bloco)*sizeof(double*));
        
        MPI_Recv(&dadosB[0], (TAM)*(TAM), MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&dadosA[0], TAM*bloco, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        
        for(i=0;i<TAM;i++){
            mB[i] = (dadosB + i*(TAM));
        }
        
        for(i=0;i<bloco;i++){
            linhasA[i] = (dadosA + i*TAM);
            linhasC[i] = (dadosC + i*TAM);
        }
        
        
        for(i=0;i<bloco;i++){
            for(j=0;j<TAM;j++){
                soma = 0.0;
                for(k=0; k<TAM; k++)
                    soma += (linhasA[i][k]*mB[k][j]);
                linhasC[i][j] = soma;
            }
        }
        MPI_Send(linhasC[0], TAM*bloco , MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        
    }
    

    MPI_Finalize();
    return 0;
}
