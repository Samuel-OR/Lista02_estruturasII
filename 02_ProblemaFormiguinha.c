#include<stdio.h>
#include<stdlib.h>
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
                estados[cont][3] = 2;
                cont++;
            }
        }
    }

    for(int x=-1; x < 2; x+=2){
        for(int y=-1; y < 2; y+=2){
            estados[cont][0] = x;
            estados[cont][1] = y;
            estados[cont][2] = 2;
            estados[cont][3] = 2;
            cont++;
        }
    }

    for(int x=-1; x < 2; x+=2){
        estados[cont][0] = x;
        estados[cont][1] = 2;
        estados[cont][2] = 2;
        estados[cont][3] = 2;
        cont++;
    }
    estados[cont][0] = 0;
    estados[cont][1] = 2;
    estados[cont][2] = 2;
    estados[cont][3] = 2;
    return estados;
}
int Length(int *estado){
    int tam=0;
    for(tam=0;estado[tam]!=0;tam++);
    return tam;
}
int igual(int *est,int *est2){
    int t=Length(est2);
    int retorno=1;
    for(int i=0;i<t && retorno==1;i++){
        if(est[i]!=est2[i]) retorno=0;
    }
    return retorno;
}

int FormigaBatendo(int *estado){
    int resul=0;
    int tam=Length(estado);
    for(int i=0;i<tam-1 && resul==0;i++)
        if(estado[i]==1 && estado[i+1]==-1) resul=1;
    return resul;
}
int FormigaDeSaida(int *estado){
    int retorno=0;
    int tam=Length(estado);
    if(estado[0]==-1 || estado[tam-1]==1) retorno=1;
    return retorno;
}
int *TiraFormiga(int *estado){
    int ini=0,end=Length(estado),novoTam=end;
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
int *TiraColisaoFormiga(int *estado){
    int tam=Length(estado);
    int *prox=(int*)malloc(sizeof(int*)*tam);
    for(int i=0;i<tam;i++)
        prox[i]=estado[i];
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
    if(FormigaBatendo(estAtual)){
        if(FormigaDeSaida(estAtual)){
            prox=TiraFormiga(estAtual);
            estAtual=prox;
        }
        prox=TiraColisaoFormiga(estAtual);
    }else if(FormigaDeSaida(estAtual)){
        prox=TiraFormiga(estAtual);
    }
    return prox;
}

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
    int t=Length(est1);
    int retorno=1;
    for(int i=0;i<t && retorno==1;i++){
        if(est1[i]!=est2[i]) retorno=0;
    }
    return retorno;
}
int equivalenteTamanho(int* est1, int* est2){
    int resul = 0;
    int t1=Length(est1);
    int t2=Length(est2);
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
void appendPilha(int v, Pilha **p){
    Pilha *cel=(Pilha*)malloc(sizeof(Pilha));
    cel->n=v;
    cel->prox=*p;
    *p=cel;
}
void buscarCaminho(Grafo *gr, int inicio, Pilha **pilha, int *cont ){
    appendPilha(inicio,pilha);
    for(int i=0; i<gr->grau[inicio]; i++){
        *cont+=1;
        buscarCaminho(gr, gr->arestas[inicio][i], pilha, cont);
    }
}

void mostrarEstados(int **estados,int indiceOUTodos){
    if( indiceOUTodos == 31){
        for(int x=0; x<31; x++){
            if(estados[x][3] != 2)
                printf("%d | %d %d %d %d\n",x, estados[x][0], estados[x][1], estados[x][2], estados[x][3] );
            else if(estados[x][2] != 2)
                printf("%d | %d %d %d\n",x, estados[x][0], estados[x][1], estados[x][2]);
            else if(estados[x][1] != 2)
                printf("%d | %d %d\n",x, estados[x][0], estados[x][1]);
            else if(estados[x][0] != 2)
                printf("%d | %d\n",x, estados[x][0]);
        }
    }else{
        if(estados[indiceOUTodos][3] != 2)
            printf("%d | %d %d %d %d\n",indiceOUTodos, estados[indiceOUTodos][0], estados[indiceOUTodos][1], estados[indiceOUTodos][2], estados[indiceOUTodos][3] );
        else if(estados[indiceOUTodos][2] != 2)
            printf("%d | %d %d %d\n",indiceOUTodos, estados[indiceOUTodos][0], estados[indiceOUTodos][1], estados[indiceOUTodos][2]);
        else if(estados[indiceOUTodos][1] != 2)
            printf("%d | %d %d\n",indiceOUTodos, estados[indiceOUTodos][0], estados[indiceOUTodos][1]);
        else if(estados[indiceOUTodos][0] != 2)
            printf("%d | %d\n",indiceOUTodos, estados[indiceOUTodos][0]);
    }
}
void mostrarPilha(Pilha *P,int **est){
    Pilha *aux = P;
    if(aux!=NULL){
        mostrarPilha(P->prox,est);
        mostrarEstados(est, aux->n);
    }
}

int main(){
    
    Pilha *pilha=NULL;
    int **estados, vet[4], qtd;

    Grafo *grafo = cria_Grafo(31);
    estados = gerarEstados(4);
    
    //mostrarEstados(estados, 31);
    construirGrafo(grafo,estados, 30);

    printf("Insira posição inicial (ex: [1 -1 1 -1] => ");
    scanf("%d %d %d %d", &vet[0],&vet[1],&vet[2],&vet[3]);
    int inicio = equivalenteIndice(estados,vet);

    buscarCaminho(grafo, inicio, &pilha, &qtd);
    mostrarPilha(pilha, estados);
    printf("Qtd passos: %d\n",qtd );

    return 0;
}