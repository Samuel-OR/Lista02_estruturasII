#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define discos 4
#define pinos 3

typedef struct{
	int nro_vertices;
	int** arestas;
	int eh_ponderado;
	float** pesos;
	int* grau;
	int grau_max;
	int nivel[81];
}Grafo;

typedef struct pi PILHA;
struct pi{
	int n;
    PILHA *prox;
};

typedef struct m_pilha maior_pilha;
struct m_pilha{
	float qtd_elementos;
	PILHA *caminho;
};

typedef struct fila Fila;
struct fila{
    int vertice;
    struct fila *prox;    
};


//Funções referntes ao CRUD de uma Pilha
PILHA *inserirPilha(PILHA *p, int v){
	PILHA *cel=(PILHA*)malloc(sizeof(PILHA));
    cel->n=v;
    cel->prox=p;
    return cel;
}
void mostrarPilha(PILHA *pilha){
	if(pilha != NULL){
		mostrarPilha(pilha->prox);
		printf("%d ", pilha->n+1);
	}
}
void removerPilha(PILHA **pilha){
	PILHA *aux;
    aux=*pilha;
    if(*pilha != NULL){
        (*pilha)=(*pilha)->prox;
        free(aux);
        aux = NULL;
    }
}
void copiarPilha(PILHA **p1,PILHA **p2){
	PILHA *aux = (PILHA *)malloc(sizeof(PILHA));
	if(*p1 == NULL){
        *p2 = NULL;
    }else{
        copiarPilha(&(*p1)->prox,p2);
        aux->n = (*p1)->n;
        aux->prox=*p2;
        *p2 = aux;
    }
}


//Funçõs para criar o grafo e gerar os estados da torre de hanói
Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado){
	Grafo *gr;
	gr = (Grafo*) malloc(sizeof(Grafo));

	if( gr != NULL){
		int i;
		gr->nro_vertices = nro_vertices;
		gr->eh_ponderado = (eh_ponderado != 0) ? 1:0;

		gr->grau=(int*)calloc(nro_vertices, sizeof(int));
		gr->arestas = (int**)malloc(nro_vertices*sizeof(int*));
		for(i=0; i<nro_vertices; i++){
			gr->arestas[i] = (int*)malloc(grau_max*sizeof(int*));
			for(int j=0; j<grau_max; j++){
				gr->arestas[i][j] = 0;
			}
		}
		if(gr->eh_ponderado == 1){
			gr->pesos = (float**)malloc(nro_vertices*sizeof(float*));
			for(int j=0; j<nro_vertices; j++){
				gr->pesos[j] = (float*)malloc(nro_vertices*sizeof(float));
			}
		}
		
	}
	return gr;
}
int **gerarPossibilidades(int possibilidades){
	int cont=0;
	int **estadoTorre = malloc(sizeof(int*)*possibilidades);;
	for(int x=0;x< pinos ;x++){
		for(int y=0;y< pinos;y++){
			for(int z=0;z<pinos;z++){
				for(int w=0;w<pinos; w++){
					estadoTorre[cont] =  malloc(sizeof(int)*discos);
					estadoTorre[cont][0] = x+1;
					estadoTorre[cont][1] = y+1;
					estadoTorre[cont][2] = z+1;
					estadoTorre[cont][3] = w+1;
					cont++;
				}
			}
		}
	}
	return estadoTorre;
}


