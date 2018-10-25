#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/*
compile
$ mpicc -Wall mpi_exercicio_5.c -o mpi_exercicio_5
$ mpirun -machinefile maquinas.txt -np 10 mpi_exercicio_5
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
            printf("%.2lf ", m[i][j]);
        }
        printf("\n");
    }
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
    
    int i, j, k;
    //double soma;
    matrizes ma;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank == 0){
        double **matriz_A;
        double **matriz_B;
        double **matriz_C;
        
        ma = criaMatriz(size-1);
        matriz_A = ma.m_A;
        matriz_B = ma.m_B;
        matriz_C = ma.m_C;
        /*
        printf("Matriz A\n");
        printaMatriz(matriz_A, size-1);
        printf("Matriz B\n");
        printaMatriz(matriz_B, size-1);
        printf("Matriz C\n");
        printaMatriz(matriz_C, size-1); 
        */
        for(i=1;i<size;i++){
            MPI_Send(matriz_B, (size-1)*(size-1), MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            MPI_Send(matriz_A[i-1], size-1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }
        for(i=1;i<size;i++){
            MPI_Recv(&matriz_C[i-1][0], size-1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
        }
        //printf("Matriz C\n");
        //printaMatriz(matriz_C, size);
        
    
    }else{
        double **mB = (double**)malloc((size-1)*(size-1)*sizeof(double*));
        double *dadosB = (double*)malloc((size-1)*(size-1)*sizeof(double));
        double *linhaA = (double*)malloc((size-1)*sizeof(double));
        double *linhaC = (double*)malloc((size-1)*sizeof(double));
        double soma;
        
        MPI_Recv(&dadosB[0], (size-1)*(size-1), MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&linhaA[0], size-1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        
        for(i = 0;i<size-1;i++){
            mB[i] = (dadosB + i*(size-1));
        }
        
        //printf("hello\n");
        //printaMatriz(mB, size-1);
        /* 
        for(i=0;i<size-1;i++){
            printf("%5.2f ", linhaA[i]);
        }
        printf("\n");*/
        
        
        //i = rank-1;
        for(j=0;j<size-1;j++){
            soma = 0.0;
            for(k=0; k<size-1; k++)
                soma += (linhaA[k]*mB[k][j]);
            linhaC[j] = soma;
        }
        MPI_Send(linhaC, size-1 , MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        
    }
    

    MPI_Finalize();
    return 0;
}
