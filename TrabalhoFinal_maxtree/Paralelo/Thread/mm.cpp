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

#include "mm.h"


// ELEMENTOS ESTRUTURANTES
Estruturante eeImagem(const Imagem& f) {
	Estruturante e(f);
	return e;
}
Estruturante eeCruz(const int& raio) {
	Estruturante e("cruz",raio);
	return e;
}
Estruturante eeCaixa(const int& raio) {
	Estruturante e("caixa",raio);
	return e;
}
Estruturante eeDisco(const int& raio) {
	Estruturante e("disco",raio);
	return e;
}
Estruturante eeReflete(const Estruturante& e) {
	int i, j;
	Imagem aux(e.altura, e.largura, 1);
	for (i=0; i < e.altura; i++)
		for (j=0; j < e.largura; j++)
			aux.matriz[i][j] = e.img->matriz[e.altura-1-i][e.largura-1-j];
	Estruturante er(aux);
	return er;
}


// LEITURA DE UMA IMAGEM E A TRANSFORMACAO PARA NIVEIS DE CINZA
Imagem imLe(char *nomeArquivo) {
	Imagem f(nomeArquivo);
	return f;
};

// CLONE DE UMA IMAGEM
Imagem imClone(const Imagem& f) {
	int i, j;
	Imagem g(f.altura, f.largura, f.profundidade, 0, f.bits);
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = f.matriz[i][j];
	return g;
}

// CRIACAO DE UMA IMAGEM COM VALOR CONSTANTE
Imagem imConstante(const int& altura, const int& largura, const int& profundidade, const int& valor, const int& tipo) {
	Imagem f(altura, largura, profundidade, valor, tipo);
	return f;
}

// CRIACAO DE UMA IMAGEM COM ZEROS
Imagem imZeros(const int& altura, const int& largura, const int& profundidade, const int& tipo) {
	Imagem f(altura, largura, profundidade, 0, tipo);
	return f;
}

// TRANSFORMACAO DE IMAGEM PARA BINARIA (BIT) OU PARA NIVEIS DE CINZA (BYTE)
Imagem imBinaria(const Imagem& f) {
	Imagem g = imClone(f);
	if (!g.ehBinaria())
		g.comoBinaria();
	return g;
}
Imagem imCinza(const Imagem& f) {
	Imagem g = imClone(f);
	if (!g.ehCinza())
		g.comoCinza();
	return g;
}
Imagem imInteira(const Imagem& f) {
	Imagem g = imClone(f);
	if (!g.ehInteira())
		g.comoInteira();
	return g;
}

// INSECAO DE ALGUNS PIXELS
Imagem imInsere(const Imagem& f, Pontos p) {
	int k, MAX = INFINITO;
	if (f.bits == CINZA) MAX = 255; else if (f.bits == BINARIA) MAX = 1;
	Imagem g = imSoma(f,0);
	for (k = 0; k < p.tam; k++)
		g.matriz[p.getX(k)][p.getY(k)] = MAX;
	return g;
}

// COORDENADAS DOS PIXELS DIFERENTES DE ZERO
Pontos imNaozero(const Imagem& f) {
	int i, j, area, pixel;
	area = imArea(f);
	Pontos p(area);
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] > 0)
				p.insere(i,j);
	return p;
}

// NORMALIZACAO
Imagem imNormaliza(const Imagem& f, const int& inf, const int& sup) {
	int i, j, d1, d2, MIN, MAX;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	MIN = imMinimo(f);
	MAX = imMaximo(f);
	d1 = MAX - MIN;
	d2 = sup - inf;
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = (int) ( inf + 1.0*(f.matriz[i][j] - MIN) * d2 / d1  + 0.5 );
	return g;
}

// APLICACAO DE UMA FUNCAO SOBRE UMA IMAGEM ROTULADA
Imagem imAplica(const Imagem& fr, int (* operador)(const Imagem& f)) {
	int k, n;
	Imagem g = imZeros(fr.altura, fr.largura, fr.profundidade, fr.bits);
	n = imMaximo(fr);
	for (k=1; k <= n; k++) {
		Imagem l = imLimiariza(fr, k, k);
		l.comoInteira();
		g = imSoma(g, imMultiplica(l,operador(l)));
	}
	return g;
}

