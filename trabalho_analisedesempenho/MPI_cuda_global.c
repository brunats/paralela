#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include<string.h>

#define TAM 2048

/*
compile
mpicc -c MPI_cuda_global.c -o main.o 
nvcc -c CUDA_mpi_global.cu -o multGlobal.o 
mpicc main.o multGlobal.o -lcudart -L/apps/CUA/cuda-5.0/lib64 -o prog 
./mpi_cp_run.sh prog maquinas_cuda.txt
mpirun -machinefile maquinas_cuda.txt -np 2 prog

$ for i in `seq 1 10`; do mpirun -machinefile maquinas_cuda.txt -np 2 prog; done
*/

double *mA, *mB, *mC;
int N, num;

void multiplica();

typedef struct matrizes{
    double **m_A;
    double **m_B;
    double **m_C;
}matrizes;

void printaMatriz(double **m, int n){
    int i, j;
    for (i=0; i<n; i++){
        for (j=0; j<n; j++){
            printf("c[%d] = %lf\n", i*n + j, m[i][j]);
        }
        //printf("\n");
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
          m_A[i][j] = j + i*n;
          m_B[i][j] = j + i*n;
          //m_C[i][j] = 0.0;
      }
  }

  mat.m_A = m_A;
  mat.m_B = m_B;
  mat.m_C = m_C;
  return mat;
}

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
    
    N = TAM;
    num = size;
    //printf("num = %d\n", num);
    
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
            MPI_Send(&matriz_B[0][0], (TAM)*(TAM), MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            MPI_Send(&matriz_A[i*bloco][0], (TAM/num)*TAM, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
        
        //mestre calcula primeira linha
        mA = matriz_A[0];
        mB = matriz_B[0];
        mC = (double*)malloc((TAM/num)*TAM*sizeof(double));
        multiplica();
        /*
        for(i=0;i<TAM*bloco;i++){
            printf("mC[%d] = %lf\n", i, mC[i]);
        }*/
        //matriz_C[0] = mC;
        memcpy(matriz_C[0], mC, (TAM/num)*TAM*sizeof(double));
        
        //mestre recebe
        for(i=1;i<size;i++){
            MPI_Recv(&matriz_C[i*bloco][0], TAM*bloco, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
        }
        end_time = wtime();
        printf ("%f\n", end_time - start_time);
        //printf("Matriz C\n");
        //printaMatriz(matriz_C, TAM);
        
    
    }else{
        //double **mB = (double**)malloc((TAM)*(TAM)*sizeof(double*));
        //double *dadosB = (double*)malloc((TAM)*(TAM)*sizeof(double));
        //double *dadosA = (double*)malloc((TAM*bloco)*sizeof(double));
        //double *dadosC = (double*)malloc((TAM*bloco)*sizeof(double));
        //double **linhasA = (double**)malloc((bloco)*sizeof(double*));
        //double **linhasC = (double**)malloc((bloco)*sizeof(double*));
        mA = (double*)malloc((TAM/num)*TAM*sizeof(double));
        mB = (double*)malloc(TAM*TAM*sizeof(double));
        mC = (double*)malloc((TAM/num)*TAM*sizeof(double));
        
        MPI_Recv(&mB[0], (TAM)*(TAM), MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&mA[0], (TAM/num)*TAM, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        
        /*
        for(i=0;i<TAM*TAM;i++){
            printf("mB[%d] = %lf\n", i, mB[i]);
        }
        for(i=0;i<TAM*bloco;i++){
            printf("mA[%d] = %lf\n", i, mA[i]);
        }
        */
        /*
        for(i=0;i<TAM;i++){
            mB[i] = (dadosB + i*(TAM));
        }
        
        for(i=0;i<bloco;i++){
            linhasA[i] = (dadosA + i*TAM);
            linhasC[i] = (dadosC + i*TAM);
        }
        */
        
        multiplica();
        /*
        for(i=0;i<TAM*bloco;i++){
            printf("mC[%d] = %lf\n", i, mC[i]);
        }
        */
        MPI_Send(&mC[0], TAM*bloco , MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }
    

    MPI_Finalize();
    return 0;
}
