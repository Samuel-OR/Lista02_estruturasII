#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DISCOS 4
#define PINOS 3

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct {
    int nVertices, ehPonderado;
    int **pesos, **arestas;
    int *grau, *cor, *dist, *anterior;
} Grafo;

typedef struct Fila{
    int vertice;
    struct Fila *prox; 
} Fila;


void printEstado(int *atual);
int contem(int *lista, int tam, int num);
Grafo * criaGrafo(int vertices, int ehPonderado);
void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo);
int ** montaRota(Grafo **gr, int origem, int destino, int *visitados, int **rotaFinal, int passos);
int ** buscaLargura(Grafo **gr, int origem, int destino, int *visitados, int **rotaFinal, int passos);

// Funções para montar o grafo
void montaHanoi(Grafo **gr, int **estados, int tamanho);
int comparaEstado(int *estado1, int *estado2, int tamanho);
int discoEmBaixo(int *estado, int posDisco, int tamanho);
int discoEmCima(int *estado, int posDisco, int tamanho);
int posDiff(int *vetor1, int *vetor2, int tamanho);
int contEquals(int *vetor1, int *vetor2, int tamanho);

// Funções para registrar rota
int length(int *lista, int tam);
void copyList(int **copia, int *original, int tam, int local);
Fila *enqueue(Fila *fila, int vertice);
Fila *dequeue(Fila **fila);
void showFila(Fila *fila);

int main(){

    int possibilidades = pow(PINOS, DISCOS);

    Grafo *grafo = criaGrafo(possibilidades, 0);

    int **estados = malloc(sizeof(int *)*possibilidades);

    int *visitados = calloc(possibilidades, sizeof(int));
    int **rotaFinal = malloc(sizeof(int*)*2);
    rotaFinal[0] = calloc(possibilidades, sizeof(int));
    rotaFinal[1] = calloc(possibilidades, sizeof(int));

    int cont = 0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                for(int l = 0; l < 3; l++){
                    estados[cont] = malloc(sizeof(int)*DISCOS);
                    estados[cont][0] = i+1;
                    estados[cont][1] = j+1;
                    estados[cont][2] = k+1;
                    estados[cont][3] = l+1;
                    cont++;
                }
            }
        }
    }

    montaHanoi(&grafo, estados, possibilidades);

    rotaFinal = buscaLargura(&grafo, 1, possibilidades, visitados, rotaFinal, 0);
    /*
    printf("Maior rota: ");
    for(int i = 1; i < possibilidades && rotaFinal[0][i] != 0; i++) printf("%d ", rotaFinal[0][i]);
    printf("\n");
    printf("Menor rota: ");
    for(int i = 1; i < possibilidades && rotaFinal[1][i] != 0; i++) printf("%d ", rotaFinal[1][i]);
    printf("\n");
    for(int i = 0; i < possibilidades; i++) printf("%d ", grafo->dist[i]);
    printf("\n");
    */

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < possibilidades; j++) {
            printf("%.2d ", grafo->arestas[j][i]);
        }
        printf("\n");
    }

    for(int i = 0; i < possibilidades; i++) {
        printEstado(estados[i]);
        printf(" %d\n", (i+1));
    }
    return 0;
}

Grafo * criaGrafo(int vertices, int ehPonderado){
    Grafo *gr;
    gr = (Grafo *) malloc(sizeof(Grafo));

    if(gr != NULL){
        gr->nVertices = vertices;
        gr->ehPonderado = (ehPonderado != 0)? 1:0;

        gr->grau = (int*) calloc(vertices, sizeof(int));
        gr->cor = (int*) calloc(vertices, sizeof(int)); 
        gr->dist = (int*) calloc(vertices, sizeof(int));
        gr->anterior = (int*) calloc(vertices, sizeof(int));

        gr->arestas = (int**) malloc(vertices*sizeof(int*));
        for(int i = 0; i < vertices; i++)
            gr->arestas[i] = (int*) calloc(vertices, sizeof(int));

        if(gr->ehPonderado){
            gr->pesos = (int**) malloc(vertices * sizeof(int*));
            for(int i = 0; i < vertices; i++)
                gr->pesos[i] = (int*) calloc(vertices, sizeof(int));
        }

    }

    return gr;
}

int ** buscaLargura(Grafo **gr, int origem, int destino, int *visitados, int **rotaFinal, int passos){
    (*gr)->cor[origem-1] = GRAY;
    (*gr)->dist[origem-1] = 0;
    (*gr)->anterior[origem-1] = -1;
    Fila *agendados = NULL;

    agendados = enqueue(agendados, origem);
    while(agendados != NULL){
        Fila *aux = dequeue(&agendados);
        for(int i = 0; i < (*gr)->grau[aux->vertice-1]; i++){
            if((*gr)->cor[((*gr)->arestas[aux->vertice-1][i])-1] == WHITE){
                (*gr)->cor[((*gr)->arestas[aux->vertice-1][i])-1] = GRAY;
                (*gr)->dist[((*gr)->arestas[aux->vertice-1][i])-1] = ((*gr)->dist[aux->vertice-1])+1;
                (*gr)->anterior[((*gr)->arestas[aux->vertice-1][i])-1] = aux->vertice;
                agendados = enqueue(agendados, (*gr)->arestas[aux->vertice-1][i]);
            }
        }
        (*gr)->cor[aux->vertice-1] = BLACK;
    }
    return montaRota(gr, origem, destino, visitados, rotaFinal, passos);
}