// QUANTIDADE DOS PIXELS DIFERENTES DE ZERO
int imArea(const Imagem& f) {
	int i, j, area=0;
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] != 0)
				area++;
	return area;
}
// QUANTIDADE DOS PIXELS DO CONTORNO DE UMA IMG BINARIA
int imPerimetro(const Imagem& f) {
	Imagem c = imGradiente(f, eeCruz(0), eeCruz());
	Pontos p = imNaozero(f);
	return p.tam;
}

// MAXIMO E MINIMO DA IMAGEM
int imMaximo(const Imagem& f) {
	int i, j, maximo=-INFINITO;
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] > maximo)
				maximo = f.matriz[i][j];
	return maximo;
}
int imMinimo(const Imagem& f) {
	int i, j, minimo=INFINITO;
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] < minimo)
				minimo = f.matriz[i][j];
	return minimo;
}

// UNICO PONTO CORRESPONDENDO AO CENTROIDE DOS PIXELS DIFERENTES DE ZERO
Imagem imCentroide(const Imagem& f) {
	int i, j, xc=0, yc=0, n=0;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, BINARIA);
	for (i = 0; i < f.altura; i++)
		for (j = 0; j < f.largura; j++)
			if (f.matriz[i][j] > 0) {
				xc = xc + i;
				yc = yc + j;
				n++;
			}
	if (n > 0) {
		xc = (int) (1.0*xc / n + 0.5);
		yc = (int) (1.0*yc / n + 0.5);
		g.matriz[xc][yc] = 1;
	}
	return g;
}

// RETANGULO MINIMO (bounding box)
Imagem imRetangulomin(const Imagem& f) {
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	return g;
}

// CIRCULO MINIMO (minimum circle)
Imagem imCirculomin(const Imagem& f) {
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	return g;
}



// SOMA ENTRE IMAGEM E UMA CONSTANTE OU ENTRE IMAGENS COM SATURACAO
Imagem imSoma(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imSoma(f, fc);
	return g;
}
Imagem imSoma(const Imagem& f1, const Imagem& f2) {
	int i, j, bits;
	int SATURACAO = INFINITO;
	if (f1.bits > f2.bits) bits = f1.bits; else bits = f2.bits;
	if (bits == BINARIA) SATURACAO = 1; else if (bits == CINZA) SATURACAO = 255;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, bits);
	for (i=0; i<f1.altura; i++)
		for (j=0; j<f1.largura; j++) {
			g.matriz[i][j] = f1.matriz[i][j] + f2.matriz[i][j];
			if (g.matriz[i][j] > SATURACAO)
				g.matriz[i][j] = SATURACAO;
		}
	return g;
}

// SUBTRACAO ENTRE IMAGEM E UMA CONSTANTE OU ENTRE IMAGENS COM SATURACAO
Imagem imSubtrai(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imSubtrai(f, fc);
	return g;
}
Imagem imSubtrai(const Imagem& f1, const Imagem& f2) {
	int i, j, bits;
	int SATURACAO = -INFINITO;
	if (f1.bits < f2.bits) bits = f1.bits; else bits = f2.bits;
	if (bits == BINARIA || bits == CINZA) SATURACAO = 0;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, bits);
	for (i=0; i<f1.altura; i++)
		for (j=0; j<f1.largura; j++) {
			g.matriz[i][j] = f1.matriz[i][j] - f2.matriz[i][j];
			if (g.matriz[i][j] < SATURACAO)
				g.matriz[i][j] = SATURACAO;
		}
	return g;
}

