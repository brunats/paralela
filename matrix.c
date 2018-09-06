#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "matrix.h"

matrix_t *matrix_create_block(int rows, int cols)
{
    //printf("Please someone implement this function!\n");
    matrix_t *temp = NULL;
    double **dtemp = NULL, *bloco = NULL;
    int i;

    temp = (matrix_t*)malloc(sizeof(matrix_t));

    temp->rows = rows;
    temp->cols = cols;

    dtemp = (double**)malloc(rows*sizeof(double*));
    bloco = (double*)malloc(rows*cols*sizeof(double));
    //dtemp[0] = bloco;
    for(i=0;i<rows;i++){
        dtemp[i] = (bloco + i*cols);
    }
    temp->data = dtemp;

    return temp;
}

matrix_t *matrix_create_pointers(int rows, int cols)
{
    //printf("Please someone implement this function!\n");
    matrix_t *temp = NULL;
    double **dtemp = NULL;
    int i;

    temp = (matrix_t*)malloc(sizeof(matrix_t));

    temp->rows = rows;
    temp->cols = cols;

    dtemp = (double**)malloc(rows*sizeof(double*));
    for(i = 0;i < rows; i++){
        dtemp[i] = (double*)malloc(cols*sizeof(double));
    }
    temp->data = dtemp;

    return temp;
}

void matrix_destroy_pointers(matrix_t *m)
{
    //printf("Please someone implement this function!\n");
    int i;

    for(i=0;i<m->rows;i++){
        free(m->data[i]);
    }
    free(m->data);
    free(m);
    m = NULL;

    return;
}

void matrix_destroy_block(matrix_t *m)
{
    //printf("Please someone implement this function!\n");

    free(m->data[0]);
    free(m->data);
    free(m);
    m = NULL;

    return;
}

void matrix_randfill(matrix_t *m)
{
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->data[i][j] = random();
      }
   }
}

void matrix_fill(matrix_t *m, double val)
{
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         m->data[i][j] = val;
      }
   }
}

void matrix_multiply(int iniA, int fimA, matrix_t *A, matrix_t *B, matrix_t *m)
{
    int i, j, k;
    double soma;

    for(i=iniA;i< fimA;i++){
        for(j=0;j< m->cols;j++){
            soma = 0.0;
            for(k=0;k< A->cols;k++){
                soma += A->data[i][k]*B->data[k][j];
            }
            m->data[i][j] = soma;
        }
    }

}

void *matrix_multiply_PARALELA(void *args)
{

    DadosThread *p = (DadosThread *) args;
    matrix_multiply(p->iniA[0], p->fimA[0], p->A, p->B, p->C);
    return NULL;
}

void matrix_multi_PARALELA_INI(matrix_t *a, matrix_t *b, matrix_t *c, int num_threads)
{
  DadosThread *dt = NULL;
  pthread_t *threads = NULL;
  int bloco, i;

  if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }

  if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }

  bloco = a->rows/num_threads;
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

  for (i = 0; i < num_threads; i++) {
	    pthread_join(threads[i], NULL);
	}
  free(dt);
  free(threads);
}

void matrix_print(matrix_t *m)
{

   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         printf("%.17f ", m->data[i][j]);
      }
      printf("\n");
   }
   fflush(stdout);
}

int matrix_equal(matrix_t*A, matrix_t *B)
{
   int i, j;

   if (A->rows != B->rows || A->cols != B->cols)
      return 0;

   for (i = 0; i < A->rows; i++) {
      for (j = 0; j < A->cols; j++) {
         if (A->data[i][j] != B->data[i][j])
            return 0;
      }
   }
   return 1;
}
/*
matrix_t *matrix_sum(matrix_t *A, matrix_t *B, matrix_t *(*p) (int, int))
{
    //printf("Please someone implement this function!\n");
    matrix_t *s;
    int i, j;

    s = (*p)(A->rows, A->cols);

    for (i = 0; i < A->rows; i++){
        for(j = 0; j < A->cols; j++){
            s->data[i][j] = A->data[i][j]+B->data[i][j];
        }
    }
    return s;
}
*/

