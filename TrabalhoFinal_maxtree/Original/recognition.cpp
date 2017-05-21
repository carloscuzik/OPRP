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

// Imagem searchShape(MaxTree *mt, int dmin, int dmax, int amin, int amax, bool (*Detecta)(const Imagem& f, int dmin, int dmax)) {
// 	int i, j, area, areaccaux;
// 	Grafo *aux = mt->grafo;
// 	Imagem g    = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
// 	Imagem cc   = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
// 	PilhaNo *lifo = NULL;
// 	
// 	lifo = lifo->empilha(aux);
// 	while (lifo) {
// 		lifo = lifo->desempilha(&aux);
// 		if (aux->isOn()) {
// 			cc.zera(); mt->retornaRegiao(aux->getNivel(), aux->getRotulo(), &cc);
// 			area = imArea(cc);
// 			if (area >= amin && area <= amax) {
// 			        //cc = imBuraco(cc); 
// 				if (Detecta(cc,dmin,dmax)) { 
// 					cc = imMultiplica(cc, aux->getNivel());
// 					g = imUniao(g, cc);
// 				}
// 				else
// 					aux->setOff();
// 			}
// 			else
// 				aux->setOff();
// 		}
// 		lifo = lifo->empilhaFilhos(aux);
// 		if (aux->filho != NULL)
// 			aux = aux->filho;
// 	}
// 	return g;
// }

Imagem searchShape(MaxTree *mt, int dmin, int dmax, int amin, int amax, bool (*Detecta)(const Imagem& f, int dmin, int dmax)) {
	int i, j, area, areaccaux, cont=0, teste =0;
	Grafo *aux = mt->grafo;
	Imagem g    = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
	Imagem cc   = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, INTEIRA);  // imagem com zeros
	PilhaNo *lifo = NULL;
	
	lifo = lifo->empilha(aux);

	while (lifo) {
		cont++;
		lifo = lifo->desempilha(&aux);
		if (aux->isOn()) {
			cc.zera(); mt->retornaRegiao(aux->getNivel(), aux->getRotulo(), &cc);
			area = imArea(cc);
			if (area >= amin && area <= amax) {
			        //cc = imBuraco(cc); 
				if (Detecta(cc,dmin,dmax)) { 
					teste++;
					cc = imMultiplica(cc, aux->getNivel());
					g = imUniao(g, cc);
				}
				else
					aux->setOff();
			}
			else
				aux->setOff();
		}
		lifo = lifo->empilhaFilhos(aux);
		if (aux->filho != NULL)
			aux = aux->filho;
	}
	printf("Qtde nos: %d\n", cont);
	printf("Qtde deteccao: %d", teste);
	//cout << "Total nós: ";
	//cout << cont;
	return g;
}
