#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int nVertices, ehPonderado;
    int *pesos, *arestas;
} Grafo;

Grafo * criaGrafo(int vertices, int ehPonderado);
void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo);
void geraEstados(int **estados);
int ** montaRota(Grafo **gr, int origem, int destino, int *visitados, int **rotaFinal, int passos);

//Funções para montar grafo
void montaTubo(Grafo **gr, int **estados, int tam);
int * proxEstado(int *estado);
int temColisao(int *estado);    
int temSaida(int *estado);
int * resolveSaida(int *estado);
int * resolveColisao(int *estado);
void copyList(int **copia, int *original, int tam);
int ehIgual(int *estado1, int *estado2);

//Auxiliares
int tamEstado(int *tamEstado);
int contem(int *lista, int tam, int num);
int length(int *lista, int tam);
void printEstado(int *estado);

int main(){

    int possibilidades = 31;

    Grafo *grafo = criaGrafo(possibilidades, 0);

    int **estados = malloc(sizeof(int *)*possibilidades);
    int *visitados = calloc(possibilidades, sizeof(int));
    int **rotaFinal = malloc(sizeof(int*));
    rotaFinal[0] = calloc(possibilidades, sizeof(int));

    geraEstados(estados);

    /* mostrar todos os estados
    for(int i = 0; i < possibilidades; i++) {
        printf("%d ", i+1);
        printEstado(estados[i]);
    }
    */

    montaTubo(&grafo, estados, possibilidades);

    /* mostrar as adjacencias
    for(int i = 0; i < possibilidades; i++) printf("%d ", grafo->arestas[i]);
    printf("\n");
    */

    rotaFinal = montaRota(&grafo, 21, 31, visitados, rotaFinal, 0);
    
    printf("Menor rota: ");
    for(int i = 1; i < possibilidades && rotaFinal[0][i] != 0; i++) printf("%d ", rotaFinal[0][i]);
    printf("\n");
    
    return 0;

}

Grafo * criaGrafo(int vertices, int ehPonderado){
    Grafo *gr;
    gr = (Grafo *) malloc(sizeof(Grafo));

    if(gr != NULL){
        gr->nVertices = vertices;
        gr->ehPonderado = (ehPonderado != 0)? 1:0;

        gr->arestas = (int*) malloc(vertices*sizeof(int));

        if(gr->ehPonderado){
            gr->pesos = (int*) malloc(vertices * sizeof(int));
        }

    }

    return gr;
}

void inserirAresta(Grafo **gr, int origem, int destino, int peso, int ehDigrafo){
    if(gr != NULL){ 
        if(origem > 0 && origem <= (**gr).nVertices){
            if(destino > 0 && destino <= (**gr).nVertices){
                (**gr).arestas[origem-1] = destino;

                if((**gr).ehPonderado)
                    (**gr).pesos[origem-1] = peso;

                if(!ehDigrafo){
                    inserirAresta(gr, destino, origem, peso, 1);
                }
            }
        }
    }
}

void geraEstados(int **estados){
    int cont = 0;

    for(int i = -1; i < 2; i+=2){
        for(int j = -1; j < 2; j+=2){
            for(int k = -1; k < 2; k+=2){
                for(int l = -1; l < 2; l+=2){
                    estados[cont] = calloc(4, sizeof(int));
                    estados[cont][0] = i;
                    estados[cont][1] = j;
                    estados[cont][2] = k;
                    estados[cont][3] = l;
                    //printf("%d (%d,%d,%d,%d)\n", cont+1, estados[cont][0], estados[cont][1], estados[cont][2], estados[cont][3]);
                    cont++;
                }
            }
        }
    }
    
    for(int i = -1; i < 2; i+=2){
        for(int j = -1; j < 2; j+=2){
            for(int k = -1; k < 2; k+=2){
                estados[cont] = calloc(3, sizeof(int));
                estados[cont][0] = i;
                estados[cont][1] = j;
                estados[cont][2] = k;
                //printf("%d (%d,%d,%d)\n", cont+1, estados[cont][0], estados[cont][1], estados[cont][2]);
                cont++;
            }
        }
    }

    for(int i = -1; i < 2; i+=2){
        for(int j = -1; j < 2; j+=2){
            estados[cont] = calloc(2, sizeof(int));
            estados[cont][0] = i;
            estados[cont][1] = j;
            //printf("%d (%d,%d)\n", cont+1, estados[cont][0], estados[cont][1]);
            cont++;
        }
    }
    for(int i = -1; i < 2; i+=2) {
        estados[cont] = calloc(1, sizeof(int));
        estados[cont][0] = i;
        //printf("%d (%d)\n", cont+1, estados[cont][0]);
        cont++;
    }

    estados[cont] = calloc(1, sizeof(int));
    estados[cont][0] = 0;
    //printf("%d (%d)\n", cont+1, estados[cont][0]);
    cont++;
}