void matrix_sum(int *ini, int *fim, matrix_t *A, matrix_t *B, matrix_t *s)
{
    //printf("Please someone implement this function!\n");
    int i, j;

    for (i = ini[0]; i < fim[0]; i++){
        for(j = ini[1]; j < fim[1]; j++){
            s->data[i][j] = A->data[i][j]+B->data[i][j];
        }
    }
    //printf("cc\n");
}

void *matrix_sum_PARALELA(void *args)
{
    DadosThread *p = (DadosThread *) args;

    matrix_sum(p->iniA, p->fimA, p->A, p->B, p->C);
    return NULL;
}

void matrix_sum_PARALELA_INI(matrix_t *a, matrix_t *b, matrix_t *c, int num_threads)
{
  DadosThread *dt = NULL;
  pthread_t *threads = NULL;
  int bloco, i;

  if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }

  if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }

  bloco = a->rows/num_threads;
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
  for (i = 0; i < num_threads; i++) {
	    pthread_join(threads[i], NULL);
	}
  free(dt);
  free(threads);
}

matrix_t *matrix_inversion(matrix_t *A, matrix_t *(*p) (int, int))
{
    //printf("Please someone implement this function!\n");

    matrix_t *inv = NULL, *temp = NULL;
    int i, j, k, l;
    double c;

    temp = (*p)(A->rows, A->cols);
    for(i=0;i< A->rows;i++){
        for(j=0;j< A->cols;j++){
            temp->data[i][j] = A->data[i][j];
        }
    }

    inv = (*p)(A->rows, A->cols);
    for(i=0;i< A->rows;i++){
        for(j=0;j< A->cols;j++){
            inv->data[i][j] = 0.0;
        }
    }
    for(i=0;i< A->rows;i++){
        inv->data[i][i] = 1.0;
    }

    //inicia a inversão Gauss-Jordan
    //cima para baixo
    for(i=0;i< (temp->rows)-1;i++){
        if(temp->data[i][i] == 0.0e0){
            printf("A matriz não é inversível\n");
            return NULL;
        }
        c = temp->data[i][i];
        for(j=0;j< i;j++){
            inv->data[i][j] = (inv->data[i][j])/c;
        }
        for(j=i;j< temp->cols;j++){
            inv->data[i][j] = (inv->data[i][j])/c;
            temp->data[i][j] = (temp->data[i][j])/c;
        }

        for(k=(i+1);k< temp->rows;k++){
            c = (temp->data[k][i])/(temp->data[i][i]);

            for(l=0;l<i;l++){
                inv->data[k][l] = inv->data[k][l] - c*inv->data[i][l];
            }
            for(l=i;l< temp->cols;l++){
                inv->data[k][l] = inv->data[k][l] - c*inv->data[i][l];
                temp->data[k][l] = temp->data[k][l] - c*temp->data[i][l];
            }
        }
    }
    c = temp->data[(temp->rows)-1][(temp->cols)-1];
    temp->data[(temp->rows)-1][(temp->cols)-1] = temp->data[(temp->rows)-1][(temp->cols)-1]/c;
    for(j=0;j< temp->cols;j++){
        inv->data[(temp->rows)-1][j] = (inv->data[(temp->rows)-1][j])/c;
    }


    //baixo para cima
    for(i=(temp->rows)-1;i>0;i--){
        for(k=i-1;k>=0;k--){
            c = temp->data[k][i]; //dividido por 1 sempre
            temp->data[k][i] = temp->data[k][i] - c*(temp->data[i][i]);
            for(j=0;j< inv->cols;j++){
                inv->data[k][j] = inv->data[k][j] - c*inv->data[i][j];
            }
        }
    }

    //matrix_print(temp);
    //printf("\n");

    return inv;
}

void matrix_transpose(int ini, int fim, matrix_t *A, matrix_t *t)
{
    //printf("Please someone implement this function!\n");
    int i, j;

    for(i=ini;i< fim;i++){
        for(j=0;j< A->cols;j++){
            t->data[i][j] = A->data[j][i];
        }
    }
}

