#include<stdio.h>
#include<stdlib.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
};

typedef struct grafos Grafos;

typedef struct{
    int **mat;
    Grafos *gr;
}Estados;

struct pilha{
    int n;
    struct pilha *prox;
};
typedef struct pilha Pilha;

Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
Estados *inicializaEst();
int tamaEst(int *estado);
int temColisao(int *estado);
int temSaida(int *estado);
int** troca(int **mat);
int *tira(int *estado);
void printEstado(int *estado);
int *tiraC(int *estado);
int *proximoEstado(int *estAtual);
int igual(int *est,int *est2);
int procuraEst(int **estados,int *estado);
void adj(Estados *est);
void push(int v, Pilha **p);
int MontaCam(Grafos *gr,int atual,Pilha **caminho,int *cont);
void exibePilha(Pilha *P,Estados *est);
void LigarEstados(Estados *est,int numeroEstados);

int main(){
    Estados *est=inicializaEst();

    Pilha *p=NULL;
    int *test=(int*)calloc(4,sizeof(int));
    printf("digite a configuração das formigas separadas por espaço: ");
    for(int i=0;i<4;i++){
        scanf("%i",&test[i]);
    }
    // test=proximoEstado(test);
    // printEstado(test);
    // printf("%d\n",procuraEst(est->mat,test));
    
    
    LigarEstados(est,30);
    //adj(est);
    
    int cont=0;
    MontaCam(est->gr,procuraEst(est->mat,test),&p,&cont);
    printf("As formigas vão passar por %d rodada(s) até saírem\nSendo elas:\n",cont);
    exibePilha(p,est);

}

int** troca(int **mat){
    int i;
    for(i=0;i<16;i++){
        for(int j=0;j<4;j++){
            if(mat[i][j]==0) mat[i][j]=-1;
        }
    }
    for(i=i;i<24;i++){
        for(int j=0;j<3;j++){
            if(mat[i][j]==0) mat[i][j]=-1;
        }    
    }
    for(i=i;i<28;i++){
        for(int j=0;j<2;j++){
            if(mat[i][j]==0) mat[i][j]=-1;
        }
    }
    return mat;
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

Estados *inicializaEst(){
    Estados *est=(Estados*)malloc(sizeof(Estados));
    est->gr=criaGrafo(31,0);
    est->mat=(int**)malloc(sizeof(int*)*31);
    int cont=0;
    int aa=1,bb=1,cc=1,dd=1;
    for (int i = 0; i < 16; ++i) {
        est->mat[cont]=(int*)calloc(4,sizeof(int));
        if (!(i % 8)) aa = !aa;
        if (!(i % 4)) bb = !bb;
        if (!(i % 2)) cc = !cc; 
        dd = !dd;
        est->mat[cont][0]=aa;
        est->mat[cont][1]=bb;
        est->mat[cont][2]=cc;
        est->mat[cont][3]=dd;
        cont++;
    }
    bb=1,cc=1,dd=1;
    for (int i = 0; i < 8; ++i) {
        est->mat[cont]=(int*)calloc(3,sizeof(int));
        if (!(i % 4)) bb = !bb;
        if (!(i % 2)) cc = !cc; 
        dd = !dd;
        est->mat[cont][0]=bb;
        est->mat[cont][1]=cc;
        est->mat[cont][2]=dd;
        cont++;
    }
    cc=1,dd=1;
    for (int i = 0; i < 4; ++i) {
        est->mat[cont]=(int*)calloc(2,sizeof(int));
        if (!(i % 2)) cc = !cc; 
        dd = !dd;
        est->mat[cont][0]=cc;
        est->mat[cont][1]=dd;
        cont++;
    }
    est->mat[cont]=(int*)calloc(1,sizeof(int));
    est->mat[cont][0]=-1;
    cont++;
    est->mat[cont]=(int*)calloc(1,sizeof(int));
    est->mat[cont][0]=1;
    cont++;
    est->mat[cont]=(int*)calloc(1,sizeof(int));
    est->mat[cont][0]=0;
    est->mat=troca(est->mat);
    return est;
}

int tamaEst(int *estado){
    int tam=0;
    for(tam=0;estado[tam]!=0;tam++);
    return tam;
}

int temColisao(int *estado){
    int retorno=0;
    int tam=tamaEst(estado);
    for(int i=0;i<tam-1 && retorno==0;i++)
        if(estado[i]==1 && estado[i+1]==-1) retorno=1;
    return retorno;
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

void printEstado(int *estado){
    int tam = tamaEst(estado);
        printf("(");
        for(int i = 0; i < tam-1; i++) printf("%d,", estado[i]);
        printf("%d)\n", estado[tam-1]);  
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

int *proximoEstado(int *estAtual){
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

int igual(int *est,int *est2){
    int t=tamaEst(est2);
    int retorno=1;
    for(int i=0;i<t && retorno==1;i++){
        if(est[i]!=est2[i]) retorno=0;
    }
    return retorno;
}

int procuraEst(int **estados,int *estado){
    int retorno = -1;
    for(int i=0;i<31 && retorno==-1;i++){
        int t=tamaEst(estados[i]);
        if(t==tamaEst(estado)){
            if(igual(estados[i],estado)){
                retorno=i;
            }
        }
    }
}

void LigarEstados(Estados *est,int numeroEstados){
    for(int i=0;i<numeroEstados;i++){
        int *prox=proximoEstado(est->mat[i]);
        int dest=procuraEst(est->mat,prox);
        //printf("DES: %d\n",dest );
        insereAresta(&est->gr,i,dest,0,1);
    }
}

void adj(Estados *est){
    for(int i=0;i<30;i++){
        printf("Estado %d ",i);printEstado(est->mat[i]);
        for(int j=0;j<est->gr->grau[i];j++){
            printf("Vai para o Estado %d ",est->gr->arestas[i][j]);printEstado(est->mat[est->gr->arestas[i][j]]);
        }
    }
}

void push(int v, Pilha **p){
    Pilha *cel=(Pilha*)malloc(sizeof(Pilha));
    cel->n=v;
    cel->prox=*p;
    *p=cel;
}

int MontaCam(Grafos *gr,int atual,Pilha **caminho,int *cont){
    int i;
    push(atual,caminho);
    for(i=0; i<gr->grau[atual]; i++){
        *cont+=1;
        MontaCam(gr, gr->arestas[atual][i], caminho, cont);
    }
}

void exibePilha(Pilha *P,Estados *est){
    Pilha *aux = P;
    if(aux!=NULL){
        exibePilha(P->prox,est);
        printf("%d:",aux->n);printEstado(est->mat[aux->n]);
    }
}