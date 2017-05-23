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

#include "maxtree.h"


int sqr(int x) {
	return x*x;
}

int number_of_threads;
void grava_n_threads(int n){
	number_of_threads = n;
}


/***************************************************************************/
/* CIRCULOS                                                                */
/***************************************************************************/

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
			if (abs(dist-medio) > dmax)
				return false;
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
	if (dist >= dmin && dist <= dmax)
		return true;
	return false;
}
/* LINHAS 2 - a entrada jah eh a transformada distancia                    */
bool isLine2(const Imagem& d, int dmin, int dmax) {
	//d, dmin e dmax - sao respectivamente transformad distancia, espessura minima e maxima
	int dist = imMaximo(d);
	if (dist >= dmin && dist <= dmax)
		return true;
	return false;
}



/***************************************************************************/
/* ANALISE DE FORMA                                                        */
/***************************************************************************/

int dmin;
int dmax;
int amin;
int amax;
MaxTree *mt;
Imagem *g;
PilhaNo **lifo;
int nos_detectados_totais;

void passe_de_paremetros(MaxTree *mt1, int dmin1, int dmax1, int amin1, int amax1, Imagem *g_l, PilhaNo **lifo_l){
	dmin = dmin1;
	dmax = dmax1;
	amin = amin1;
	amax = amax1;
	mt = mt1;
	g = g_l;
	lifo = lifo_l;
	nos_detectados_totais = 0;
}

void *searchShape(void *arg){
	int numero_da_thread_atal = (int&) arg;
	
	// printf("processo: %i\n",numero_da_thread_atal);
	int i, area, areaccaux, nos_detectados_locais=0;
	Grafo *aux = mt->grafo;
	Imagem g_l   = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
	Imagem cc   = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
	while (lifo[numero_da_thread_atal]) {
		lifo[numero_da_thread_atal] = lifo[numero_da_thread_atal]->desempilha(&aux);
		cc.zera();
		mt->retornaRegiao(aux->getNivel(), aux->getRotulo(), &cc);
		area = imArea(cc);
		if (area >= amin && area <= amax) {
		    if (isCircle(cc,dmin,dmax)) {
				nos_detectados_locais++;
				cc = imMultiplica(cc, aux->getNivel());
				g_l = imUniao(g_l, cc);
			}
		}
	}
	nos_detectados_totais += nos_detectados_locais;
	*(g) = imUniao(*g,g_l);
}

void imprime_numero_de_nos_detectados(){
	printf("Qtde deteccao: %i\n", nos_detectados_totais);
}	


void *consumidor(void *arg){}