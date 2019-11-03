#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
    int nivel[81];
};

typedef struct grafos Grafos;

struct torre{
    Grafos *gr;
    int **estado;
};

struct fila{
    int vertice;
    struct fila *prox;    
};
typedef struct fila Fila;

struct pilha{
    int n;
    struct pilha *prox;
};

struct maior{
    float tam1, tam2;
    struct pilha *caminho1, *caminho2;
};


typedef struct pilha Pilha;
typedef struct maior Maior;
typedef struct torre Torre;

Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
void buscaProf(Grafos *gr, int raiz, int *visitado, Maior *cam);
void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, Pilha **caminho, int cont, Maior *cam);
void push(int v, Pilha **p);
void pop(Pilha **p);
void copiaPilha(Pilha **p1,Pilha **p2);
void mostraCam(Pilha *caminho, Torre *hanoi);
void LiberaP(Pilha **p);
void conectar(Torre *hanoi);
int move1Disco(Torre *hanoi, int origem, int destino, int *pos);
int podeBotar(Torre *hanoi, int destino, int pos);
void mostraEstados(Torre *hanoi);
void buscaEmLargura(Grafos *gr, int raiz);
void copiaFila(Fila **p1,Fila **p2);
void LiberaF(Fila **p);
Fila *ColocaVerticeNaFila(Fila *F, int raiz);
Fila *RemoveVerticeNaFila(Fila *F);
int VerificaSeTaNaFila(Fila *F, int raiz);
void exibeFila(Fila *F);
int converteEstado(Torre *hanoi, int *estado);

int main(){
    int i, j, k, l;
    Torre *hanoi=(Torre*)malloc(sizeof(Torre)); 
    hanoi->gr=NULL;
    //3^n => 3 ^ 4 == 81
    //Grafos *gr;
    hanoi->gr = criaGrafo(81, 0);
    Maior *cam=(Maior*)malloc(sizeof(Maior));
    cam->tam1=0;
    cam->caminho1=NULL;
    cam->tam2=INFINITY;
    cam->caminho2=NULL;
    int contPos = 0, vis[81], estado[4];
    int pos=0;
    hanoi->estado=(int**)malloc(sizeof(int*)*81);
    for(i=0;i<81;i++){
        hanoi->estado[i]=(int*)malloc(sizeof(int)*4);
    }
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            for(k=0; k<3; k++){
                for(l=0; l<3; l++){
                    hanoi->estado[contPos][0] = i+1;
                    hanoi->estado[contPos][1] = j+1;
                    hanoi->estado[contPos][2] = k+1;
                    hanoi->estado[contPos][3] = l+1;
                    contPos++;
                }
            }
        }
    }
    contPos = 0;
    conectar(hanoi);
    buscaEmLargura(hanoi->gr,0);
    //mostraEstados(hanoi);
    printf("Digite o estado inicial da torre[numeros separados por espaco]:\n");
    scanf("%d%d%d%d", &estado[0], &estado[1], &estado[2], &estado[3]);
    buscaProf(hanoi->gr, converteEstado(hanoi, estado), vis, cam);
    printf("O maior caminho possivel eh:\n");
    mostraCam(cam->caminho1, hanoi);
    printf("O menor caminho possivel eh:\n");
    mostraCam(cam->caminho2, hanoi);
    return 0;
}

int converteEstado(Torre *hanoi, int *estado){
    int i, result = -1;
    for(i=0; i<81; i++){
        if(hanoi->estado[i][0] == estado[0] && hanoi->estado[i][1] == estado[1] && hanoi->estado[i][2] == estado[2] && hanoi->estado[i][3] == estado[3]){
            result = i;
        }
    }
    return result;
}

void mostraCam(Pilha *caminho, Torre *hanoi){
    if(caminho==NULL){
    }else{
        mostraCam(caminho->prox, hanoi);
        printf("%d: %d,%d,%d,%d\n",caminho->n, hanoi->estado[caminho->n][0], hanoi->estado[caminho->n][1], hanoi->estado[caminho->n][2], hanoi->estado[caminho->n][3]);
    }
}