// MULTIPLICAO ENTRE IMAGEM E UMA CONSTANTE OU ENTRE IMAGENS COM SATURACAO
Imagem imMultiplica(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imMultiplica(f, fc);
	return g;
}
Imagem imMultiplica(const Imagem& f1, const Imagem& f2) {
	int i, j, bits;
	int SATURACAO = INFINITO;
	if (f1.bits > f2.bits) bits = f1.bits; else bits = f2.bits;
	if (bits == BINARIA) SATURACAO = 1; else if (bits == CINZA) SATURACAO = 255;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, bits);
	for (i=0; i<f1.altura; i++)
		for (j=0; j<f1.largura; j++) {
		g.matriz[i][j] = f1.matriz[i][j] * f2.matriz[i][j];
		if (g.matriz[i][j] > SATURACAO)
			g.matriz[i][j] = SATURACAO;
		}
	return g;
}

// NEGACAO
Imagem imNega(const Imagem& f) {
	int i, j, MAX = INFINITO;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	if      (f.bits == BINARIA) MAX = 1;
	else if (f.bits == CINZA)   MAX = 255;
	for (i=0; i<f.altura; i++)
		for (j=0; j<f.largura; j++)
			g.matriz[i][j] = MAX - f.matriz[i][j];
	return g;
}

// LIMIARIZACAO
Imagem imLimiariza(const Imagem& f, const int& inf, const int& sup) {
	int i, j;
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, BINARIA);
	for (i=0; i<f.altura; i++)
		for (j=0; j<f.largura; j++)
			if (f.matriz[i][j] >= inf && f.matriz[i][j] <= sup)
				g.matriz[i][j] = 1;
	return g;
}


// DILATACAO
Imagem imDilata(const Imagem& f, const Estruturante& e) {
	Estruturante er = eeReflete(e);
	int k, i, j, vx, vy, maximo;
	Imagem g  = imZeros(f.altura, f.largura, f.profundidade, f.bits);
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++) {
			maximo = -INFINITO;
			for (k=0; k < er.tam; k++) {
				vx = i + er.vizX(k);
				vy = j + er.vizY(k);
				if ((vx >=0 && vx < f.altura) && (vy >=0 && vy < f.largura) && f.matriz[vx][vy] > maximo)
					maximo = f.matriz[vx][vy];
			}
			g.matriz[i][j] = maximo;
		}
	er.destroi();
	return g;
}

// EROSAO
Imagem imErode(const Imagem& f, const Estruturante& e) {
	Estruturante er = eeReflete(e);
	Imagem g = imNega(imDilata(imNega(f),er));
	er.destroi();
	return g;
}

// ABERTURA
Imagem imAbre(const Imagem& f, const Estruturante& e) {
	Imagem g = imDilata( imErode(f,e), e );
	return g;
}

// FECHAMENTO
Imagem imFecha(const Imagem& f, const Estruturante& e) {
	Imagem g = imErode( imDilata(f,e), e );
	return g;
}

// GRADIENTE MORFOLOGICO
Imagem imGradiente(const Imagem& f, const Estruturante& edil, const Estruturante& eero) {
	Imagem g = imSubtrai( imDilata(f,edil), imErode(f,eero) );
	return g;
}

// TRANSFORMADA DISTANCIA
Imagem imDistancia(const Imagem& f, const Estruturante& e) {
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, INTEIRA);
	Imagem a = imBinaria(f);
	Imagem b = imErode(a,e);
	while (!imSaoiguais(a,b)) {
		g = imSoma(g, a);
		a = b;
		b = imErode(a,e);
	}
	return g;
}

// UNIAO
Imagem imUniao(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imUniao(f, fc);
	return g;
}
Imagem imUniao(const Imagem& f1, const Imagem& f2) {
	int i, j;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, f1.bits);
	for (i=0; i < f1.altura; i++)
		for (j=0; j < f1.largura; j++)
			if (f1.matriz[i][j] > f2.matriz[i][j])
				g.matriz[i][j] = f1.matriz[i][j];
			else
				g.matriz[i][j] = f2.matriz[i][j];
	return g;
}

// INTERSECAO
Imagem imIntersecao(const Imagem& f, const int& c) {
	Imagem fc = imConstante(f.altura, f.largura, f.profundidade, c, f.bits);
	Imagem g  = imIntersecao(f, fc);
	return g;
}
Imagem imIntersecao(const Imagem& f1, const Imagem& f2) {
	int i, j;
	Imagem g = imZeros(f1.altura, f1.largura, f1.profundidade, f1.bits);
	for (i=0; i < f1.altura; i++)
		for (j=0; j < f1.largura; j++)
			if (f1.matriz[i][j] < f2.matriz[i][j])
				g.matriz[i][j] = f1.matriz[i][j];
			else
				g.matriz[i][j] = f2.matriz[i][j];
	return g;
}

