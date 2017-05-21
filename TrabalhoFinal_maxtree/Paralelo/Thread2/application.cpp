#include "maxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char **argv) {
	struct timeval ini, final;
    double tempo_real;
	char *arq = "imgs/asp150.pgm";
	int number_threads;
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
	// if(strcmp(arq,"imgs/asp150.pgm")==0){
		// out = fopen("result.txt","w");
	// }else{
		out = fopen("result.txt","a");
	// }
	grava_n_threads(number_threads);
	Imagem f = imLe(arq);
	int N = f.altura * f.largura;
	// printf("(%d, %d) pixels=%d\n", f.altura, f.largura, f.altura*f.largura);
    gettimeofday(&ini, NULL);
	// clock_t inicio = clock();
	MaxTree *mt = img2tree(f, eeCaixa());


	Grafo *aux = mt->grafo;
	PilhaNo *lifo_aux = NULL;
	PilhaNo *lifo[number_threads];
	int i, cont =0, k=0;
	for(i=0;i<number_threads;i++){
		lifo[i] = NULL;
	}

	lifo_aux = lifo_aux->empilha(aux);
	
	while(lifo_aux){
		cont++;
		if(k==number_threads){
			k=0;
		}
		lifo_aux = lifo_aux->desempilha(&aux);
		Grafo *aux2 = aux;
		*aux2 = *aux;
		lifo[k] = lifo[k]->empilha(aux2);
		lifo_aux = lifo_aux->empilhaFilhos(aux);
		if (aux->filho != NULL){
			aux = aux->filho;
		}
		k++;
	}



	Imagem g = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);
	passe_de_paremetros(mt, -1, 20, 50, N, &g, lifo);
	
	if(number_threads==1){
		searchShape((void *)0);
	}else{
		pthread_t cons[number_threads]; // cria as threads
		int rc;
		for(i=0;i<number_threads;i++){
			rc = pthread_create(&cons[i], NULL, searchShape,(void *)i);
		}
		for(i=0;i<number_threads;i++){
			rc = pthread_join(cons[i], NULL);
		}
	}
	g = imNormaliza(g,0,255);
	g.comoCinza();
	g.escreve("imgs/out.pgm");
	gettimeofday(&final, NULL);
	// clock_t fim = clock();
	// printf("Qtde nos: %d\n", cont);
	// imprime_numero_de_nos_detectados();	
	// printf("\nTempo total errado: %f\n", (fim-inicio)/(CLOCKS_PER_SEC/1.0));
	tempo_real = (1 * (final.tv_sec - ini.tv_sec) + (final.tv_usec - ini.tv_usec) / 1000000.0);
    // printf("tempo: %lf segundos\n",tempo_real);
    if(tempo_real<100){
    	printf("  ");
    }else if(tempo_real<10){
    	printf(" ");
    }
    printf("%.4lf ",tempo_real);
    fprintf(out,"%lf ",tempo_real);
	return 0;
}