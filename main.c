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
   int nrows, ncols;
   int num_threads = 0;
   int i;
   matrix_t *a, *b, *c;

   if ((argc != 4)) {
      printf("Uso: %s <rows> <cols> <n_threads>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nrows = atoi(argv[1]);
   ncols = atoi(argv[2]);
   num_threads = atoi(argv[3]);


   printf("num_threads = %d\n", num_threads);


   start_time = wtime();
   //inicio
   /*
   DadosThread *dt = NULL;
   pthread_t *threads = NULL;

	if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}

   if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
   }
   */
   //create matrix a, b, c
   a = matrix_create_block(nrows, ncols);
   b = matrix_create_block(nrows, ncols);
   c = matrix_create_block(nrows, ncols);
   matrix_randfill(a);
   matrix_randfill(b);
/*
   int bloco;
   bloco = a->rows/num_threads;
   //printf("bloco: %i\n",bloco );
   //printf("c");

*/
    /*
    //inicio soma
    for (i = 0; i < num_threads-1; i++) {
        dt[i].id = i;
        dt[i].A = a;
        dt[i].B = b;
        dt[i].C = c;
        dt[i].iniA[0] = i*bloco;
        dt[i].iniA[1] = 0;
        dt[i].iniB[0] = i*bloco;
        dt[i].iniB[1] = 0;
        dt[i].fimA[0] = (dt[i].id*bloco)+bloco;
        dt[i].fimA[1] = a->cols;
        dt[i].fimB[0] = (dt[i].id*bloco)+bloco;
        dt[i].fimB[1] = b->cols;
        printf("a");
        pthread_create(&threads[i], NULL, matrix_sum_PARALELA, (void *) (dt + i));
     }
    dt[i].id = num_threads-1;
    dt[i].A = a;
    dt[i].B = b;
    dt[i].C = c;
    dt[i].iniA[0] = (num_threads-1)*bloco;
    dt[i].iniA[1] = 0;
    dt[i].iniB[0] = (num_threads-1)*bloco;
    dt[i].iniB[1] = 0;
    dt[i].fimA[0] = a->rows;
    dt[i].fimA[1] = a->cols;
    dt[i].fimB[0] = b->rows;
    dt[i].fimB[1] = b->cols;
    pthread_create(&threads[num_threads-1], NULL, matrix_sum_PARALELA, (void *) (dt + num_threads-1));
    //fim soma


    //inicio multiplicação
    for (i = 0; i < num_threads-1; i++) {
        dt[i].id = i;
        dt[i].A = a;
        dt[i].B = b;
        dt[i].C = c;
        dt[i].iniA[0] = i*bloco;
        dt[i].iniA[1] = 0;
        dt[i].iniB[0] = i*bloco;
        dt[i].iniB[1] = 0;
        dt[i].fimA[0] = (dt[i].id*bloco)+bloco;
        dt[i].fimA[1] = a->cols;
        dt[i].fimB[0] = (dt[i].id*bloco)+bloco;
        dt[i].fimB[1] = b->cols;
        printf("a");
        pthread_create(&threads[i], NULL, matrix_multiply_PARALELA, (void *) (dt + i));
     }
    dt[i].id = num_threads-1;
    dt[i].A = a;
    dt[i].B = b;
    dt[i].C = c;
    dt[i].iniA[0] = (num_threads-1)*bloco;
    dt[i].iniA[1] = 0;
    dt[i].iniB[0] = (num_threads-1)*bloco;
    dt[i].iniB[1] = 0;
    dt[i].fimA[0] = a->rows;
    dt[i].fimA[1] = a->cols;
    dt[i].fimB[0] = b->rows;
    dt[i].fimB[1] = b->cols;
    pthread_create(&threads[num_threads-1], NULL, matrix_multiply_PARALELA, (void *) (dt + num_threads-1));
    //fim multiplicação

	for (i = 0; i < num_threads; i++) {
        //printf("mandando rodar?\n");
	    pthread_join(threads[i], NULL);
	}

    matrix_print(a);
    printf("\n");
    matrix_print(b);
    printf("\n");
    matrix_print(c);

	free(dt);
	free(threads);
  */
  /*
   //inicio transposta
   for (i = 0; i < num_threads-1; i++) {
       dt[i].id = i;
       dt[i].A = a;
       dt[i].B = NULL;
       dt[i].C = c;
       dt[i].iniA[0] = i*bloco;
       dt[i].iniA[1] = 0;
       dt[i].iniB[0] = -1;
       dt[i].iniB[1] = -1;
       dt[i].fimA[0] = (dt[i].id*bloco)+bloco;
       dt[i].fimA[1] = a->cols;
       dt[i].fimB[0] = -1;
       dt[i].fimB[1] = -1;
       pthread_create(&threads[i], NULL, matrix_transpose_PARALELA, (void *) (dt + i));
    }
   dt[i].id = num_threads-1;
   dt[i].A = a;
   dt[i].B = NULL;
   dt[i].C = c;
   dt[i].iniA[0] = (num_threads-1)*bloco;
   dt[i].iniA[1] = 0;
   dt[i].iniB[0] = -1;
   dt[i].iniB[1] = -1;
   dt[i].fimA[0] = a->rows;
   dt[i].fimA[1] = a->cols;
   dt[i].fimB[0] = -1;
   dt[i].fimB[1] = -1;
   pthread_create(&threads[num_threads-1], NULL, matrix_transpose_PARALELA, (void *) (dt + num_threads-1));
   //fim multiplicação

 for (i = 0; i < num_threads; i++) {
       //printf("mandando rodar?\n");
     pthread_join(threads[i], NULL);
 }
   //matrix_print(a);
   //printf("\n");
   //matrix_print(c);
 free(dt);
 free(threads);
 */



   //det--------------------------------
   /*
   a = matrix_create_block(nrows, ncols);
   matrix_randfill(a);
   det = matrix_determinant_PARALELA(a, num_threads);
   printf("Determinante: %lf\n", det);
   printf("\n");

   matrix_destroy_block(a);

   //fim
   */

   //sum--------------------------------
   a = matrix_create_block(nrows, ncols);
   b = matrix_create_block(nrows, ncols);
   c = matrix_create_block(nrows, ncols);
   matrix_randfill(a);
   matrix_randfill(b);
   matrix_sum_PARALELA_INI(a, b, c, num_threads);

   /*
   printf("\n");
   matrix_print(a);
   printf("\n");
   matrix_print(b);
   printf("\n");
   matrix_print(c);
   */

   matrix_destroy_block(a);
   matrix_destroy_block(b);
   matrix_destroy_block(c);

   //fim

   end_time = wtime();

   printf("%d %d %f\n", nrows, ncols, end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
