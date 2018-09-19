// ./copyright
//
// INTEL CONFIDENTIAL
//
// Copyright 2011 Intel Corporation All Rights Reserved.
//
// The source code contained or described herein and all documents related to the
// source code ("Material") are owned by Intel Corporation or its suppliers
// or licensors. Title to the Material remains with Intel Corporation or its suppliers
// and licensors. The Material contains trade secrets and proprietary and confidential
// information of Intel or its suppliers and licensors. The Material is protected by
// worldwide copyright and trade secret laws and treaty provisions. No part of the
// Material may be used, copied, reproduced, modified, published, uploaded, posted,
// transmitted, distributed, or disclosed in any way without Intel.s prior express
// written permission.
//
// No license under any patent, copyright, trade secret or other intellectual property
// right is granted to or conferred upon you by disclosure or delivery of the Materials,
// either expressly, by implication, inducement, estoppel or otherwise. Any license under
// such intellectual property rights must be express and approved by Intel in writing.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "black-scholes.h"
#include <iostream>
#include <omp.h>
#include <sys/time.h>
using namespace std;

inline float RandFloat(float low, float high) {
	float t = (float) rand() / (float) RAND_MAX;
	return (1.0f - t) * low + t * high;
}

double wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

int main(int argc, char* argv[]) {
	FPARRAY CallResultParallel, CallConfidence, StockPrice, OptionStrike,
			OptionYears;

	int i, mem_size;

	int OPT_N;
	scanf("%d",&OPT_N);


	mem_size = sizeof(float) * OPT_N;

	CallResultParallel.SPData = (float *) malloc(mem_size);
	CallConfidence.SPData = (float *) malloc(mem_size);
	StockPrice.SPData = (float *) malloc(mem_size);
	OptionStrike.SPData = (float *) malloc(mem_size);
	OptionYears.SPData = (float *) malloc(mem_size);

	for (i = 0; i < OPT_N; i++) {
		CallResultParallel.SPData[i] = 0.0;
		CallConfidence.SPData[i] = -1.0;
		StockPrice.SPData[i] = RandFloat(5.0f, 50.0f);
		OptionStrike.SPData[i] = RandFloat(10.0f, 25.0f);
		OptionYears.SPData[i] = RandFloat(1.0f, 5.0f);
	}
	/* QTD threads AQUI*/
	int n_threads = 17;
	int j, q_exec;

	int l_threads[n_threads];
	double matT[10], total, media, desviopadrao, somatorio, mul;

	for (i = 0; i < n_threads; i++)
		l_threads[i] = i+1;

  //fscanf(stdin,"%d",&n);
  printf("n_threads\tmedia\t\tdesvio\n");
  for (j = 0; j < n_threads; j++){
    total = 0;
    somatorio = 0;
    q_exec = 10;
		//printf("oi\n");
    printf("%i\t", l_threads[j]);
    for (i = 0; i < q_exec; i++){
					//printf("ola\n");
          double start_time, end_time, final_time;
          start_time = wtime();
					MonteCarlo(CallResultParallel.SPData, CallConfidence.SPData, StockPrice.SPData, OptionStrike.SPData, OptionYears.SPData, OPT_N, l_threads[j]);
					end_time = wtime();
        	final_time = ((double)(end_time - start_time));
          matT[i] = final_time;
          //printf("%i - Exec\tTime:%fs\n",i+1, matT[i]);
          total += matT[i];
          //printf("thread %i - exec %i - t %f\n",l_threads[j], i, matT[i]);
					// results in CallResultParallel.SPData[i]
					/*
					for (i = 0; i < OPT_N; i++)
						printf("%5.2f\n", CallResultParallel.SPData[i]);
					*/
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



	free(CallResultParallel.SPData);
	free(CallConfidence.SPData);
	free(StockPrice.SPData);
	free(OptionStrike.SPData);
	free(OptionYears.SPData);
	return 0;
}
