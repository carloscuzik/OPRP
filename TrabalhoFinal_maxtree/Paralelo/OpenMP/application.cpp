#include "maxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char **argv) {
	struct timeval ini, final;
    double tempo_real;
    gettimeofday(&ini, NULL);
	char *arq = "imgs/asp150.pgm";
	int number_threads, i;
	if(argc!=2 && argc!=3){
		printf("Falta algum arguento ai FERA ;-)\n");
		return -1;
	}
	if(argc == 2){
		number_threads = atoi(argv[1]);
	}
	if(argc == 3){
		number_threads = atoi(argv[1]);
		arq = argv[2];
	}
	FILE* out;
	out = fopen("result.txt","a");
	grava_n_threads(number_threads);
	Imagem f = imLe(arq);
	int N = f.altura * f.largura;
	Imagem g = imZeros(f.altura, f.largura, 1, INTEIRA);
	passe_de_paremetros(-1, 20, 50, N, &g, &f);
	
	if(number_threads==1){
		searchShape((void *)0);
	}else{
		#pragma omp parallel for num_threads(number_threads)
		for(i=0;i<number_threads;i++){
			// printf("%i\n",i);
			searchShape((void *)i);
		}
	}
	g = imNormaliza(g,0,255);
	g.comoCinza();
	g.escreve("imgs/out.pgm");
	gettimeofday(&final, NULL);
	tempo_real = (1 * (final.tv_sec - ini.tv_sec) + (final.tv_usec - ini.tv_usec) / 1000000.0);
    if(tempo_real>=1000){
    	printf("");
    }else if(tempo_real>=100){
    	printf(" ");
    }else if(tempo_real>=10){
    	printf("  ");
    }else{
    	printf("   ");
    }
    printf("%.4lf ",tempo_real);
    fprintf(out,"%lf ",tempo_real);
	return 0;
}