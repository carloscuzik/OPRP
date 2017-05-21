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

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/wait.h>
//#include "morph4cpp.h"

#define BINARIA  1
#define CINZA    8
#define INTEIRA 32

#define INFINITO 2147483647


class Pontos {
	private:
		int *a;
		int *b;
		int tam_max;
	public:
		int tam;

		Pontos(const int& tamanho) {
			tam_max = tamanho;
			a = new int[tam_max];
			b = new int[tam_max];
			tam = 0;
		}
		
		void insere(const int& x, const int& y) {
			if (tam == tam_max)
				return;
			a[tam] = x;
			b[tam] = y;
			tam++;
		}

		int getX(const int& pos) {
			return a[pos];
		}

		int getY(const int& pos) {
			return b[pos];
		}
		
		Pontos& operator=(const Pontos& p) {
			for (int k=0; k < p.tam; k++) {
				a[k] = p.a[k];
				b[k] = p.b[k];
			}
			tam_max = p.tam_max;
			tam     = p.tam;
		}
			
		~Pontos() {
		//	if (a != NULL) {
		//		delete a;
		//		delete b;
		//		a = b = NULL;
		//		tam_max = tam = 0;
		//	}
		}
};



 

class Imagem {
	public:
		int   bits; //numero de bits (binaria=1, niveis de cinza=8, ...)
		char  formato[3];
		int   altura;
		int   largura;
		int   profundidade;
		int **matriz;

		// transforma Image do mmorph para Imagem do mm
		/*
		void morph2mm(const Image& img) {
			int i, j, aux;
			altura       = img.height();
			largura      = img.width();
			profundidade = img.depth();
			//--------aloca matriz de pixels--------
			matriz = new int*[altura];
			for (i=0 ; i<altura ; i++)
				matriz[i] = new int[largura];
			//--------------------------------------
			for (i=0; i<altura; i++) {
				for (j=0; j<largura; j++) {
					aux = (int) img.getpixel(j,i,0);
					matriz[i][j] = aux;
				}
			}
			bits = CINZA;
		}
		*/

		// cria imagem com um certo valor
		Imagem(const int& imAltura, const int& imLargura, const int& imProfundidade, const int& valor = 0, const int& tipo = CINZA) {
			int i, j;
			altura       = imAltura;
			largura      = imLargura;
			profundidade = imProfundidade;
			//--------aloca matriz de pixels--------
			matriz = new int*[altura];
			for (i=0; i<altura; i++) {
				matriz[i] = new int[largura];
				for (j=0; j<largura; j++)
					matriz[i][j] = valor;
			}
			//--------------------------------------
			bits = tipo;
		}
		
		// le imagem de um arquivo
		Imagem(char *nomeArquivo) {
			FILE *fp;
			int valor, MAX = INFINITO;
			int h = 0, w = 0;
			fp = fopen(nomeArquivo,"rb");
			if (fp==NULL) { 
				fprintf(stderr, "não é possível abrir o arquivo %s\n",nomeArquivo);
				exit(-1);
			}
			fscanf(fp, "%s", formato); //P5 -> int   |  P2 -> char
			if((strcmp(formato,"P5") != 0) && (strcmp(formato,"P2") != 0)) {
				fprintf(stderr, "arquivo P5 ou P2 esperado! \n");
				exit(-1);
			}
			//-------leitura dos comentarios e largura, altura e MAX------
			int i=fgetc(fp);
			while(isspace(i)) i=fgetc(fp);
			if(i=='#')  {
				while(i!=10) i=fgetc(fp);
				fscanf(fp,"%d%d%d",&largura,&altura,&MAX);
			}
			else {
				rewind(fp);
				fscanf(fp,"%s%d%d%d",formato,&largura,&altura,&MAX);
			}
			fgetc(fp);
			//--------aloca matriz de pixels--------
			matriz = new int*[altura];
			for (i=0; i<altura; i++)
				matriz[i] = new int[largura];
			//--------------------------------------
			if(strcmp(formato,"P5") == 0) {
				valor=fgetc(fp);
				while(!feof(fp)) {
					matriz[h][w] = valor;
					valor=fgetc(fp);
					w++;
					if(w == largura) { w = 0; h++; }
				}
			}
			else {  //P2
				fscanf(fp,"%d",&valor);
				while(!feof(fp)) {
					matriz[h][w] = valor;
					fscanf(fp,"%d",&valor);
					w++;
					if(w == largura) { w = 0; h++; }
				}
			}
			if (MAX == 255) bits = CINZA; else if (MAX == 1) bits = BINARIA; else bits = INTEIRA;
		}

