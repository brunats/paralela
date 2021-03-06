/******************************************************************************
* FILE: mm.c
* DESCRIPTION:  
*   Matrix Multiply - C Version
*   Modified from Blaise Barney OpenMP code.
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

#define NRA 2000                 /* number of rows in matrix A */
#define NCA 2000               /* number of columns in matrix A */
#define NCB 2000                  /* number of columns in matrix B */

int main (int argc, char *argv[]) 
{
  int	 i, j, k;
//  double a[NRA][NCA],           /* matrix A to be multiplied */
//	 b[NCA][NCB],           /* matrix B to be multiplied */
//	 c[NRA][NCB];           /* result matrix C */
  double **a, **b, **c;
  
  a = malloc(NRA*sizeof(double*));
  for(i=0;i<NRA;i++){
    a[i] = malloc(NCA*sizeof(double));
  }
  
  b = malloc(NCA*sizeof(double*));
  for(i=0;i<NCA;i++){
    b[i] = malloc(NCB*sizeof(double));
  }
  
  c = malloc(NRA*sizeof(double*));
  for(i=0;i<NRA;i++){
    c[i] = malloc(NCB*sizeof(double));
  }

  #pragma omp parallel private(i, j, k) shared(a, b, c)
  {
      /*** Initialize matrices ***/
      #pragma omp for schedule(dynamic) nowait
      for (i=0; i<NRA; i++)
        for (j=0; j<NCA; j++)
          a[i][j]= i+j;
      
      #pragma omp for schedule(dynamic) nowait
      for (i=0; i<NCA; i++)
        for (j=0; j<NCB; j++)
          b[i][j]= i*j;
      
      #pragma omp for schedule(dynamic) nowait
      for (i=0; i<NRA; i++)
        for (j=0; j<NCB; j++)
          c[i][j]= 0;

      /*** Do matrix multiply ***/
      #pragma omp for schedule(dynamic) nowait
      for (i=0; i<NRA; i++)    
        for(j=0; j<NCB; j++)       
          for (k=0; k<NCA; k++)
            c[i][j] += a[i][k] * b[k][j];
  }
  
  
  /*** Print results ***/
  /*
  printf("******************************************************\n");
  printf("Result Matrix:\n");
  for (i=0; i<NRA; i++)
  {
    for (j=0; j<NCB; j++) 
      printf("%6.2f   ", c[i][j]);
    printf("\n"); 
  }
  printf("******************************************************\n");
  */
  printf ("Done.\n");

}
