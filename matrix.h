#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>

#define random() ((rand() ^ rand()) / (RAND_MAX + 1.0))

typedef struct {
      double **data;
      int rows;
      int cols;
} matrix_t;

typedef struct {
	int id, iniA[2], iniB[2], fimA[2], fimB[2];
	matrix_t *A;
	matrix_t *B;
  matrix_t *C;
} DadosThread;

/*
 * All functions must return a new matriz (when need)
 */


matrix_t *matrix_create_pointers(int rows, int cols);

matrix_t *matrix_create_block(int rows, int cols);

void matrix_destroy_pointers(matrix_t *m);

void matrix_destroy_block(matrix_t *m);

void matrix_randfill(matrix_t *m);

void matrix_fill(matrix_t *m, double val);

//matrix_t *matrix_multiply(matrix_t *A, matrix_t *B, matrix_t *(*p) (int, int));
void matrix_multiply(int iniA, int fimA, matrix_t *A, matrix_t *B, matrix_t *m);

//matrix_t *matrix_sum(matrix_t *A, matrix_t *B, matrix_t *(*p) (int, int));
void matrix_sum(int *ini, int *fim, matrix_t *A, matrix_t *B, matrix_t *s);

matrix_t *matrix_inversion(matrix_t *A, matrix_t *(*p) (int, int));

matrix_t *matrix_transpose(matrix_t *A, matrix_t *(*p) (int, int));

double matrix_determinant(matrix_t *A);

double matrix_determinant_rec(matrix_t *A);

matrix_t *submatrix(matrix_t *A, int pos);

void matrix_print(matrix_t *m);

int matrix_equal(matrix_t*A, matrix_t *B);

// funcoes de trabalho da thread
void *matrix_sum_PARALELA(void *args);

void *matrix_multiply_PARALELA(void *args);

#endif
