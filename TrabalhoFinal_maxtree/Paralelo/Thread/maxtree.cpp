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


MaxTree *img2tree(const Imagem& f, const Estruturante& se) {
	// clock_t inicio = clock();	
	// printf("Construindo a max-tree... ");

	MaxTree *mt = new MaxTree(se.img->matriz, se.img->altura, se.img->largura, f);

	// printf("Ok!\n");
	// clock_t fim = clock();
	// printf("Tempo max-tree: %f\n", (fim-inicio)/(1.*CLK_TCK));
	return mt;
}


Imagem tree2img(MaxTree *mt) {
	Grafo *aux = mt->grafo;
	int nNos=0, nNosOn=0;
	Imagem g = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, CINZA);  // imagem com zeros
	PilhaNo *lifo = NULL;
	lifo = lifo->empilha(aux);
	while (lifo) {
		lifo = lifo->desempilha(&aux);
		nNos++;
		if (aux->isOn()) {
			nNosOn++;
			mt->retornaRegiao(aux->getNivel(), aux->getRotulo(), &g); //maximo entre as duas imagens
		}
		lifo = lifo->empilhaFilhos(aux);
	}
	printf("Estatistica: %d nos, sendo %d descartados.\n", nNos, nNos-nNosOn);
	return g;
}




Imagem tree2aux(MaxTree *mt) {
	Grafo *aux = mt->grafo;
	int i, j, nNos=0, nNosOn=0;
	Imagem g = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, CINZA);  // imagem com zeros
	Imagem x = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, CINZA);  // imagem com zeros
	PilhaNo *lifo = NULL;
	lifo = lifo->empilha(aux);
	while (lifo) {
		lifo = lifo->desempilha(&aux);
		nNos++;
		if (aux->isOn()) {
			nNosOn++;
			for (i=0; i<mt->retornaAltura(); i++) for (j=0; j<mt->retornaLargura(); j++) x.matriz[i][j] = 0;
			mt->retornaRegiao(aux->getNivel(), aux->getRotulo(), &x); //maximo entre as duas imagens
			x = imMultiplica(x,imCentroide(x));
			//x = imGradiente(x,eeCruz(0),eeCaixa());
			g = imUniao(g,x);
		}
		lifo = lifo->empilhaFilhos(aux);
	}
	printf("Estatistica: %d nos, sendo %d descartados.\n", nNos, nNos-nNosOn);
	g = imDilata(g, eeCaixa(10));
	return g;
}




Imagem marker2region(MaxTree *mt, Pontos pt) {  //regiao a partir de marcadores
	int nivel;
	int rotulo;
	Grafo *aux;
	Imagem  g = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, CINZA);  // imagem com zeros
	Imagem cc = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, CINZA);  // imagem com zeros
	PilhaNo *lifo = NULL;
	for (int i = 0; i < pt.tam; i++) {
		nivel = mt->retornaPixel (pt, i);
		rotulo= mt->retornaStatus(pt, i);
		printf("i=%d, nivel=%d, rotulo=%d\n", i, nivel, rotulo);
		aux  = mt->grafo;
		lifo = lifo->empilha(aux);
		while (lifo) {
			lifo = lifo->desempilha(&aux);
			if (aux->isOn() && nivel==aux->getNivel() && rotulo==aux->getRotulo()) {
				mt->retornaRegiao(aux->getNivel(), aux->getRotulo(), &cc);
				g  = imUniao(g, cc); //maximo entre as duas imagens
	
			}
			lifo = lifo->empilhaFilhos(aux);
			if (aux->filho != NULL)
				aux = aux->filho;
		}
	}
	return g;
}
