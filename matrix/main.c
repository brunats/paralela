#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "matrix.h"

/*
Compile:
  $ make clean
  $ make
  $ for j in `seq 1 10`; do for i in `seq 1 10`; do ./main 3 3 $j 1; done; done
*/
/*
DETERMINANTE:
for j in `seq 1 10`; do for i in `seq 1 10`; do ./main 3000 3000 $j 1; done; done
SOMA:
for j in `seq 1 10`; do for i in `seq 1 10`; do ./main 18000 18000 $j 2; done; done
MULTIPLICACAO:
for j in `seq 1 10`; do for i in `seq 1 10`; do ./main 2000 2000 $j 3; done; done
TRANSPOSTA:
for j in `seq 1 10`; do for i in `seq 1 10`; do ./main 20000 20000 $j 4; done; done
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

   if ((argc != 5)) {
      printf("Uso: %s <rows> <cols> <n_threads>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nrows = atoi(argv[1]);
   ncols = atoi(argv[2]);
   num_threads = atoi(argv[3]);
   op = atoi(argv[4]);

   printf("num_threads = %d\n", num_threads);
   //printf("Opcao:\n\t-1: Determinante\n\t-2: Soma\n\t-3: Multiplicacao\n\t-4: Transposta\n:");
   //scanf("%d", &op);

<<<<<<< HEAD:matrix/main.c
   switch (op){
     case 1:{
       //ini_det--------------------------------
       start_time = wtime();
       a = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
       det = matrix_determinant_PARALELA(a, num_threads);
       //printf("Determinante: %lf\n", det);
       //printf("\n");
       matrix_destroy_block(a);
       end_time = wtime();
=======
   start_time = wtime();
   a = matrix_create_block(nrows, ncols);
   matrix_randfill(a);
   switch (op){
     case 1:{
       //ini_det--------------------------------
       det = matrix_determinant_PARALELA(a, num_threads);
       //printf("Determinante: %lf\n", det);
       //printf("\n");
>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
       //fim_det--------------------------------
       break;
     }
     case 2:{
<<<<<<< HEAD:matrix/main.c
       //ini_sum-------------------------------
       start_time = wtime();
       a = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
=======
       //ini_sum--------------------------------
>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
       b = matrix_create_block(nrows, ncols);
       c = matrix_create_block(nrows, ncols);
       matrix_randfill(b);
       matrix_sum_PARALELA_INI(a, b, c, num_threads);

       //printf("\n");
       //matrix_print(a);
       //printf("\n");
       //matrix_print(b);
       //printf("\n");
       //matrix_print(c);

       matrix_destroy_block(b);
       matrix_destroy_block(c);
       matrix_destroy_block(a);
       end_time = wtime();
       break;
       //fim_sum--------------------------------
     }
     case 3:{
       //ini_multi--------------------------------
<<<<<<< HEAD:matrix/main.c
       start_time = wtime();
       a = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
=======
>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
       b = matrix_create_block(nrows, ncols);
       c = matrix_create_block(nrows, ncols);
       matrix_randfill(b);
       matrix_multi_PARALELA_INI(a, b, c, num_threads);

       //printf("\n");
       //matrix_print(a);
       //printf("\n");
       //matrix_print(b);
       //printf("\n");
       //matrix_print(c);
<<<<<<< HEAD:matrix/main.c

=======
       
>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
       matrix_destroy_block(b);
       matrix_destroy_block(c);
       matrix_destroy_block(a);
       end_time = wtime();
       //fim_multi--------------------------------
       break;
     }
     case 4:{
       //ini_transpo--------------------------------
<<<<<<< HEAD:matrix/main.c
       start_time = wtime();
       a = matrix_create_block(nrows, ncols);
       matrix_randfill(a);
=======
>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
       c = matrix_create_block(nrows, ncols);
       matrix_transpo_PARALELA_INI(a, c, num_threads);

       //printf("\n");
       //matrix_print(a);
       //printf("\n");
       //matrix_print(c);
<<<<<<< HEAD:matrix/main.c

=======
       
>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
       matrix_destroy_block(c);
       matrix_destroy_block(a);
       end_time = wtime();
       //fim_transpo--------------------------------
       break;
     }
     default:{
       printf("nada nada nada\n");
       break;
     }
   }
<<<<<<< HEAD:matrix/main.c
=======
   matrix_destroy_block(a);
   end_time = wtime();

>>>>>>> 72a8b42e0a03f184186b426568b8881f78d03f2c:main.c
   printf("%d %d %f\n", nrows, ncols, end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