Grafos* criaGrafo(int vertice, int ehPonderado){
    Grafos *gr;
    int i;
    gr = (Grafos*)malloc(sizeof(Grafos));
    if(gr != NULL){
        gr->nVertices = vertice;
        gr->ehPonderado = (ehPonderado != 0)? 1:0;
        gr->grau = (int*) calloc(sizeof(int), vertice);
        gr->arestas = (int**) malloc(vertice * sizeof(int*));
        for(i=0; i<vertice; i++){
            gr->arestas[i] = (int*) calloc(vertice, sizeof(int));
        }
        if(gr->ehPonderado){
            gr->pesos = (int**) malloc(vertice * sizeof(int*));
            for(i=0; i<vertice; i++){
                gr->pesos[i] = (int*) calloc(vertice, sizeof(int));
            }   
        }
    }
    return gr;
}

void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo){
    if(*gr != NULL){
        if(origem >= 0 && origem <= (*gr)->nVertices){
            if(destino > 0 && destino <= (*gr)->nVertices){
                (*gr)->arestas[origem][(*gr)->grau[origem]] = destino;
            }
            if((*gr)->ehPonderado){
                (*gr)->pesos[origem][(*gr)->grau[origem]] = peso;
            }
            (*gr)->grau[origem]++;
            if(!ehDigrafo){
                insereAresta(gr, destino, origem, peso, 1);
            }
        }
    }
}

int move1Disco(Torre *hanoi, int origem, int destino, int *pos){
    int i, cont = 0;
    for(i=0; i<4; i++){
        if(hanoi->estado[origem][i] != hanoi->estado[destino][i]){
            *pos = i;
            cont++;
        }
    }
    if(cont != 1){
        cont = 0;
    }
    return cont;
}

int podeBotar(Torre *hanoi, int destino, int pos){
    int i, result = 1;
    for(i=pos+1; i<4; i++){
        if(hanoi->estado[destino][pos] == hanoi->estado[destino][i]){
            result = 0;
        }
    }
    return result;
}

void conectar(Torre *hanoi){
    int i, j, pos;
    for(i=0; i<81; i++){
        for(j=i; j<81; j++){
            if(move1Disco(hanoi, i, j, &pos)){
                if(podeBotar(hanoi, i, pos)){
                    if(podeBotar(hanoi, j, pos)){
                        insereAresta(&hanoi->gr, i, j, 0, 0);
                    }
                }
            }
        }
    }
}

void mostraEstados(Torre *hanoi){
    int i, j, k, l, contPos = 0;
    for(i=0; i<81; i++){
        printf("%d: %d, %d, %d, %d\n",contPos, hanoi->estado[contPos][0], hanoi->estado[contPos][1], hanoi->estado[contPos][2], hanoi->estado[contPos][3]);
        contPos++;
    }
     for(i=0; i<hanoi->gr->nVertices; i++){
         printf("%d: ", i);
        for(j=0; j<hanoi->gr->grau[i]; j++){
            printf("%d ", hanoi->gr->arestas[i][j]);
        }
        printf(" | Altura do no: %d\n",hanoi->gr->nivel[i]);
     }
}

void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, Pilha **caminho, int cont, Maior *cam){
    int i;
    visitado[raiz] = -1;
    push(raiz,caminho);
    for(i=0; i<gr->grau[raiz]; i++){
        if(!visitado[gr->arestas[raiz][i]] && gr->nivel[gr->arestas[raiz][i]] >= gr->nivel[raiz]){
            buscaEmProfundidade(gr, gr->arestas[raiz][i], visitado, caminho, cont+1, cam);
        }
    }
    if(raiz == 80){
        if(cont>cam->tam1){
            LiberaP(&cam->caminho1);
            cam->caminho1=NULL;
            copiaPilha(caminho,&cam->caminho1);
            cam->tam1=cont;
        }
        if(cont < cam->tam2){
            LiberaP(&cam->caminho2);
            cam->caminho2=NULL;
            copiaPilha(caminho,&cam->caminho2);
            cam->tam2=cont;
        }
    }
    pop(caminho);
    visitado[raiz]=0;
}