// RECONSTRUCAO (f=imagem, m=marcador, e=elem. estruturante)
Imagem imReconstroi(const Imagem& f, const Imagem& m, const Estruturante& e) {
	Imagem a = imClone(m);
	Imagem b = imIntersecao( imDilata(m,e), f );
	while (!imSaoiguais(a,b)) {
		a = imClone(b);
		b = imIntersecao( imDilata(a,e), f );
	}
	return b;
}

// REMOCAO DE PICOS COM CONTRASTE MENOR QUE h
Imagem imHmax(const Imagem& f, const int& h, const Estruturante& e) {
	Imagem m = imSubtrai(f,h);
	Imagem g = imReconstroi(f,m,e);
	return g;
}

// INSERCAO DE UMA BORDA
Imagem imBorda(const Imagem& f, const int& tam_vert, const int& tam_horiz, const int& valor_borda, const int& valor_fundo) {
	int i, j;
	Imagem g = imConstante(f.altura, f.largura, f.profundidade, valor_fundo, f.bits);
	for (i=0; i < tam_vert; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = valor_borda;
	for (; i < f.altura-tam_vert; i++)
		for (j=0; j < tam_horiz; j++) {
			g.matriz[i][j]             = valor_borda;
			g.matriz[i][f.largura-j-1] = valor_borda;
		}
	for (; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = valor_borda;
	return g;
}

// FECHAMENTO DE BURACOS
Imagem imBuraco(const Imagem& f, const Estruturante& e) {
	Imagem borda = imBorda(f);
	Imagem g = imNega( imReconstroi(imNega(f), borda, e) );
	return g;
}

// ROTULACAO
Imagem imRotula(const Imagem& f, const Estruturante& e) {
	int i, j, rotulo = 1;
	Imagem a = imBinaria(f);
	Imagem g = imZeros(f.altura, f.largura, f.profundidade, INTEIRA);
	for (i=0; i < a.altura; i++)
		for (j=0; j < a.largura; j++)
			if (a.matriz[i][j]) { // nao visitado e faz parte da regiao
				Imagem m = imZeros(f.altura, f.largura, f.profundidade, BINARIA);
				m.matriz[i][j] = 1;
				Imagem r = imReconstroi(a,m,e);
				a = imSubtrai(a, r);
				g = imUniao( g, imMultiplica(imInteira(r),rotulo) );
				rotulo++;
			}
	return g;
}

// REFLEXAO
Imagem imReflete(const Imagem& f) {
	int i, j;
	Imagem g(f.altura, f.largura, f.profundidade);
	for (i=0; i < f.altura; i++)
		for (j=0; j < f.largura; j++)
			g.matriz[i][j] = f.matriz[f.altura-1-i][f.largura-1-j];
	return g;
}

// VERIFICACAO SE DUAS IMAGENS SAO IGUAIS
bool imSaoiguais(const Imagem& f1, const Imagem& f2) {
	int i, j;
	for (i=0; i<f1.altura; i++)
		for (j=0; j<f1.largura; j++)
			if (f1.matriz[i][j] != f2.matriz[i][j]){
				return false;
			}
	return true;
}


Pontos imLimites(Pontos p) {
	int i, minX, maxX, minY, maxY;
	minX = p.getX(0); maxX = p.getX(0);
	minY = p.getY(0); maxY = p.getY(0);
	for (i=1; i<p.tam; i++) {
		if (p.getX(i) < minX)
			minX = p.getX(i);
		if (p.getX(i) > maxX)
			maxX = p.getX(i);
		if (p.getY(i) < minY)
			minY = p.getY(i);
		if (p.getY(i) > maxY)
			maxY = p.getY(i);
	}
	Pontos q(2);
	q.insere(minX,minY);
	q.insere(maxX,maxY);
	return q;
}