//Funções para validar uma adjacência.
int qtdMovimentos(int* estadoTorre1, int* estadoTorre2){
	int cont = 0;
	for (int x=0; x < discos; x++){
		if(estadoTorre2[x] != estadoTorre1[x])
			cont++;
	}
	return cont;
}
int posMovimento(int* estadoTorre1, int* estadoTorre2){
	int pos = -1;
	int cont = 0;
	for (int x=0; x < discos; x++){
		if(estadoTorre2[x] != estadoTorre1[x]){
			cont++;
			pos = x;
		}
	}
	if(cont!=1)
		cont = -1;
	return pos;
}
int discoMenorAcima(int* estadoTorre, int pos){
	int resul = 1;
	for (int x=pos+1; x < discos; x++){
		if(estadoTorre[x] == estadoTorre[pos])
			resul = 0;
	}
	return resul;
}
int compararEstadoTorre(int* estadoTorre1, int* estadoTorre2){
	int resul = 0;
	if( qtdMovimentos(estadoTorre1, estadoTorre2)==1 ){
		int pos = posMovimento(estadoTorre1, estadoTorre2);
		if(  pos != -1){
			if(discoMenorAcima(estadoTorre1, pos) == 1){
				if(discoMenorAcima(estadoTorre2, pos) == 1){
					resul = 1;
				}

			}
		}
	}
	return resul;
}


//Funções para construir o grafo.
int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso){
	int resul = 0;
	if(gr != NULL){
		if(( orig >= 0 && orig <= gr->nro_vertices) && (dest > 0 && dest <= gr->nro_vertices)){		
			gr->arestas[orig-1][gr->grau[orig-1]] = dest;
			if(gr->eh_ponderado)
				gr->pesos[orig-1][gr->grau[orig-1]] = peso;
			gr->grau[orig-1]++;

			if(eh_digrafo == 0)
				insereAresta(gr,dest,orig,1,peso);
			resul = 1;
		}
	}
	return resul;
}
void construirGrafo(Grafo *grafo, int **estadoTorre, int possibilidades){
	int aux =0 ;
	for(int x = 0; x<possibilidades; x++){
		for(int y = 0; y<possibilidades; y++){
			if(compararEstadoTorre(estadoTorre[x], estadoTorre[y]) == 1){
				insereAresta(grafo, x+1, y+1, 1, 0);
			}
		}

	}
}


//Funcção para realizar a busca no grafo
void buscaProfundidade(Grafo *gr, int ini, int *visitado, int cont, PILHA *pilha, maior_pilha *maior_p,  maior_pilha *menor_p){

	int i;
	visitado[ini] = -1;
	pilha = inserirPilha(pilha,ini);

	for(i=0; i<gr->grau[ini]; i++){
		if(!visitado[gr->arestas[ini][i]-1] && gr->nivel[gr->arestas[ini][i]-1] >= gr->nivel[ini]){
			buscaProfundidade(gr, gr->arestas[ini][i]-1, visitado, cont+1, pilha, maior_p, menor_p);
		}
	}
	
	if(ini == 80){
		if(cont > maior_p->qtd_elementos){
			free(maior_p->caminho);
			maior_p->caminho=NULL;
			copiarPilha(&pilha,&maior_p->caminho);
			maior_p->qtd_elementos=cont;
		}

		if(cont < menor_p->qtd_elementos){
			free(menor_p->caminho);
			menor_p->caminho=NULL;
			copiarPilha(&pilha,&menor_p->caminho);
			menor_p->qtd_elementos=cont;
		}
	}
	removerPilha(&pilha);
	visitado[ini] = 0;
}
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado, int possibilidades){

	maior_pilha *maior_p = (maior_pilha*)malloc(sizeof(maior_pilha));
	maior_p->qtd_elementos = 0;
	maior_p->caminho = NULL;

	maior_pilha *menor_p = (maior_pilha*)malloc(sizeof(maior_pilha));
	menor_p->qtd_elementos = INFINITY;
	menor_p->caminho = NULL;

	PILHA *pilha = (PILHA*)malloc(sizeof(PILHA));
	pilha = NULL;
	
	int i, cont = 0;
	for(i=0; i<gr->nro_vertices; i++){
		visitado[i] = 0;
	}

	buscaProfundidade(gr, ini, visitado, cont, pilha, maior_p, menor_p);

	printf("\nMaior Caminho: ");
	mostrarPilha(maior_p->caminho);
	printf("\nQuantidade de cidades: %f\n",maior_p->qtd_elementos);

	printf("\nMenor Caminho: ");
	mostrarPilha(menor_p->caminho);
	printf("\nQuantidade de cidades: %f\n",menor_p->qtd_elementos);
}