int ** montaRota(Grafo **gr, int origem, int destino, int *visitados, int **rotaFinal, int passos){
    if(gr != NULL) {
        int tamanho = 31;
        visitados[passos] = origem;

        if(length(rotaFinal[0], tamanho) < length(visitados, tamanho) || length(rotaFinal[0], tamanho) == 0){
            if(contem(visitados, tamanho, tamanho)) copyList(rotaFinal, visitados, tamanho);
        }

        if(origem != destino){
            int **retorno = montaRota(gr, (*gr)->arestas[origem-1], destino, visitados, rotaFinal, passos+1);
            if(length(*rotaFinal, tamanho) > length(*retorno, tamanho)) rotaFinal = retorno;
            visitados[passos+1] = 0;
        }
    }
    return rotaFinal;
}

void montaTubo(Grafo **gr, int **estados, int tam){
    for(int i = 0; i < tam; i++){
        int *aux = proxEstado(estados[i]);
        for(int j = 0; j < tam; j++){
            if(ehIgual(estados[j], aux)){
                inserirAresta(gr, i+1, j+1, 0, 1);
            }
        }
    }
}

int * proxEstado(int *estado){
    int *aux;
    //printf("estado: "); printEstado(estado);
    if(temColisao(estado)){
        //printf("Tem colisao\n");
        if(temSaida(estado)){
            //printf("Tem saida\n");
            aux = resolveSaida(estado);
            estado = aux;
            //printf("aux pos saida: "); printEstado(aux);
        }
        aux = resolveColisao(estado);
        //printf("aux pos colisao: "); printEstado(aux);
    } else if(temSaida(estado)){
        //printf("Tem saida\n");
        aux = resolveSaida(estado);
        //printf("aux pos saida: "); printEstado(aux);
    }

    //printf("---------------\n");

    return aux;
}

int temColisao(int *estado){
    int houveColisao = 0;
    int tam = tamEstado(estado);
    for(int i = 0; i < tam-1 && houveColisao == 0; i++){
        if(estado[i] == 1 && estado[i+1] == -1) houveColisao = 1;
    }
    return houveColisao;
}

int temSaida(int *estado){
    int houveColisao = 0;
    int tam = tamEstado(estado);
    if(estado[0] == -1 || estado[tam-1] == 1) houveColisao = 1;
    return houveColisao;
}

int * resolveSaida(int *estado){
    int tam = tamEstado(estado);
    int *aux, newTam = tam, i = 0, cont = 0;
    if(estado[0] == -1) {
        newTam -= 1;
        i = 1;
    }
    if(estado[tam-1] == 1) {
        newTam -= 1;
        tam -= 1;
    }

    aux = calloc(newTam, sizeof(int));  
    for(i; i < tam; i++, cont++){
        aux[cont] = estado[i];
    }

    return aux;
}

int * resolveColisao(int *estado){
    int tam = tamEstado(estado);
    int *aux = calloc(tam, sizeof(int));

    for(int i = 0; i < tam; i++){
        aux[i] = estado[i];
    }

    for(int i = 0; i < tam-1; i++){
        if(aux[i] == 1 && aux[i+1] == -1){
            aux[i] *= -1;
            aux[i+1] *= -1;
            i+=1;
        }
    }

    return aux;
}

int ehIgual(int *estado1, int *estado2){
    int ehIgual = 1;

    int tam = tamEstado(estado1);
    if(tam == tamEstado(estado2)){
        for(int i = 0; i < tam && ehIgual != 0; i++){
            if(estado1[i] != estado2[i]) ehIgual = 0;
        }
    } else ehIgual = 0;

    return ehIgual;
}

int tamEstado(int *estado){
    int i;
    for(i = 0; estado[i] != 0; i++);
    return i;
}

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

void copyList(int **copia, int *original, int tam){
    int i = 0;
    for(i = 0; i < tam && original[i] != 0; i++, copia[0][i] = original[i]);
    for(i; i < tam; i++) copia[0][i] = 0;
}

void printEstado(int *estado){
    int tam = tamEstado(estado);
    
    printf("(");
    for(int i = 0; i < tam-1; i++) printf("%d,", estado[i]);
    printf("%d)\n", estado[tam-1]);
}