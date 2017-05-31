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
#define CLK_TCK CLOCKS_PER_SEC

#define PI  3.1415926535897931
#define EPS 1e-10

#define LINE 1
#define SLIN 2
#define ARCH 3
#define CIRC 4
#define ELIP 5

#define SUCESSO  1
#define FRACASSO 0
#define SIM      1
#define NAO      0

#define ORDEM    5



using std::cout;
using std::endl;
using std::cin;



class Grafo;


/* ------------------------------- ANE ------------------------------- */
typedef struct {
	int chave;  /* rotulo */
	Grafo *no;
} Registro;
typedef struct NoANE {
	Registro info;
	struct NoANE *filho[ORDEM];
} NoANE, *pNoANE;
typedef struct {
	pNoANE raiz;
} ANE, *pANE, **ppANE;
pANE criaANE();
void destroiABB(ppANE pp);
pNoANE criaNoANE(Registro x);
int ehVaziaANE(pANE pd);
int insereANE(pANE pd, Registro x, int *comparacoes);
int removeANE(pANE pd, int chave);
int buscaANE(pANE pd, int chave, Registro *px, int *comparacoes);
void percursoANE(pNoANE p);
/* ------------------------------------------------------------------- */





//----------------------classe arvore-------------------
class Grafo {
	private:
		int  nome[2];    //(nivel,rotulo)
		int  ordem;      //numero de filhos
		int  particao;   //rotulo do noh conforme marcadores
		bool ativo;      //ativado=1, desativado=0
	public:
		Grafo *filho;    //armazena sempre o filho mais novo
		Grafo *pai;
		Grafo *irmao;    //imediatamente mais velho q o noh corrente!!! o filho mais velho nao tem irmao!!
		Grafo *proximo;  //sequencia de construção das regioes!
		int getNivel() {
			return nome[0];
		}
		int getRotulo() {
			return nome[1];
		}
		int getOrdem() {
			return ordem;
		}
		int isOn() {
			return ativo;
		}
		void setOn() {
			ativo = true;
		}
		void setOff() {
			ativo = false;
		}
		int getParticao() {
			return particao;
		}
		bool setParticao(int valor) {
			if (particao > 0)
				return false;
			particao = valor;
			return true;
		}
		Grafo() { //construtor
		//	nome[0]  = -1;
		//	nome[1]  =  1;
		//	ordem    =  0;
		//	particao =  0;
		//	ativo    =  true;
		//	filho = pai = irmao = proximo = NULL;
		}
		
		int inserir(Grafo *p, pANE hash[257]) {
			Registro x;
			int cmps=0;
			x.chave = p->getRotulo();
			x.no    = p;
			return insereANE(hash[p->getNivel()+1], x, &cmps);
		}