//Funções para colocar nivel nos vertices do grafo.
Fila *appendFila(Fila *F, int raiz){
    Fila *novo, *aux;
    novo = (Fila*)malloc(sizeof(Fila));
    novo->vertice = raiz;
    if(F==NULL){
        F = novo;
        novo->prox = NULL;
    }else{
        for(aux=F; aux->prox!=NULL; aux=aux->prox);
        aux->prox=novo;
        novo->prox = NULL;
    }
    return F;
}
Fila *deleteFila(Fila *F){
    Fila *aux;
    aux = F->prox;
    F = NULL;
    free(F);
    return aux;
}
void LiberaF(Fila **p){
    if(*p==NULL){
        *p=NULL;
    }else{
        LiberaF(&(*p)->prox);
        Fila *aux=*p;
        *p=NULL;
        free(aux);
    }
}
void copiaFila(Fila **p1,Fila **p2){
    Fila *aux=(Fila*)malloc(sizeof(Fila));
    if(*p1 == NULL){
        *p2 = NULL;
    }else{
        copiaFila(&(*p1)->prox,p2);
        aux->vertice = (*p1)->vertice;
        aux->prox=*p2;
        *p2 = aux;
    }
}
void nivelLargura(Grafo *gr, int ini){
	int *pontVertice, *visitados;
	int altura=0, vertice, contido;
	Fila *visitar = NULL, *aux = NULL;

	visitar = appendFila(visitar, 0);

	visitados = (int*)calloc(gr->nro_vertices, sizeof(int));
    visitados[ini] = 1;
	
	visitar = appendFila(visitar, ini);
	
	do{
		LiberaF(&aux);
		while(visitar != NULL){
			vertice = visitar->vertice;
			pontVertice = gr->arestas[vertice];
			for(int i=0; i<gr->grau[vertice]; i++){
				contido = pontVertice[i]-1;
				if(visitados[contido]==0){
					visitados[contido] = 1;
					aux = appendFila(aux, contido);
				}
			}
			gr->nivel[vertice] = altura;
			visitar = deleteFila(visitar);
		}
		copiaFila(&aux, &visitar);
		altura++;
	}while(aux!=NULL);
}

//Converter entrada do usuario
int equivalente(int **estadoTorre, int *vet){
	int resul = 0;
	for(int i=0; i<81; i++){
        if(estadoTorre[i][0] == vet[0] && estadoTorre[i][1] == vet[1] && 
           estadoTorre[i][2] == vet[2] && estadoTorre[i][3] == vet[3]){
            resul = i;
        }
    }
    printf("POSIÇÃO: %d\n",resul );
    return resul;
}

int main(){
 	
	int possibilidades = pow(pinos,discos);
	
	int **estadoTorre = gerarPossibilidades(possibilidades);

 	Grafo *grafo = cria_Grafo(possibilidades,pinos, 0);
	
	int *visitados = (int*) calloc(possibilidades+1, sizeof(int));
	
	construirGrafo(grafo, estadoTorre, possibilidades);
	
	nivelLargura(grafo,0);
	
	for(int i =0; i<81; i++){
		printf("%d|",i+1);
		for(int x=0; x<grafo->grau[i]; x++){
			printf("%d ",grafo->arestas[i][x] );
		}
		printf("\n");
	}



	int vet[4];
	printf("Digite  posição de inicio (Ex: [1 1 1 1]): ");
	scanf("%d %d %d %d", &vet[0], &vet[1], &vet[2], &vet[3]);
	
	int inicio = equivalente(estadoTorre,vet);
	printf("Inicio: %d (%d)\n",inicio+1,inicio );
	buscaProfundidade_Grafo(grafo, inicio, visitados, possibilidades);
	return 0;
}