void *matrix_transpose_PARALELA(void *args)
{
    DadosThread *p = (DadosThread *) args;
    //printf("bb\n");
    //printf("dados da thread %i:\n\tini[0]:%i,\tfim[0]:%i,\tini[1]:%i, \tfim[1]:%i\n", p->id, p->iniA[0], p->fimA[0], p->iniA[1], p->fimA[1]);
    matrix_transpose(p->iniA[0], p->fimA[0], p->A, p->C);
    return NULL;
}

void matrix_transpo_PARALELA_INI(matrix_t *a, matrix_t *c, int num_threads)
{
  DadosThread *dt = NULL;
  pthread_t *threads = NULL;
  int bloco, i;

  if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }

  if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
    printf("Erro ao alocar memória\n");
    exit(EXIT_FAILURE);
  }

  bloco = a->rows/num_threads;
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

  for (i = 0; i < num_threads; i++) {
	    pthread_join(threads[i], NULL);
	}
  free(dt);
  free(threads);
}

double matrix_determinant(matrix_t *A)
{

    matrix_t *temp = NULL;
    int i, j, k, l;
    double c;
    double det = 1.0;

    if(A->rows != A->cols){
        printf("A matriz não é quadrada\n");
        return -1;
    }
    if(A->rows == 1){
        return A->data[0][0];
    }
    if(A->rows == 2){
        return (A->data[0][0]*A->data[1][1]) - (A->data[0][1]*A->data[1][0]);
    }

    temp = matrix_create_block(A->rows, A->cols);
    for(i=0;i< A->rows;i++){
        for(j=0;j< A->cols;j++){
            temp->data[i][j] = A->data[i][j];
        }
    }

    for(i=0;i< (temp->rows)-1;i++){
        if(temp->data[i][i] == 0.0e0){
            //printf("A matriz não é inversível\n");
            return 0.0e0;
        }

        for(k=(i+1);k< temp->rows;k++){
            c = (temp->data[k][i])/(temp->data[i][i]);
            for(l=i;l< temp->cols;l++){
                temp->data[k][l] = temp->data[k][l] - c*temp->data[i][l];
            }
        }
    }

    for(i=0;i<temp->rows;i++){
        det = det*temp->data[i][i];
    }

    matrix_destroy_block(temp);

    return det;
}

