#include<stdio.h>
#include<stdlib.h>
#define TAM 4
typedef struct{
    int nro_vertices;
    int eh_ponderado;
    int **arestas;
    int *grau;
}Grafo;

struct pilha{
    int n;
    struct pilha *prox;
};
typedef struct pilha Pilha;

int **gerarEstados(){
    int **estados = (int**)malloc(sizeof(int*)*31);
    int cont = 0;

    for(int x=0; x < 31; x++){
        estados[x]=(int*)calloc(4,sizeof(int));
    }

    for(int x=-1; x < 2; x+=2){
        for(int y=-1; y < 2; y+=2){
            for(int z=-1; z < 2; z+=2){
                for(int w=-1; w < 2; w+=2){
                    estados[cont][0] = x;
                    estados[cont][1] = y;
                    estados[cont][2] = z;
                    estados[cont][3] = w;
                    cont++;
                }
            }
        }
    }

    for(int x=-1; x < 2; x+=2){
        for(int y=-1; y < 2; y+=2){
            for(int z=-1; z < 2; z+=2){
                estados[cont][0] = x;
                estados[cont][1] = y;
                estados[cont][2] = z;
                cont++;
            }
        }
    }

    for(int x=-1; x < 2; x+=2){
        for(int y=-1; y < 2; y+=2){
            estados[cont][0] = x;
            estados[cont][1] = y;
            cont++;
        }
    }

    for(int x=-1; x < 2; x+=2){
        estados[cont][0] = x;
        cont++;
    }
    estados[cont][0] = 0;
    return estados;
}

int mostrarEstados(int **estados,int indiceOUTodos){
    if( indiceOUTodos == 31){
        for(int x=0; x<31; x++){
            printf("%d | %d %d %d %d\n",x, estados[x][0], estados[x][1], estados[x][2], estados[x][3] );
        }
    }else{
        printf("%d | %d %d %d %d\n",indiceOUTodos
            ,estados[indiceOUTodos][0], estados[indiceOUTodos][1]
            ,estados[indiceOUTodos][2], estados[indiceOUTodos][3]
            );
    }
}

Grafo* cria_Grafo(int nro_vertices){
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(Grafo));
    if( gr != NULL){
        gr->nro_vertices = nro_vertices;
        gr->grau=(int*)calloc(nro_vertices, sizeof(int));
        gr->arestas = (int**)malloc(nro_vertices*sizeof(int*));
        for(int i=0; i<nro_vertices; i++){
            gr->arestas[i] = (int*)calloc(nro_vertices, sizeof(int));
        }
    }
    return gr;
}

void insereAresta(Grafo **gr, int origem, int destino){
    if(*gr != NULL){
        if(origem >= 0 && origem <= (*gr)->nro_vertices){
            if(destino > 0 && destino <= (*gr)->nro_vertices){
                (*gr)->arestas[origem][(*gr)->grau[origem]] = destino;
            }
            (*gr)->grau[origem]++;
        }
    }
}



int tamaEst(int *estado){
    int tam=0;
    for(tam=0;estado[tam]!=0;tam++);
    return tam;
}

int igual(int *est,int *est2){
    int t=tamaEst(est2);
    int retorno=1;
    for(int i=0;i<t && retorno==1;i++){
        if(est[i]!=est2[i]) retorno=0;
    }
    return retorno;
}




//#############
int temColisao(int *estado){
    int resul=0;
    int tam=tamaEst(estado);
    for(int i=0;i<tam-1 && resul==0;i++)
        if(estado[i]==1 && estado[i+1]==-1) resul=1;
    return resul;
}

int temSaida(int *estado){
    int retorno=0;
    int tam=tamaEst(estado);
    if(estado[0]==-1 || estado[tam-1]==1) retorno=1;
    return retorno;
}

int *tira(int *estado){
    int ini=0,end=tamaEst(estado),novoTam=end;
    if(estado[ini]==-1){
        novoTam-=1;
        ini=1;
    }
    if(estado[end-1]==1){
        novoTam-=1;
        end-=1;
    }
    int *NovoEstado=(int*)malloc(sizeof(int)*novoTam);
    int cont=0;
    for(int i=ini;i<end;i++,cont++){
        NovoEstado[cont]=estado[i];
    }
    return NovoEstado;
}