		Grafo *buscar(int nivel, int rotulo, pANE hash[257]) {
			Registro x;
			int cmps=0;
			if (buscaANE(hash[nivel+1], rotulo, &x, &cmps) == SUCESSO) {
				return x.no;
			}
			return NULL;
		}

//--------------------função inserir principal---------------------
		Grafo *inserir(int nPai, int rPai, int nFilho, int rFilho, pANE hash[257]) { //retorna sempre o primeiro da lista
			Grafo *grafoPai   = this;
			Grafo *grafoFilho = this;
			Grafo *aux        = this;
			
			grafoPai   = buscar(nPai,   rPai,   hash);
			grafoFilho = buscar(nFilho, rFilho, hash);

		    if (grafoPai) {
				if (grafoFilho) {
					grafoFilho->irmao = grafoPai->filho;
					grafoFilho->pai   = grafoPai;
					grafoPai->filho   = grafoFilho; /////nao pode fazer: grafoFilho->filho = NULL;
					grafoPai->ordem++;
					return this;
				}
				grafoFilho = inserirNovo(nFilho,rFilho,hash);  //se soh existe o noh pai!!
				grafoFilho->irmao = grafoPai->filho;
				grafoFilho->pai   = grafoPai;
				grafoPai->filho   = grafoFilho;
				grafoPai->ordem++;
				return grafoFilho; //ultimo inserido!!
			}
			grafoPai = inserirNovo(nPai,rPai,hash);
			if (grafoFilho) {
				//se o noh filho existir!!
				grafoFilho->irmao = grafoPai->filho; //nao precisa!!!
				grafoFilho->pai   = grafoPai;
				grafoPai->filho   = grafoFilho;
				grafoPai->ordem++;
				return grafoPai;
			}
			//se nao existir nenhum dos dois
			grafoFilho = inserirNovo(nFilho,rFilho,hash);
			grafoFilho->irmao   = grafoPai->filho;  //nao precisa
			grafoFilho->pai     = grafoPai;
			grafoPai->filho     = grafoFilho;
			grafoFilho->proximo = grafoPai;
			grafoPai->ordem++;
			return grafoFilho;
		}
//---------------------------inserir--------------------------

//------------------função inserirNovo--------------------------
		Grafo *inserirNovo(int nPai, int rPai, pANE hash[257]) {   //insere sempre no inicio da lista
			Grafo *novo   = new Grafo;
			novo->nome[0] = nPai;
			novo->nome[1] = rPai;
			novo->filho   = NULL;
			novo->pai     = NULL;
			novo->irmao   = NULL;
			novo->proximo = this;  //???
			novo->ordem   =  0;
			novo->particao=  0;
			novo->ativo   =  true;
			inserir(novo, hash);
			return novo;
		}
//----------------------inserirNovo-----------------------------

};

//--------------fim------classe arvore------------------










//-----------------classe fila no-------------------------
class PilhaNo {
	public:
		Grafo   *noh;
		PilhaNo *proximo;
		PilhaNo *empilha(Grafo *no) {
			PilhaNo *novo = new PilhaNo;
			novo->noh = no;
			novo->proximo = this;
			return novo;
		}
		PilhaNo *desempilha(Grafo **no) {
			*no = noh;
			PilhaNo *pilhaAuxiliar = this;
			PilhaNo *pilhaAux = pilhaAuxiliar->proximo;
			delete pilhaAuxiliar;
			return pilhaAux;
		}
		PilhaNo *empilhaFilhos(Grafo *no) {
			PilhaNo *pilhaAux = this;
			Grafo   *aux = no->filho;
			while (aux) {
				pilhaAux = pilhaAux->empilha(aux);
				aux = aux->irmao;
			}
			return pilhaAux;

		}
};
//------------fim--classe pilha no---------------------





















//-----------------classe pilha-------------------------
class Pilha {
	private:      
		int Px;
		int Py;
		int nivel;
	public:
		Pilha *proximo;
		int getPx() {
			return Px;
		}
		int getPy() {
			return Py;
		}  
		int getNivel() {
			return nivel;
		}      
		void setPx(int x) {
			Px = x;
		}             
		void setPy(int y) {
			Py = y;
		}  
		void setNivel(int n) {
			nivel = n;
		}

//---------------------------insere na pilha-------------------	
		Pilha *inserirNaPilha(int x, int y, int n) { //insere no inicio
			Pilha * novo = new Pilha;
			novo->Px = x;
			novo->Py = y; 	                    
			novo->nivel = n;
			novo->proximo = this;
			return novo;
		}
//-------------------------fim da função inserir-------------------------

//-------------------------remover e retornar primeiro da pilha-----------
		Pilha *removerDaPilha() {
			Pilha * pilhaAuxiliar = this;
			Pilha * pilhaAux = pilhaAuxiliar->proximo;
			delete pilhaAuxiliar;
			return pilhaAux;
		}
//-------------------------fim a função remover--------------------------

};
//------------fim--classe pilha---------------------


