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


	struct timeval ini, final;
    double tempo_real;
	char *arq = "imgs/asp150.pgm";
	if(argc == 2){
		arq = argv[1];
	}
	FILE* out;
	out = fopen("result.txt","a");
	Imagem f = imLe(arq);
	int N = f.altura * f.largura;
    gettimeofday(&ini, NULL);
	MaxTree *mt = img2tree(f, eeCaixa());
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int number_threads = size;

	grava_n_threads(number_threads);

	Grafo *aux = mt->grafo;
	PilhaNo *lifo_aux = NULL;
	PilhaNo *lifo[number_threads];
	int i, j, cont =0, k=0;
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

	// MPI_Datatype datatype;
	// MPI_Type_contiguous(sizeof(Imagem), MPI_BYTE, &datatype);
	// MPI_Type_commit(&datatype);


	if(rank==0){
		Imagem g_recebido = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);
		//espera as outras thread
		for(i=1;i<size;i++){
			MPI_Recv(&g_recebido.bits,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(g_recebido.formato,3,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(&g_recebido.altura,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(&g_recebido.largura,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Recv(&g_recebido.profundidade,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			int altura = g_recebido.altura;
			int largura = g_recebido.largura;
			for(j=0;j<altura;j++){
				for(k=0;k<largura;k++){
					MPI_Recv(&g_recebido.matriz[j][k],1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				}
			}
			g = imUniao(g,g_recebido);
		}
		g = imNormaliza(g,0,255);
		g.comoCinza();
		g.escreve("imgs/out.pgm");
		
		gettimeofday(&final, NULL);
		tempo_real = (1 * (final.tv_sec - ini.tv_sec) + (final.tv_usec - ini.tv_usec) / 1000000.0);
	    if(tempo_real<100){
	    	printf("  ");
	    }else if(tempo_real<10){
	    	printf(" ");
	    }
	    printf("%.4lf ",tempo_real);
	    fprintf(out,"%lf ",tempo_real);
	}else{
		MPI_Send(&g.bits,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(g.formato,3,MPI_CHAR,0,0,MPI_COMM_WORLD);
		MPI_Send(&g.altura,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(&g.largura,1,MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(&g.profundidade,1,MPI_INT,0,0,MPI_COMM_WORLD);
		int altura = g.altura;
		int largura = g.largura;
		for(i=0;i<altura;i++){
			for(j=0;j<largura;j++){
				MPI_Send(&g.matriz[i][j],1,MPI_INT,0,0,MPI_COMM_WORLD);
			}
			// MPI_Send(g.matriz[j],largura,MPI_INT,0,0,MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}