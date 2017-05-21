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
#include <time.h>


int main(int argc, char **argv) {
	char *arq = "imgs/asp75.pgm";
	if (argv[1])
		arq = argv[1];
	Imagem f = imLe(arq);
	int N = f.altura * f.largura;

	//f = imHmax(f, 30); f.escreve("imgs/tmp.pgm");
	printf("(%d, %d) pixels=%d\n", f.altura, f.largura, f.altura*f.largura);
	clock_t inicio = clock();
	MaxTree *mt = img2tree(f, eeCaixa());
	//Imagem g = searchShape(mt, -1, 3, 50, N, isCircle);
	Imagem g = searchShape(mt, -1, 20, 50, N, isCircle);
	//g = tree2img(mt);
	g = imNormaliza(g,0,255); g.comoCinza();
	g.escreve("imgs/out.pgm");
	clock_t fim = clock();printf("\nTempo total: %f\n", (fim-inicio)/(1.*CLK_TCK));return 0;
	
	return 0;
}