//-----------------classe fila-------------------------
class Fila {
	private:      
		int Px;
		int Py;
		int nivel;
	public:
		Fila *proximo;
		int getPx() {
			return Px;
		}
		int getPy() {
			return Py;
		}
		int getNivel() {
			return nivel;
		}
		void setPx(int x) {
			Px = x;
		}
		void setPy(int y) {
			Py = y;
		}
		void setNivel(int n) {
			nivel = n;
		}

//---------------------------insere ao final da fila-------------------	
		Fila *inserir(int x, int y, int n) {   //insere ao final da fila e retorna o primeiro
			Fila *novo = new Fila;
			novo->Px = x;
			novo->Py = y;
			novo->nivel = n;
			novo->proximo = NULL;
			Fila *filaAuxiliar = this;
			while (filaAuxiliar) {
				if (filaAuxiliar->proximo == NULL) { //ultimo elemento aponta para novo
					filaAuxiliar->proximo = novo; //nó inserido ao final da fila!
					return this; //retorna o primeiro que continua sendo o obeto corrente
				}
				filaAuxiliar = filaAuxiliar->proximo;
			} //só sai do while se não houver nenhum elemento na fila logo novo será o primeiro
			return novo;
		}
//-------------------------fim da função inserir-------------------------

//-------------------------remover e retornar primeiro da fila-----------
		Fila *removeDaFila() {
			Fila  *filaAuxiliar = this;
			Fila  *filaAux = filaAuxiliar->proximo;
			delete filaAuxiliar;
			return filaAux;
		}
//-------------------------fim a função remover--------------------------

};
//------------fim--classe fila---------------------



                                  
//----------------------classe lista de filas-------------------
class ListaFila {
	private:
		int nivel;
	public:
		Fila      *fila;    //armazena sempre o último inserido
		ListaFila *proximo;
		
		int getNivel() {
			return nivel;
		}      

//--------------------função inserir principal---------------------	
		ListaFila *inserir(int n, int xFila, int yFila, int nFila) { //retorna sempre o primeiro da lista(ultimo inserido)
			ListaFila *listaAuxiliar = this;
			while (listaAuxiliar) {
				if (listaAuxiliar->getNivel() == n) {
					listaAuxiliar->fila = listaAuxiliar->fila->inserir(xFila, yFila, nFila); //esta função deve inserir no final da fila
					return this; // e retornar o primeiro da fila
				}
				listaAuxiliar = listaAuxiliar->proximo;
			}
			return inserirNovo(n, xFila, yFila, nFila);
		}
//---------------------------inserir--------------------------

//------------------função inserirNovo--------------------------
		ListaFila *inserirNovo(int n, int xFila, int yFila, int nFila) {   //insere sempre no inicio da lista
			ListaFila *novo = new ListaFila;               
			novo->nivel   = n;
			novo->fila    = NULL;
			novo->fila    = novo->fila->inserir(xFila, yFila, nFila);  //esta função deve inserir no final da fila
			novo->proximo = this; // e retornar o primeiro da fila
			return novo;
		}

//-----------retira da fila-----se a fila for vazia retorna NULL --------
		Fila *primeiroDaFila(int n) { //remove e retorna o primeiro da fila
			ListaFila *listaAuxiliar = this;
			while (listaAuxiliar) {
				if (listaAuxiliar->getNivel() == n)
					if (listaAuxiliar->fila) {
						Fila *filaAuxiliar = new Fila; //deve ser deletada na função flood!!!! ou entao pode ser uma variavel global ou ainda passada como argumento!!!
						filaAuxiliar->setNivel(listaAuxiliar->fila->getNivel());
						filaAuxiliar->setPx(listaAuxiliar->fila->getPx());
						filaAuxiliar->setPy(listaAuxiliar->fila->getPy());
						listaAuxiliar->fila = listaAuxiliar->fila->removeDaFila();
						return filaAuxiliar;
					}
					listaAuxiliar = listaAuxiliar->proximo;
			}
			return NULL; //fila vazia ou nao existe.
		}
//--------------------------------------------------------------

};
//--------------fim------classe lista de filas------------------       
       







