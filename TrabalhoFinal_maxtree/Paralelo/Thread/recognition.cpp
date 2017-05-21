/***************************************************************************
 *   Copyright (C) 2006 by Alexandre Gonçalves Silva                       *
 *   alexandre@joinville.udesc.br                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>



#include "maxtree.h"


int sqr(int x) {
	return x*x;
}

int n_of_threads;


/***************************************************************************/
/* CIRCULOS                                                                */
/***************************************************************************/

void *asd(void *args){
	
} 

bool isCircle(const Imagem& f, int dmin, int dmax) {
	//dmax - tolerancia maxima entre raios  (dmin nao utilizado)
	int xc, yc;
	int x , y;
	int dx, dy;
	int dist, k = 0, soma = 0, medio = 0;
	
	//dmin = dmin * dmin;
	dmax = dmax * dmax;

	Imagem aux = imGradiente(f, eeCruz(0), eeCruz(1));
	Pontos ind = imNaozero(aux);

	if (ind.tam != 0) {
		Pontos cent = imNaozero(imCentroide(aux));
		xc = cent.getX(0);
		yc = cent.getY(0);
		for (int i=0; i < ind.tam; i++) {
			dx = (ind.getX(i) - xc);
			dy = (ind.getY(i) - yc);
			dist = dx*dx + dy*dy;
			k++;
			soma = soma + dist;
			medio = soma / k;
			if (abs(dist-medio) > dmax){
				return false;
			}
		}
		return true;
	}
	return false;
}



/***************************************************************************/
/* LINHAS                                                                  */
/***************************************************************************/

bool isLine(const Imagem& f, int dmin, int dmax) {
	//dmin e dmax - sao respectivamente espessura minima e maxima
	Imagem d = imDistancia(f);
	int dist = imMaximo(d);
	if (dist >= dmin && dist <= dmax){
		return true;
	}
	return false;
}
/* LINHAS 2 - a entrada jah eh a transformada distancia                    */
bool isLine2(const Imagem& d, int dmin, int dmax) {
	//d, dmin e dmax - sao respectivamente transformad distancia, espessura minima e maxima
	int dist = imMaximo(d);
	if (dist >= dmin && dist <= dmax){
		return true;
	}
	return false;
}

int teste = 0;
PilhaNo **lifo;
Infos_cons infos;

Imagem searchShape(MaxTree *mt, int dmin, int dmax, int amin, int amax, bool (*Detecta)(const Imagem& f, int dmin, int dmax)) {
	clock_t inicio = clock();
	int i, j, area, areaccaux, cont=0;
	Grafo *aux = mt->grafo;
	Imagem g = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);
	
	PilhaNo *lifo_aux = NULL;
	lifo_aux = lifo_aux->empilha(aux);
	int number_of_threads = n_of_threads;
	PilhaNo *a[number_of_threads];
	lifo = a;
	int k=0;
	for(i=0;i<number_of_threads;i++){
		lifo[i]=NULL;
	}
	//coloca todos os nos da arvore em uma pilha
	while(lifo_aux){
		if(k==number_of_threads){
			k=0;
		}
		cont++;
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
	//armazena informações para o consumidor
	infos.dmin = dmin;
	infos.dmax = dmax;
	infos.amin = amin;
	infos.amax = amax;
	infos.Detecta = Detecta;
	infos.mt = mt;
	infos.g = &g;
	// começa a inicializar as threads
	pthread_t cons[number_of_threads]; // cria as threads
	int rc;
	for(i=0;i<number_of_threads;i++){
		rc = pthread_create(&cons[i], NULL, consumidor,(void *)(intptr_t)i);
	}
	for(i=0;i<number_of_threads;i++){
		rc = pthread_join(cons[i], NULL);
	}

	// single core >>
	// int asd;
	// consumidor((void*)asd);
	clock_t fim = clock();
	printf("\nTempo searchShape: %f\n", (fim-inicio)/(1.*CLK_TCK));
	printf("Qtde nos: %d\n", cont);
	printf("Qtde deteccao: %d\n", teste);
	return g;
}

pthread_mutex_t mtx_des = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx2 = PTHREAD_MUTEX_INITIALIZER;

void *consumidor(void *a){
	

	int dmin = infos.dmin;
	int dmax = infos.dmax;
	int amin = infos.amin;
	int amax = infos.amax;
	bool (*Detecta)(const Imagem& f, int dmin, int dmax);
	Detecta = infos.Detecta;
	MaxTree *mt = infos.mt;
	*mt = *(infos.mt);
	int k = (intptr_t) a;
	
	int area;
	int detec_pos = 0;
	Grafo *aux;
	Imagem g_local = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);
	Imagem cc = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
	// clock_t inicio, fim;
	// double t_des = 0;
	while(lifo[k]){
		lifo[k] = lifo[k]->desempilha(&(aux));
		cc.zera();
		int a = aux->getNivel();
		int b = aux->getRotulo();
		mt->retornaRegiao(a, b, &cc);
		// t_des += (fim-inicio)/(1.*CLK_TCK);
		area = imArea(cc);
		// inicio = clock();
		if(area >= amin && area <= amax){
			if(isCircle(cc,dmin,dmax)){
				detec_pos++;
				cc = imMultiplica(cc, aux->getNivel());
				g_local = imUniao(g_local,cc);
			}
		}
		// fim = clock();
	}
	// printf("Tempo thread %i retReg: %f\n", k, t_des);
	// printf("Tempo thread %i uni: %f\n", k, t_uni);
	// inicio = clock();
	pthread_mutex_lock(&mtx1);
		teste += detec_pos;
	pthread_mutex_unlock(&mtx1);
	pthread_mutex_lock(&mtx2);
		*(infos.g) = imUniao(*(infos.g),g_local);
	pthread_mutex_unlock(&mtx2);
	// fim = clock();
	// printf("Tempo thread %i c: %f\n", k, (fim-inicio)/(1.*CLK_TCK));
}

void grava_n_threads(int n_thread){
	n_of_threads = n_thread;
}