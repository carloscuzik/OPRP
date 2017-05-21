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


Imagem kmax(MaxTree *mt, int kup, int kdown, int max_dif_level, int amin, int amax) {
	printf("entrando kmax\n");
	int i, j, level_pre, level_pos;
	bool CONT;
	Folhas *leaves = mt->retornaFolhas();
	Folhas *leaf   = leaves;
	Grafo  *no     = NULL, *aux = NULL;
	Imagem g = imZeros(mt->retornaAltura(), mt->retornaLargura(), 1, CINZA);  // imagem com zeros
	
	Grafo *x = mt->grafo;
	while (x) {
		x->setOff();
		x = x->proximo;
	}
	
	while (leaf) {
		no = leaf->folha;
		for (i=0; i<kup; i++) {
			level_pre = no->getNivel();
			if (no->pai != NULL)
				no = no->pai;
			else
				level_pre = 0;
			if (no->pai == NULL)
				level_pos = 0;
			else
				level_pos = no->getNivel();
			if (abs(level_pre-level_pos) > max_dif_level) {
				no = NULL;
				break;
			}
		}
		if (no->pai != NULL && no != NULL) {
			aux = no;
			CONT = true;
			for (j=0; j<kdown; j++) {
				aux = aux->pai;
				if (aux->pai == NULL) {
					CONT = false;
					break;
				}
			}
			if (CONT) {
				no->setOn();
				mt->retornaRegiao(no->getNivel(), no->getRotulo(), &g);
			}
		}
		leaf = leaf->proximo;
	}
	printf("saindo kmax\n");
	return g;
}