class Vizinhanca {
	private:
		int y;
		int x;
	public:
		Vizinhanca *proximo;
		int getX() {
			return(x);
		}   
		int getY() {
			return(y);
		}
		Vizinhanca *inserir(int x, int y) {   //insere sempre no inicio da lista
			Vizinhanca *novo = new Vizinhanca;
			novo->x = x;
			novo->y = y;
			novo->proximo = this;
			return novo;
		}
		Vizinhanca *remover() {
			Vizinhanca *aux = this->proximo;
			delete this;
			return aux;
		}
		Vizinhanca *destruir() {
			Vizinhanca *aux = this;
			Vizinhanca *auxProximo;
			while (aux) {
				auxProximo = aux->proximo;
				delete aux;
				aux = auxProximo;
			}
			return NULL;
		}
		//~Vizinhanca() {
		//	Vizinhanca *aux = this;
		//	Vizinhanca *auxProximo;
		//	while (aux) {
		//		auxProximo = aux->proximo;
		//		delete aux;
		//		aux = auxProximo;
		//	}
		//}
};
//----------------fim----classe vizinhança--------------












//-------------------- lista de folhas -------------------
class Folhas {
	public:
		Grafo  *folha;
		Folhas *proximo;

		Folhas *insereFolha(Grafo *regiao) {  //insere no inicio
			Folhas *novo  = new Folhas;
			novo->folha   = regiao;
			novo->proximo = this;  
			return novo;
		}
};
//--------------------------------------------------------





//################################################################
//################################################################






//----------------------classe Maxtree principal-------------------
class MaxTree {

	public:
		int      **img;  //matriz da imagem
		int        altura; //dimensões da imagem
		int        largura;
		int        maiorOrdem; //ordem maxima de um no da max-tree
		int        menorValor;
		int        maiorValor;
		int      **status;
		ListaFila *fifo;
		int        numeroNos[256];
		int        nivelNo[256];
		int      **matrizVizinhos;
		int        alturaVizinho; //size_y e size_x no codigo python
		int        larguraVizinho;
		Grafo     *grafo; //self.graph = {}
		int        NODES;
		int        COMPLEX;
		//int **seq;
		//int s;
		pANE       hash[257];
		
		Vizinhanca* vizinhos(Fila *p) {
			int i, j, x, y;
			int center_x = alturaVizinho  / 2;
			int center_y = larguraVizinho / 2;
			Vizinhanca *ne = NULL; //vizinhos válidos
			for (i=0 ; i < alturaVizinho ; i++)
				for (j=0 ; j < larguraVizinho ; j++) {
				if ((matrizVizinhos[i][j] > 0) && ((i != center_x) || (j != center_y))) {
					x = p->getPx() + i - center_x;
					y = p->getPy() + j - center_y;
					if ((x>=0) && (y>=0) && (x<altura) && (y<largura))
						ne = ne->inserir((p->getPx()+i-center_x),(p->getPy()+j-center_y)); //NEp
				}
				}
				return ne;
		}

		
		int flood(int h) {
			
			Fila *p = fifo->fila;
			Vizinhanca *q = NULL;
			int m, i, j;

			p = fifo->primeiroDaFila(h);  //se p for NULL é pq a fila está vazia
			
			while (p) {
				status[p->getPx()][p->getPy()] = numeroNos[h] + 1;
				q = vizinhos(p);
				while (q) {
					COMPLEX++;
					if (status[q->getX()][q->getY()] == 0) {
						m = img[q->getX()][q->getY()];
						fifo = fifo->inserir(m,q->getX(),q->getY(),m);
						status[q->getX()][q->getY()] = -1;
						nivelNo[m] = 1;
						while (m > h)
							m = flood(m);
					}
					q = q->proximo;
				}
				p = fifo->primeiroDaFila(h);
			}

			m = h - 1;
			while ((m >= 0) && (nivelNo[m] == 0))
				m = m - 1;

			if (m >= 0) {
				NODES++;
				i = numeroNos[h] + 1;
				j = numeroNos[m] + 1;
				grafo = grafo->inserir(m,j,h,i,hash);
				if (grafo->pai && grafo->pai->getOrdem() > maiorOrdem)
					maiorOrdem = grafo->pai->getOrdem();
			}
			else
				grafo = grafo->inserir(-1,1,menorValor,1,hash);

			nivelNo[h]   = 0;
			numeroNos[h] = numeroNos[h] + 1;

			return m;
		}
		
