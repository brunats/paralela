#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "matrix.h"

/*
Compile:
  $ make clean
  $ make
  $ for i in `seq 1 10`; do ./main 3 3 3; done
*/

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

int main(int argc, char **argv)
{
   double start_time, end_time, det;
   int nrows, ncols, i, op;
   int num_threads = 0;
   matrix_t *a, *b, *c;

   if ((argc != 4)) {
      printf("Uso: %s <rows> <cols> <n_threads>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nrows = atoi(argv[1]);
   ncols = atoi(argv[2]);
   num_threads = atoi(argv[3]);

   printf("num_threads = %d\n", num_threads);
   printf("Opcao:\n\t-1: Determinante\n\t-2: Soma\n\t-3: Multiplicacao\n\t-4: Transposta\n:");
   scanf("%d", &op);

   start_time = wtime();
   switch (op){
     case 1:{
       //ini_det--------------------------------
       a = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
       det = matrix_determinant_PARALELA(a, num_threads);
       //printf("Determinante: %lf\n", det);
       //printf("\n");
       matrix_destroy_block(a);
       //fim_det--------------------------------
       break;
     }
     case 2:{
       //ini_sum--------------------------------
       a = matrix_create_block(nrows, ncols);
       b = matrix_create_block(nrows, ncols);
       c = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
       matrix_randfill(b);
       matrix_sum_PARALELA_INI(a, b, c, num_threads);

       //printf("\n");
       //matrix_print(a);
       //printf("\n");
       //matrix_print(b);
       //printf("\n");
       //matrix_print(c);

       matrix_destroy_block(a);
       matrix_destroy_block(b);
       matrix_destroy_block(c);
       break;
       //fim_sum--------------------------------
     }
     case 3:{
       //ini_multi--------------------------------
       a = matrix_create_block(nrows, ncols);
       b = matrix_create_block(nrows, ncols);
       c = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
       matrix_randfill(b);
       matrix_multi_PARALELA_INI(a, b, c, num_threads);

       //printf("\n");
       //matrix_print(a);
       //printf("\n");
       //matrix_print(b);
       //printf("\n");
       //matrix_print(c);

       matrix_destroy_block(a);
       matrix_destroy_block(b);
       matrix_destroy_block(c);
       //fim_multi--------------------------------
       break;
     }
     case 4:{
       //ini_transpo--------------------------------
       a = matrix_create_block(nrows, ncols);
       c = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
       matrix_transpo_PARALELA_INI(a, c, num_threads);

       //printf("\n");
       //matrix_print(a);
       //printf("\n");
       //matrix_print(c);

       matrix_destroy_block(a);
       matrix_destroy_block(c);
       //fim_transpo--------------------------------
       break;
     }
     default:{
       printf("nada nada nada\n");
     }
   }
   end_time = wtime();

   printf("%d %d %f\n", nrows, ncols, end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