int *tiraC(int *estado){
    int tam=tamaEst(estado);
    int *prox=(int*)malloc(sizeof(int*)*tam);
    for(int i=0;i<tam;i++){
        prox[i]=estado[i];
    }
    for(int i=0;i<tam-1;i++){
        if(prox[i]==1 && prox[i+1]==-1){
            prox[i]*=-1;
            prox[i+1]*=-1;
            i++;
        }
    }
    return prox;
}

int *EstadoAdj(int *estAtual){
    int *prox;
    if(temColisao(estAtual)){
        if(temSaida(estAtual)){
            prox=tira(estAtual);
            estAtual=prox;
        }
        prox=tiraC(estAtual);

    }else if(temSaida(estAtual)){
        prox=tira(estAtual);
    }
    return prox;
}

//#############
int equivalenteIndice(int **estado,int *vet){
    int resul = 0;
    for(int i=0; i< 31; i++){
        if( estado[i][0]==vet[0] &&
            estado[i][1]==vet[1] &&
            estado[i][2]==vet[2] &&
            estado[i][3]==vet[3]){
            resul = i;
        }
    }
    return resul;
}

int equivalenteEstado(int* est1, int* est2){
    int t=tamaEst(est1);
    int retorno=1;
    for(int i=0;i<t && retorno==1;i++){
        if(est1[i]!=est2[i]) retorno=0;
    }
    return retorno;
}

int equivalenteTamanho(int* est1, int* est2){
    int resul = 0;
    int t1=tamaEst(est1);
    int t2=tamaEst(est2);
    if(t1==t1)
        resul = 1;
    return resul;
}


int EstadoDestino(int **estados,int *estado){
    int resul = -1;
    for(int i=0;i<31 && resul==-1;i++){
        if(equivalenteTamanho(estados[i], estado)){
            if(equivalenteEstado(estados[i], estado)){
                resul=i;    
            }

        }
    }
    return resul;
}

void construirGrafo(Grafo *gr,int **estados, int qtd){
    for(int i=0;i<qtd;i++){
        
        int *adj =EstadoAdj(estados[i]);
        int destino = EstadoDestino(estados,adj);
        //printf("DES: %d\n",destino );
        insereAresta(&gr,i,destino);

    }
}

//#########################
void push(int v, Pilha **p){
    Pilha *cel=(Pilha*)malloc(sizeof(Pilha));
    cel->n=v;
    cel->prox=*p;
    *p=cel;
}

void buscarCaminho(Grafo *gr, int inicio, Pilha **pilha, int *cont ){
    push(inicio,pilha);
    for(int i=0; i<gr->grau[inicio]; i++){
        *cont+=1;
        buscarCaminho(gr, gr->arestas[inicio][i], pilha, cont);
    }
}

void exibePilha(Pilha *P,int **est){
    Pilha *aux = P;
    if(aux!=NULL){
        exibePilha(P->prox,est);
        printf("%d:",aux->n);
        mostrarEstados(est, aux->n);
    }
}

int main(){
    
    Pilha *pilha=NULL;
    int **estados, vet[TAM], cont;

    Grafo *grafo = cria_Grafo(31);
    estados = gerarEstados(TAM);
    
    //mostrarEstados(estados, 31);
    construirGrafo(grafo,estados, 30);
    

    /*for(int i=0;i<31;i++){
        printf("Estado ");
        printf(" [");
        mostrarEstados(estados, i);
        printf("] ");
        for(int j=0;j<grafo->grau[i];j++){
            printf("Vai para o Estado ");
            mostrarEstados(estados, grafo->arestas[i][j]);
        }
        printf("\n");
    }*/


    printf("Insira posição inicial (ex: [1 -1 1 -1].\n");
    printf("->");
    scanf("%d %d %d %d", &vet[0],&vet[1],&vet[2],&vet[3]);
    int inicio = equivalenteIndice(estados,vet);

   

    buscarCaminho(grafo, inicio, &pilha, &cont);
    exibePilha(pilha, estados);
    

    return 0;
}