		MaxTree(int **vz, int vzAltura, int vzLargura, const Imagem& imag) {
			int **box = NULL;
			int i ,j, minPx, minPy;
			
			for (i=0; i < 257; i++)
				hash[i] = criaANE();

			matrizVizinhos = vz;
			
			img          =  imag.matriz;
			altura       =  imag.altura;
			largura      =  imag.largura;
			maiorOrdem   =  0;
			menorValor   =  INFINITO;
			maiorValor   = -INFINITO;
			
			for (i=0 ; i < altura ; i++) {
				for (j=0 ; j < largura ; j++) {
					if (imag.matriz[i][j] < menorValor) {
						menorValor = imag.matriz[i][j];
						minPx = i;
						minPy = j;
					}
					if (imag.matriz[i][j] > maiorValor)
						maiorValor = imag.matriz[i][j];
				}
			}
			
			//matriz do tamanho da imagem zerada
			status = new int*[altura];
			for (i=0 ; i < altura ; i++) {
				status[i] = new int[largura];
				for (j=0 ; j < largura ; j++)
					status[i][j] = 0;
			}

			fifo = NULL; //inicializa fifo com o menor nivel de cinza e as coordenadas correspondentes
			//fifo = fifo->inserir(menorValor,minPx,minPy,menorValor); // VERIFICAR !!!!!
			fifo = fifo->inserir(menorValor,0,0,menorValor); //para começar a inundaçao sempre pelo 1º pixel,
			                                                   //mas aih, eh como se o primerio pixel sempre possuisse 
			                                                   //o menor valor da imagem, oq eh um erro!!

			for(i=0 ; i<256 ; i++) { //vetor de cor zerada
				numeroNos[i] = 0;
				nivelNo[i]   = 0;
			}

			grafo = NULL;

			alturaVizinho  = vzAltura;
			larguraVizinho = vzLargura;
			
			NODES   = 0;
			COMPLEX = 0;

			flood(menorValor);

		}
		
		Folhas *retornaFolhas() {
			Grafo  *grafoaux = NULL;
			Folhas *folhas = NULL;
			grafoaux = grafo;
			while (grafoaux) { //inserção das folhas...
				if (grafoaux->filho == NULL)
					folhas = folhas->insereFolha(grafoaux);
				grafoaux = grafoaux->proximo;
			}
			return folhas;
		}
	
		int retornaPixel(Pontos pt, int pos) {
			return img[pt.getX(pos)][pt.getY(pos)];
		}

		int retornaStatus(Pontos pt, int pos) {
			return status[pt.getX(pos)][pt.getY(pos)];
		}

		int retornaLargura() {
			return largura;
		}

		int retornaAltura() {
			return altura;
		}
		
		int retornaRegioes() {
			return NODES;
		}
		
		int retornaComplex() {
			return COMPLEX;
		}