		// escreve imagem de uma matriz para arquivo
		void escreve(char *nomeArquivo) {
			FILE *fp;
			int a, b, i, MAX = INFINITO;
			if (bits == CINZA) MAX = 255; else if (bits == BINARIA) MAX = 1;
			fp = fopen(nomeArquivo,"w+b");
			if (fp==NULL) {
				fprintf(stderr, "não é possível abrir o arquivo %s\n",nomeArquivo);
				exit(-1);
			}
			i = fputs("P5\n", fp);
			i = fputs("#", fp);
			i = fputs(" Created", fp);
			i = fputs(" by", fp);
			i = fputs(" LAPIS/UDESC\n", fp);
			fprintf(fp, "%d %d\n", largura,altura);
			fprintf(fp, "%d\n", MAX);
			for(a=0; a < altura; a++) {
				for(b=0; b < largura; b++) {
					i = fputc(matriz[a][b], fp);
				}
			}
			fclose(fp);
		}
		
		void zera() {
			int i, j;
			for (i=0; i < altura; i++)
				for (j=0; j < largura; j++)
					matriz[i][j] = 0;
		}
		
		bool ehBinaria() {
			return (bits == BINARIA);
		}
		
		bool ehCinza() {
			return (bits == CINZA);
		}
		
		bool ehInteira() {
			return (bits == INTEIRA);
		}
		
		void comoBinaria() {
			int i, j;
			if (bits != BINARIA) {
				for (i=0; i < altura; i++)
					for (j=0; j < largura; j++)
						if (matriz[i][j] > 0)
							matriz[i][j] = 1;
						else
							matriz[i][j] = 0;
				bits = BINARIA;
			}
		}
		
		void comoCinza() {
			int i, j;
			if (bits != BINARIA && bits != CINZA) {
				for (i=0; i < altura; i++)
					for (j=0; j < largura; j++)
						if (matriz[i][j] > 255)
							matriz[i][j] = 255;
						else if (matriz[i][j] < 0)
							matriz[i][j] = 0;
			}
			bits = CINZA;
		}

		void comoInteira() {
			int i, j;
			bits = INTEIRA;
		}

		Imagem& operator=(const Imagem& f) {
			int i, j;
			for (int i=0; i < f.altura; i++)
				for (int j=0; j < f.largura; j++)
					matriz[i][j] = f.matriz[i][j];
			bits = f.bits;
		}
		
		~Imagem() {
			if (matriz != NULL) {
				for (int i=0; i<altura; i++)
					delete matriz[i];
				delete matriz;
				matriz = NULL;
			}
		}

};
// fim Imagem





Imagem imInsere(const Imagem& f, Pontos p);
Pontos imNaozero(const Imagem& f);
Imagem imNormaliza(const Imagem& f, const int& inf = 0, const int& sup = 255);

Imagem imAplica(const Imagem& fr, int (* operador)(const Imagem& f));

int imArea(const Imagem& f);
int imPerimetro(const Imagem& f);
int imMaximo(const Imagem& f);
int imMinimo(const Imagem& f);

Imagem imCentroide(const Imagem& f);
Imagem imRetangulomin(const Imagem& f); // IMPLEMENTAR !!!!!!!!!!!!!!!!!!!!!!
Imagem imCirculomin(const Imagem& f); // IMPLEMENTAR !!!!!!!!!!!!!!!!!!!!!!
		
Imagem imSoma(const Imagem& f, const int& c);
Imagem imSoma(const Imagem& f1, const Imagem& f2);

Imagem imSubtrai(const Imagem& f, const int& c);
Imagem imSubtrai(const Imagem& f1, const Imagem& f2);

Imagem imMultiplica(const Imagem& f, const int& c);
Imagem imMultiplica(const Imagem& f1, const Imagem& f2);

Imagem imReflete(const Imagem& f);




class Estruturante {
	public:
		Imagem *img;
		Pontos *viz; 
		int     altura;
		int     largura;
		int     tam;

