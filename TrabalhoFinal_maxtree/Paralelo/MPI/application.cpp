#include "maxtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char **argv) {
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	struct timeval ini, final;
    double tempo_real;
	char *arq = "imgs/asp150.pgm";
	int number_threads = size;
	if(argc == 2){
		arq = argv[2];
	}
	FILE* out;
	out = fopen("result.txt","a");
	grava_n_threads(number_threads);
	Imagem f = imLe(arq);
	int N = f.altura * f.largura;
    gettimeofday(&ini, NULL);
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
	
	searchShape((void *)rank);

	g = imNormaliza(g,0,255);
	g.comoCinza();
	gettimeofday(&final, NULL);
	tempo_real = (1 * (final.tv_sec - ini.tv_sec) + (final.tv_usec - ini.tv_usec) / 1000000.0);

	if(rank==0){
	    
		 g.escreve("imgs/out.pgm");
	    if(tempo_real<100){
	    	printf("  ");
	    }else if(tempo_real<10){
	    	printf(" ");
	    }
	    printf("%.4lf ",tempo_real);
	    fprintf(out,"%lf ",tempo_real);
	}
	MPI_Finalize();
	return 0;
}