int ** montaRota(Grafo **gr, int origem, int destino, int *visitados, int **rotaFinal, int passos){
    if(gr != NULL) {
        int tamanho = pow(PINOS, DISCOS);
        visitados[passos] = origem;

        if(length(rotaFinal[0], tamanho) < length(visitados, tamanho)){
            if(contem(visitados, tamanho, tamanho)) copyList(rotaFinal, visitados, tamanho, 0);
        } else if(length(rotaFinal[1], tamanho) > length(visitados, tamanho) || length(rotaFinal[1], tamanho) == 0){
            if(contem(visitados, tamanho, tamanho)) copyList(rotaFinal, visitados, tamanho, 1);
        }

        if(origem != destino){
            for(int i = 0; i < (*gr)->grau[origem-1]; i++){
                if(!contem(visitados, passos, (*gr)->arestas[origem-1][i]) && (*gr)->dist[(*gr)->arestas[origem-1][i]-1] >= (*gr)->dist[origem-1]){
                    int **retorno = montaRota(gr, (*gr)->arestas[origem-1][i], destino, visitados, rotaFinal, passos+1);
                    if(length(*rotaFinal, tamanho) < length(*retorno, tamanho)) rotaFinal = retorno;
                    visitados[passos+1] = 0;
                }
            }
        }

    }
    return rotaFinal;
}

void printEstado(int *atual){
    printf("(%d,%d,%d,%d)", atual[0], atual[1], atual[2], atual[3]);
}


//MONTAR GRAFO

    void montaHanoi(Grafo **gr, int **estados, int tamanho){
        for(int i = 0; i < tamanho; i++){
            for(int j = 0; j < tamanho; j++){
                if(comparaEstado(estados[j], estados[i], DISCOS)){
                    inserirAresta(gr, i+1, j+1, 0, 1);
                }
            }
        }
    }

    void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo){
        if(gr != NULL){ 
            if(origem > 0 && origem <= (**gr).nVertices){
                if(destino > 0 && destino <= (**gr).nVertices){
                    (**gr).arestas[origem-1][(**gr).grau[origem-1]] = destino;

                    if((**gr).ehPonderado)
                        (**gr).pesos[origem-1][(**gr).grau[origem-1]] = peso;

                    (**gr).grau[origem-1]++;

                    if(!ehDigrafo){
                        inserirAresta(gr, d5estino, origem, peso, 1);
                    }
                }
            }
        }
    }

    int comparaEstado(int *estado1, int *estado2, int tamanho){
        int temAresta = 0;

        if(contEquals(estado1, estado2, tamanho) == DISCOS-1){
            int posicao = posDiff(estado1, estado2, tamanho);
            if(posicao != -1){
                if(!discoEmCima(estado1, posicao, tamanho)){
                    if(!discoEmBaixo(estado2, posicao, tamanho)){
                        temAresta = 1;
                    }
                }
            }
        }

        return temAresta;
    }

    int discoEmBaixo(int *estado, int posDisco, int tamanho){
        int discoAbaixo = 0;
        for(int i = posDisco+1; i < tamanho; i++) 
            if(estado[i] == estado[posDisco]) discoAbaixo = 1;
        return discoAbaixo;
    }

    int discoEmCima(int *estado, int posDisco, int tamanho){
        int discoAcima = 0;
        for(int i = posDisco+1; i < tamanho; i++) 
            if(estado[i] == estado[posDisco]) discoAcima = 1;
        return discoAcima;
    }

    int posDiff(int *vetor1, int *vetor2, int tamanho){
        int pos = -1;
        for(int i = 0; i < tamanho; i++) if(vetor1[i] != vetor2[i]) pos = i;
        return pos;
    }

    int contEquals(int *vetor1, int *vetor2, int tamanho){
        int cont = 0;
        for(int i = 0; i < tamanho; i++) if(vetor1[i] == vetor2[i]) cont++;
        return cont;
    }

//FIM MONSTAR GRAFO

int contem(int *lista, int tam, int num){
    int contem = 0;

    for(int i = 0; i < tam; i++)
        if(lista[i] == num) contem = 1;
    
    return contem;
}

int length(int *lista, int tam){
    int cont = 0;
    for(int i = 1; i < tam && lista[i] != 0; i++, cont++);
    return cont;
}

void copyList(int **copia, int *original, int tam, int local){
    int i = 0;
    for(i = 0; i < tam && original[i] != 0; i++, copia[local][i] = original[i]);
    for(i; i < tam; i++) copia[local][i] = 0;
}

Fila *enqueue(Fila *fila, int vertice){
    Fila *elemento = malloc(sizeof(Fila));
    elemento->vertice = vertice;
    elemento->prox = NULL;

    Fila *aux;
    if(fila != NULL){
        for(aux = fila; aux != NULL && aux->prox != NULL; aux = aux->prox);
 
        if(aux == NULL) fila = elemento;
        else aux->prox = elemento;
    }
    else fila = elemento;
    
    return fila;
}

Fila *dequeue(Fila **fila){
    Fila *aux = (*fila);
    (*fila) = (*fila)->prox;
    return aux;
}

void showFila(Fila *fila){
    if(fila != NULL){
        printf("%d ", fila->vertice);
        showFila(fila->prox);
    }
}