	private:
		void imagem(const Imagem& f) {
			int i, j;
			int ic = f.altura  / 2;
			int jc = f.largura / 2;
			img = new Imagem(f.altura, f.largura, f.profundidade, 0, f.bits);
			viz = new Pontos(imArea(f));
			for (i=0; i < f.altura; i++)
				for (j=0; j < f.largura; j++) {
					img->matriz[i][j] = f.matriz[i][j];
					if (f.matriz[i][j] > 0)
						viz->insere(i - ic, j - jc);
				}
			tam = viz->tam;
			altura  = f.altura;
			largura = f.largura;
		}
	
	public:
		Estruturante(char *tipo = "cruz", const int& raio = 1) {
			if (tipo == "caixa") {
				int dim = 2*raio+1;
				Imagem f(dim, dim, 1, 1);
				imagem(f);
			}
			else if (tipo == "disco") {
				int i, j, dim = 2*raio+1;
				Imagem f(dim, dim, 1);
				for (i=0; i<dim; i++)
					for (j=0; j<dim; j++)
						f.matriz[i][j] = ( (pow(i-raio,2) + pow(j-raio,2)) <= pow(raio+0.5,2) );
				imagem(f);
			}
			else {
				int i, j, dim = 2*raio+1;
				Imagem f(dim, dim, 1);
				for (i=0; i<dim; i++)
					for (j=0; j<dim; j++)
						f.matriz[i][j] = ( (abs(raio-i) + abs(raio-j)) <= raio);
				imagem(f);
			}
		}
		
		Estruturante(const Imagem& f) {
			imagem(f);
		}
		
		int vizX(const int& pos) {
			return viz->getX(pos);
		}
		
		int vizY(const int& pos) {
			return viz->getY(pos);
		}
		
		void destroi() {
			if (img != NULL) {
				tam = 0;
			}
		}
/*
		Estruturante& operator=(const Estruturante& e) {
			int i, j;
			altura  = e.img->altura;
			largura = e.img->largura;
			img = new Imagem(altura, largura, e.img->profundidade, 0, e.img->bits);
			for (i=0; i < altura; i++)
				for (j=0; j < largura; j++)
					img->matriz[i][j] = e.img->matriz[i][j];
			tam = e.tam;
			viz = new Pontos(tam);
			for (i=0; i < tam; i++)
				viz->insere(e.viz->getX(i), e.viz->getY(i));
		}

		~Estruturante() {
			if (img != NULL) {
				delete img;
				delete viz;
				altura = largura = tam = 0;
			}
		}
*/
};
// fim Estruturante


Estruturante eeImagem(const Imagem& f);
Estruturante eeCaixa(const int& raio = 1);
Estruturante eeCruz (const int& raio = 1);
Estruturante eeDisco(const int& raio = 3);
Estruturante eeReflete(const Estruturante& e);

Imagem imLe(char *nomeArquivo);
Imagem imClone(const Imagem& f);
Imagem imConstante(const int& altura, const int& largura, const int& profundidade, const int& valor = 0, const int& tipo = CINZA);
Imagem imZeros(const int& altura, const int& largura, const int& profundidade, const int& tipo = CINZA);

Imagem imBinaria(const Imagem& f);
Imagem imCinza(const Imagem& f);
Imagem imInteira(const Imagem& f);

Imagem imNega(const Imagem& f);
Imagem imLimiariza(const Imagem& f, const int& inf, const int& sup = INFINITO);

Imagem imDilata(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imErode(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imAbre(const Imagem& f, const Estruturante& e = eeCruz());
Imagem imFecha(const Imagem& f, const Estruturante& e = eeCruz());

Imagem imGradiente(const Imagem& f, const Estruturante& edil = eeCruz(), const Estruturante& eero = eeCruz());
Imagem imDistancia(const Imagem& f, const Estruturante& e = eeCruz());

Imagem imUniao(const Imagem& f, const int& c);
Imagem imUniao(const Imagem& f1, const Imagem& f2);
Imagem imIntersecao(const Imagem& f, const int& c);
Imagem imIntersecao(const Imagem& f1, const Imagem& f2);

Imagem imReconstroi(const Imagem& f, const Imagem& m, const Estruturante& e = eeCruz());
Imagem imHmax(const Imagem& f, const int& h = 1, const Estruturante& e = eeCruz());

Imagem imBorda(const Imagem& f, const int& tam_vert=1, const int& tam_horiz=1, const int& valor_borda = 255, const int& valor_fundo = 0);
Imagem imBuraco(const Imagem& f, const Estruturante& e = eeCruz());

Imagem imRotula(const Imagem& f, const Estruturante& e = eeCruz());

bool imSaoiguais(const Imagem& f1, const Imagem& f2);
