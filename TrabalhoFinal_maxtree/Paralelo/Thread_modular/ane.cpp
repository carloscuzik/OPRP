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
#include "maxtree.h"


/****************************************/
/*  criaANE                             */
/****************************************/
pANE criaANE() {
	pANE pd;
	pd = (pANE) malloc(sizeof(ANE));
	if (pd != NULL)
		pd->raiz = NULL;
	return pd;
}


/****************************************/
/*  criaNoANE                           */
/****************************************/
pNoANE criaNoANE(Registro x) {
	int i;
	pNoANE p;
	p = (pNoANE) malloc(sizeof(NoANE));
	if (p != NULL) {
		p->info = x;
		for (i=0; i < ORDEM; i++)
			p->filho[i] = NULL;
	}
	return p;
}


/****************************************/
/*  ehVaziaANE                          */
/****************************************/
int ehVaziaANE(pANE pd) {
	return (pd->raiz == NULL);
}


/****************************************/
/*  insereANE                           */
/****************************************/
int insereANE(pANE pd, Registro x, int *comparacoes) {
	int chave, divisor, resto, CONTINUA = SIM, nivel = 0;
	pNoANE p = pd->raiz, ppai = NULL;
	if (ehVaziaANE(pd)) {
		pd->raiz = criaNoANE(x);
		return SUCESSO;
	}
	while (p != NULL  &&  p->info.chave != x.chave) {
		(*comparacoes)++;
		ppai = p;
		chave = x.chave;
		for (divisor = ORDEM + nivel;   CONTINUA  &&  divisor >= ORDEM;   divisor--) {
			resto = chave % divisor;
			if (resto < ORDEM)
				CONTINUA = NAO;
			else
				//chave = resto;
				chave ++;
		}
		CONTINUA = SIM;
		p = p->filho[resto];
		nivel++;
	}
	if (p != NULL) /* chave jah existente */
		return FRACASSO;
	ppai->filho[resto] = criaNoANE(x);
	return SUCESSO;
}


/****************************************/
/*  percursoANE                         */
/****************************************/
void percursoANE(pNoANE p) {
	int i;
	if (p != NULL) {
		printf("%d ", p->info.chave);
		for (i=0; i < ORDEM; i++) {
			percursoANE(p->filho[i]);
		}
	}
}


/****************************************/
/*  buscaANE                            */
/****************************************/
int buscaANE(pANE pd, int x, Registro *r, int *comparacoes) {
	int chave, divisor, resto, CONTINUA = SIM, nivel = 0;
	pNoANE p = pd->raiz, ppai = NULL;
	if (ehVaziaANE(pd))
		return FRACASSO;
	while (p != NULL  &&  p->info.chave != x) {
		(*comparacoes)++;
		ppai = p;
		chave = x;
		for (divisor = ORDEM + nivel;   CONTINUA  &&  divisor >= ORDEM;   divisor--) {
			resto = chave % divisor;
			if (resto < ORDEM)
				CONTINUA = NAO;
			else
				//chave = resto;
				chave++;
		}
		CONTINUA = SIM;
		p = p->filho[resto];
		nivel++;
	}
	if (p != NULL) { /* encontrou a chave (o rotulo) */
		*r = p->info;
		return SUCESSO;
	}
	return FRACASSO;
}
