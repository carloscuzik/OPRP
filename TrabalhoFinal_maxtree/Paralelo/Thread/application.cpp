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
	char *arq = "imgs/asp75.pgm";
	int number_threads;
	if(argc == 2){
		number_threads = atoi(argv[1]);
	}
	if(argc == 3){
		number_threads = atoi(argv[1]);
		arq = argv[2];
	}
	grava_n_threads(number_threads);
	Imagem f = imLe(arq);
	int N = f.altura * f.largura;
	printf("(%d, %d) pixels=%d\n", f.altura, f.largura, f.altura*f.largura);
    gettimeofday(&ini, NULL);
	// clock_t inicio = clock();
	MaxTree *mt = img2tree(f, eeCaixa());
	Imagem g = searchShape(mt, -1, 20, 50, N, isCircle);
	g = imNormaliza(g,0,255);
	g.comoCinza();
	g.escreve("imgs/out.pgm");
	// clock_t fim = clock();
	// printf("\nTempo total: %f\n", (fim-inicio)/(1.*CLK_TCK));
	gettimeofday(&final, NULL);
	tempo_real = (1 * (final.tv_sec - ini.tv_sec) + (final.tv_usec - ini.tv_usec) / 1000000.0);
    printf("tempo: %lf segundos\n",tempo_real);
	return 0;
}