double matrix_determinant_PARALELA(matrix_t *A, int num_threads)
{
    matrix_t *temp = NULL;
    int i, j, k, l;
    int bloco;
    double det = 1.0;
    double c;
    DadosThread *dt = NULL;
    pthread_t *threads = NULL;

    if(A->rows != A->cols){
        printf("A matriz não é quadrada\n");
        return -1;
    }

    //testa casos triviais
    if(A->rows == 1){
        return A->data[0][0];
    }
    if(A->rows == 2){
        return (A->data[0][0]*A->data[1][1]) - (A->data[0][1]*A->data[1][0]);
    }
    //

    //aloca threads e dados thread
    if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}

    if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    //

    temp = matrix_create_block(A->rows, A->cols);
    bloco = A->rows/num_threads;
    for(i=0;i<num_threads-1;i++){
        dt[i].id = i;
        dt[i].A = A;
        dt[i].B = temp;
        dt[i].iniA[0] = i*bloco;
        dt[i].fimA[0] = (i*bloco)+bloco;
        pthread_create(&threads[i], NULL, matrix_copia_block, (void *) (dt + i));
    }
    dt[num_threads-1].id = num_threads-1;
    dt[num_threads-1].A = A;
    dt[num_threads-1].B = temp;
    dt[num_threads-1].iniA[0] = (num_threads-1)*bloco;
    dt[num_threads-1].fimA[0] = A->rows;
    pthread_create(&threads[num_threads-1], NULL, matrix_copia_block, (void *) (dt + num_threads-1));

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    //matrix_print(temp);
    //printf("\n");

    for(i=0;i< (temp->rows)-1;i++){
        if(temp->data[i][i] == 0.0e0){
            //printf("A matriz não é inversível\n");
            return 0.0e0;
        }

        if(num_threads > (temp->rows - i)){
            for(k=(i+1);k< temp->rows;k++){
                c = (temp->data[k][i])/(temp->data[i][i]);
                for(l=i;l< temp->cols;l++){
                    temp->data[k][l] = temp->data[k][l] - c*temp->data[i][l];
                }
            }
        }
        else{
            bloco = (temp->rows - i)/num_threads;
            for(j=0;j<num_threads-1;j++){
                dt[j].id = j;
                dt[j].A = temp;
                dt[j].iniA[0] = i + 1 + j*bloco;
                dt[j].fimA[0] = i + 1 + (j*bloco)+bloco;
                dt[j].iniB[0] = i;
                pthread_create(&threads[j], NULL, matrix_determinant_block, (void *) (dt + j));
            }
            dt[num_threads-1].id = num_threads-1;
            dt[num_threads-1].A = temp;
            dt[num_threads-1].iniA[0] = i + 1 + (num_threads-1)*bloco;
            dt[num_threads-1].fimA[0] = temp->rows;
            dt[num_threads-1].iniB[0] = i;
            pthread_create(&threads[num_threads-1], NULL, matrix_determinant_block, (void *) (dt + num_threads-1));

            for (k = 0; k < num_threads; k++) {
                pthread_join(threads[k], NULL);
            }
        }
    }

    //matrix_print(temp);
    //printf("\n");

    for(i=0;i<temp->rows;i++){
        det = det*temp->data[i][i];
    }

    matrix_destroy_block(temp);

    return det;
}

void *matrix_copia_block(void *args)
{
    int i, j;
    DadosThread *p = (DadosThread *) args;

    for(i=p->iniA[0];i< p->fimA[0];i++){
        for(j=0;j< p->A->cols;j++){
            p->B->data[i][j] = p->A->data[i][j];
        }
    }

    return NULL;
}

void *matrix_determinant_block(void *args)
{
    DadosThread *p = (DadosThread *) args;
    int k, l;
    double c;

    for(k=p->iniA[0];k< p->fimA[0];k++){
        c = (p->A->data[k][p->iniB[0]])/(p->A->data[p->iniB[0]][p->iniB[0]]);
        for(l=p->iniB[0];l< p->A->cols;l++){
            p->A->data[k][l] = p->A->data[k][l] - c*p->A->data[p->iniB[0]][l];
        }
    }

    return NULL;
}

double matrix_determinant_antiga(matrix_t *A)
{
    double det;

    if(A->rows != A->cols){
        printf("A matriz não é quadrada\n");
        return -1;
    }
    if(A->rows == 1){
        return A->data[0][0];
    }
    det = matrix_determinant_rec(A);

    return det;
}

double matrix_determinant_rec(matrix_t *A)
{

    double det = 0.0;
    int sinal = 1;
    int i;
    matrix_t *sub = NULL;

    if(A->rows == 2){
        return (A->data[0][0]*A->data[1][1]) - (A->data[0][1]*A->data[1][0]);
    }

    for(i=0;i < A->rows;i++){
        sub = submatrix(A, i);
        det += (A->data[0][i])*(matrix_determinant_rec(sub))*sinal;
        sinal = -sinal;
    }

    return det;
}

matrix_t *submatrix(matrix_t *A, int pos)
{

    int i=0, j=0;
    int linha, coluna;
    matrix_t *sub = NULL;

    sub = matrix_create_block((A->rows)-1, (A->cols)-1);

    for(linha=1;linha < A->rows;linha++){
        for(coluna=0;coluna < A->cols;coluna++){
            if(coluna != pos){
                sub->data[i][j] = A->data[linha][coluna];
                j++;
                if(j == sub->cols){
                    j = 0;
                    i++;
                    break;
                }
            }
        }
    }

    return sub;
}