		int retornaRegiao(int nivel, int rotulo, Imagem *cc) { // retorna a area
			Fila       *fifo  = NULL;
			Vizinhanca *vz, *a= NULL;
			Pilha      *pilha = NULL;
			int         i, j, valor, area = 0;
			if (cc->bits == BINARIA) valor = 1; else valor = nivel;
			for (i=0; i < altura; i++) //podia nao ser for pois quando if é satisfeito, o for deve ser quebrado!
				for (j=0; j < largura; j++) {
					if (img[i][j]==nivel && status[i][j]==rotulo) {
						pilha = pilha->inserirNaPilha(i,j,1); //primeira e unica semente de inundação
						if (cc->matriz[i][j] < valor) {
							cc->matriz[i][j] = valor; area++;
							while (pilha) {
								fifo  = fifo->inserir(pilha->getPx(),pilha->getPy(),255); //insere na fila pra poder achar os vizinhos
								vz    = vizinhos(fifo); //encontra os vizinhos
								pilha = pilha->removerDaPilha(); //remove da pilha pois o valor ja foi usado!  
								delete fifo; fifo = NULL;
								while (vz) {
									if ((img[vz->getX()][vz->getY()] >= nivel) && (cc->matriz[vz->getX()][vz->getY()] < valor)) {
										pilha = pilha->inserirNaPilha(vz->getX(),vz->getY(),255);
										cc->matriz[vz->getX()][vz->getY()] = valor; area++;
									}
									a  = vz; vz = vz->proximo; delete a;
								}
							}
						}
						return area; //quebra o for!!
					}
				}
			return -1; //fracasso
		}

		int retornaArea(int nivel, int rotulo) {
			Imagem cc = imZeros(altura, largura, 1, BINARIA);
			int area = retornaRegiao(nivel, rotulo, &cc);
			return area;
		}

		Grafo *retornaPai(int nivel, int rotulo) {
			Grafo *filho = grafo;
			while (filho) {
				if (filho->getNivel()==nivel && filho->getRotulo()==rotulo)
					return filho->pai;
				filho = filho->proximo;
			}
			return NULL;  //fracasso!
		}

};
//------------------fim---classe Maxtree principal-------------------










//----------------- MAIN FUNCTIONS ------------------
MaxTree *img2tree(const Imagem& f, const Estruturante& se);
Imagem tree2img(MaxTree *mt);
Imagem tree2aux(MaxTree *mt);
Imagem marker2region(MaxTree *mt, Pontos pt);  //regiao a partir de marcadores


//----------------- FILTERING FUNCTIONS ------------------
void filterSalientPeaks(MaxTree *mt);
void filterWithoutBrothers(MaxTree *mt);
void filterNodesDegree(MaxTree *mt, int infDegree, int supDegree);
void filterNodesArea(MaxTree *mt, int infArea, int supArea);


//----------------- SEGMENTATION FUNCTIONS ------------------
Imagem kmax(MaxTree *mt, int kup, int kdown, int max_dif_level, int amin, int amax);
Imagem regionsFromTwoPoints(MaxTree *mt, Pontos m1, Pontos m2);
Imagem regionsFromTwoMarkers(MaxTree *mt, Pontos m1, Pontos m2);


//----------------- RECOGNITION FUNCTIONS ------------------
bool isArch(const Imagem& f, int dmin, int dmax);
bool isCircle(const Imagem& f, int dmin, int dmax);
bool isEllipse(const Imagem& f, int dmin, int dmax);
bool isLine(const Imagem& f, int dmin, int dmax);
bool isStraightLine(const Imagem& f, int dmin, int dmax);
void *searchShape(void *arg);
void grava_n_threads(int n);
void *consumidor(void *arg);
void passe_de_paremetros(int dmin1, int dmax1, int amin1, int amax1, Imagem *g_l, Imagem *f_l);
void imprime_numero_de_nos_detectados();

//----------------- CLASSIFICATION FUNCTIONS ------------------
int *maxHistogram(MaxTree *mt, int (*Statistic)(const Imagem& f, int dmin, int dmax), int *ha=NULL, int *hp=NULL, int *hg=NULL, int *hn=NULL);
void maxCompare(MaxTree *mt1, MaxTree *mt2);
