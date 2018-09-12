#include <complex>
#include <iostream>
#include <omp.h>
#include <sys/time.h>
#include <math.h>

using namespace std;

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

double f_Mandelbrot(int max_row, int max_column, int max_n){
  double start_time, end_time, final_time;
  start_time = wtime();

	char **mat = (char**)malloc(sizeof(char*)*max_row);

	#pragma omp parallel for num_threads(8)
	for (int i=0; i<max_row;i++)
		mat[i]=(char*)malloc(sizeof(char)*max_column);

	//omp_set_nested(1);
	#pragma omp parallel for schedule(guided) num_threads(8)
	for(int r = 0; r < max_row; ++r){
		//#pragma omp parallel for schedule(guided) num_threads(8)
		for(int c = 0; c < max_column; ++c){
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}

	for(int r = 0; r < max_row; ++r){
		for(int c = 0; c < max_column; ++c)
			std::cout << mat[r][c];
		cout << '\n';
	}
	end_time = wtime();
	final_time = ((double)(end_time - start_time));
	return final_time;
}

int main(){
	int i, q_exec;
	double matT[10], total, media, desviopadrao, somatorio, mul;

  int max_row, max_column, max_n;
	cin >> max_row;
	cin >> max_column;
	cin >> max_n;

	total = 0;
  q_exec = 10;
  somatorio = 0;

	for (i=0; i<q_exec; i++){
        matT[i] = f_Mandelbrot(max_row, max_column, max_n);
        printf("%i - Exec\tTime:%fs\n",i+1, matT[i]);
        total += matT[i];
	}
	media = total/q_exec;
	for (i=0; i<q_exec; i++){
        mul = (matT[i] - media);
        somatorio += pow(mul, 2);
	}
  somatorio = somatorio/q_exec;
  desviopadrao = sqrt(somatorio);
  printf("\n\nMedia: %f\nDesvio Padrao: %f",media, desviopadrao);
}
