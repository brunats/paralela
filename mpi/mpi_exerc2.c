#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

void rand_fill(double** x, int n, int m){
  int i, j;
  for(i = 0; i < n; i++)
    for(j = 0; j < m; j++){
      x[i][j] = (i+1)*(j+1);
    }
  for(i = 0; i < n; i++)
    x[i][i] *= 100;
}

double** allocaMatriz(int x, int y){
	double **m;
	double *dados;
	int i;
	
	m = (double**)malloc(x*sizeof(double*));
	dados = (double*)malloc(y*x*sizeof(double));
	
	for(i=0;i<x;i++){
		m[i] = dados + i*y;
	}
	
	rand_fill(m, x, y);
	
	return m;
}

int main(int argc, char **argv){
    
    int rank, size;
    int tag = 0;
    MPI_Status status;
    double **m;
    int i;
    int x = 10, y = 30;
    double soma = 0.0;
    
    m = allocaMatriz(x, y);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if(rank == 0){
    	
    	int fim;
    	if(x<size-1){
    		fim = x+1;
    	}
    	else{
    		fim = size;
    	}
    	for(i = 1;i < fim;i++){
    		MPI_Send(m, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
    	}
    	
        
        //MPI_Recv(&token, 1, MPI_INT, size-1, tag, MPI_COMM_WORLD, &status);
        //printf("token: %d\n", token);
    }
    else{
        
        if(rank-1 < x){
        	MPI_Recv(m, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        	double soma = 0.0;
        	for(){
        		
        	}
        }
        
        
        
        /*
        MPI_Recv(m, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        
        int inicio, fim, passo;
        
        passo = x/(size-1);
        if(passo == 0){
        	passo = 1;
        }
        inicio = passo*rank;
        fim = passo*rank + passo;
        
        token += rank;
        if(rank==size-1){
            MPI_Send(&token, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
            printf("%d enviando %d para %d\n", rank, token, 0);
        }
        else{
            MPI_Send(&token, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
            printf("%d enviando %d para %d\n", rank, token, rank+1);
        }
        */
    }
    
    MPI_Finalize();
    return 0;
    
}