void buscaProf(Grafos *gr, int raiz, int *visitado, Maior *cam){
    int i, cont = 1;
    for(i=0; i<gr->nVertices; i++){
        visitado[i] = 0;
    }
    Pilha *Caminho=(Pilha*)malloc(sizeof(Pilha));
    Caminho=NULL;
    cont=0;
    buscaEmProfundidade(gr, raiz, visitado, &Caminho, cont, cam);
}

void push(int v, Pilha **p){
    Pilha *cel=(Pilha*)malloc(sizeof(Pilha));
    cel->n=v;
    cel->prox=*p;
    *p=cel;
}

void pop(Pilha **p){
    Pilha *aux;
    aux=*p;
    if(*p != NULL){
        (*p)=(*p)->prox;
        free(aux);
        aux = NULL;
    }
}

void copiaPilha(Pilha **p1,Pilha **p2){
    Pilha *aux=(Pilha*)malloc(sizeof(Pilha));
    if(*p1 == NULL){
        *p2 = NULL;
    }else{
        copiaPilha(&(*p1)->prox,p2);
        aux->n = (*p1)->n;
        aux->prox=*p2;
        *p2 = aux;
    }
}

void LiberaP(Pilha **p){
    if(*p==NULL){
        *p=NULL;
    }else{
        LiberaP(&(*p)->prox);
        Pilha *aux=*p;
        *p=NULL;
        free(aux);
    }
}

void buscaEmLargura(Grafos *gr, int raiz){
    Fila *F=NULL,*F2=NULL;
    int i,cont = 0, j = 0;
    int vertice1,altura=0;
    int *pointer;
    int *marcados;
    int elemento;
    marcados = (int*)calloc(gr->nVertices, sizeof(int));
    marcados[raiz] = 1;
    cont++;
    F = ColocaVerticeNaFila(F, raiz);
    do{
        LiberaF(&F2);
        while(F!=NULL){
            vertice1 = F->vertice;
            pointer = gr->arestas[vertice1];
            for(int i=0; i<gr->grau[vertice1]; i++){
                elemento = pointer[i];
                if(marcados[elemento]==0){
                    //printf("%d visitou %d.\n", vertice1, elemento);
                    marcados[elemento]=1;
                    F2=ColocaVerticeNaFila(F2, elemento);
                //    exibeFila(F);
                }else if(VerificaSeTaNaFila(F, elemento)){
                    //printf("%d visitou %d.\n", vertice1, elemento);
                }
            }
            gr->nivel[vertice1]=altura;
            F=RemoveVerticeNaFila(F);        
            //exibeFila(F);                
        }
        //exibeFila(F2);
        printf("%d\n",altura);
        copiaFila(&F2,&F);
        altura++;
    }while (F2!=NULL);
    
}

void exibeFila(Fila *F){
    Fila *aux = F;
    for(aux;aux!=NULL;aux=aux->prox)
        printf("%d ", aux->vertice);
    puts("\n");
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

Fila *ColocaVerticeNaFila(Fila *F, int raiz){
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

Fila *RemoveVerticeNaFila(Fila *F){
    Fila *aux;
    aux = F->prox;
    F = NULL;
    free(F);
    return aux;
}

int VerificaSeTaNaFila(Fila *F, int raiz){
    Fila *aux = F;
    int retorno = 0;
    for(aux; aux!=NULL; aux=aux->prox){
        if(aux->vertice==raiz){
            retorno=1;
        }
    }
    return retorno;
}