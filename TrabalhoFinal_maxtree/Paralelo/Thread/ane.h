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

 /* ARVORE N-ARIA DE BUSCA POR ESPALHAMENTO */

//#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "maxtree.h"


#define SUCESSO  1
#define FRACASSO 0
#define SIM      1
#define NAO      0

#define ORDEM  100



class Grafo;



/* Estrutura de um registro de dados */
typedef struct {
    int chave;  /* rotulo */
    Grafo *no;
    /*
    Colocar outros campos aqui
    de acordo com a aplicacao
    */
} Registro;


/* Estrutura de um noh */
typedef struct NoANE {
	Registro info;
	struct NoANE *filho[ORDEM];
} NoANE, *pNoANE;


/* Descritor */
typedef struct {
	pNoANE raiz;
} ANE, *pANE, **ppANE;



/* Alocacao do objeto ANE */
pANE criaANE();

/* Desalocacao do objeto ANE */
void destroiABB(ppANE pp);


/* Alocacao de um noh de dados */
pNoANE criaNoANE(Registro x);


/* Verifica se estah vazia */
int ehVaziaANE(pANE pd);


/* INSERCAO */
int insereANE(pANE pd, Registro x, int *comparacoes);

/* REMOCAO */
int removeANE(pANE pd, int chave);

/* BUSCA */
int buscaANE(pANE pd, int chave, Registro *px, int *comparacoes);

/* PERCURSO */
void percursoANE(pNoANE p);
