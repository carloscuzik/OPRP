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


void filterSalientPeaks(MaxTree *mt) {
	//remove folhas sem irmaos enquanto houver
	Folhas  *folhas = mt->retornaFolhas();
	Grafo   *aux;
	while (folhas) {
		aux = folhas->folha;
		while (aux != NULL && aux->pai != NULL && aux->pai->getOrdem() == 1) { //se eh filho unico
			aux->setOff();
			aux = aux->pai;
		}
		folhas = folhas->proximo;
	}
}



void filterWithoutBrothers(MaxTree *mt) {
	Grafo *aux = mt->grafo;
	while (aux) {
		//if (aux->filho != NULL && aux->pai != NULL && aux->pai->getOrdem() == 1) //nao folha, nao raiz, sem irmaos
		if (aux->pai != NULL && aux->pai->getOrdem() == 1) //nao raiz, sem irmaos
			aux->setOff();
		aux = aux->proximo;
	}
}



void filterNodesDegree(MaxTree *mt, int infDegree, int supDegree) {
	Grafo *aux = mt->grafo;
	while (aux) {
		if ( ! (aux->pai != NULL && aux->getOrdem() >= infDegree && aux->getOrdem() <= supDegree) )  //nao raiz, com grau entre [inf,sup]
			aux->setOff();
		aux = aux->proximo;
	}
}



void filterNodesArea(MaxTree *mt, int infArea, int supArea) {
	Grafo *aux = mt->grafo;
	int area;
	int nNos=0, nNosOff=0;
	while (aux) {
		nNos++;
		if (aux->isOn()) {
			area = mt->retornaArea(aux->getNivel(), aux->getRotulo());
			if (area < infArea || area > supArea) {  //nao raiz, com area entre [inf,sup]
				aux->setOff();
				nNosOff++;
			}
		}
		aux = aux->proximo;
	}
	printf("Estatistica: %d nos, sendo %d descartados.\n", nNos, nNosOff);
}
