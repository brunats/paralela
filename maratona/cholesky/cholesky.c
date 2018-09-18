/*****************************************************
 * Site: https://rosettacode.org/wiki/Cholesky_decomposition
 *****************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include <sys/time.h>

void cholesky(double** A, int n, int nt) {
  #pragma omp parallel for num_threads(nt)
  for (int i = 0; i < n; i++)
    for (int j = 0; j < (i + 1); j++) {
      double s = 0;
      for (int k = 0; k < j; k++)
        s += A[i][k] * A[j][k];
      A[i][j] = (i == j) ? sqrt(A[i][i] - s) : (1.0 / A[j][j] * (A[i][j] - s));
    }
}

void show_matrix(double** A, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
        printf("%2.5f ", A[i][j]);
    printf("\n");
  }
}

void mat_zero(double** x, int n) {
  int i, j;
  for(i=0; i<n; i++) {
    for(j=0; j<n; j++) {
      x[i][j] = 0.0;
    }
  }
}

double** mat_new(int n) {
  int i;
  double** x = malloc(sizeof(double*) * n);
  assert(x != NULL);

  for(i = 0; i < n; i++){
    x[i]  = malloc(sizeof(double) * n);
    assert(x[i] != NULL);
  }

  mat_zero(x,n);

  return x;
}

void rand_fill(double** x, int n){
  int i, j;
  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++){
      x[i][j] = (i+1)*(j+1);
    }
  for(i = 0; i < n; i++)
    x[i][i] *= 100;
}


void mat_gen(double** s, int n) {
  int i,j;
  for(i=0; i<n; i++) {
    for(j=0; j<n; j++) {
      scanf("%lf",&s[i][j]);
    }
  }
}

void mat_del(double** x) {
  free(x[0]);
  free(x);
}

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

int main() {
  int n;
  int i, j, q_exec;
  int l_threads[17] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  int n_threads = 17;
	double matT[10], total, media, desviopadrao, somatorio, mul;


  // testar com n = 5000
  n = 2000;
  //fscanf(stdin,"%d",&n);
  printf("n_threads\tmedia\t\tdesvio\n");
  for (j = 0; j < n_threads; j++){
    total = 0;
    somatorio = 0;
    q_exec = 10;
    printf("%i\t", l_threads[j]);
    for (i = 0; i < q_exec; i++){
          double start_time, end_time, final_time;
          start_time = wtime();
          double** A = mat_new(n);
          rand_fill(A, n);

          cholesky(A, n, l_threads[j]);
          //show_matrix(A, n);

          mat_del(A);
          end_time = wtime();
        	final_time = ((double)(end_time - start_time));
          matT[i] = final_time;
          //printf("%i - Exec\tTime:%fs\n",i+1, matT[i]);
          total += matT[i];
          //printf("thread %i - exec %i - t %f\n",l_threads[j], i, matT[i]);
  	}
  	media = total/q_exec;
  	for (i=0; i<q_exec; i++){
          mul = (matT[i] - media);
          somatorio += pow(mul, 2);
  	}
    somatorio = somatorio/q_exec;
    desviopadrao = sqrt(somatorio);
    printf("\t%f\t%f\n",media, desviopadrao);
  }

  return 0;
}
