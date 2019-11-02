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
}Grafo;

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
			
			if(gr->eh_ponderado == 1){
				gr->pesos = (float**)malloc(nro_vertices*sizeof(float*));
				for(int j=0; j<nro_vertices; j++){
					gr->pesos[j] = (float*)malloc(nro_vertices*sizeof(float));
				}
			}
		}
	}
	return gr;
}

int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso){
	int resul = 0;
	if(gr != NULL){
		if(( orig > 0 && orig <= gr->nro_vertices) || (dest > 0 && dest <= gr->nro_vertices)){		
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

/*
int removeAresta(Grafo *gr, int orig, int dest, int eh_digrafo){
	int resul = 0;

	if(gr != NULL){
		if(( orig >= 0 && orig < gr->nro_vertices) || (dest >= 0 && dest < gr->nro_vertices)){

			int i = 0;
			while(i<gr->grau[orig] && gr->arestas[orig][i] != dest)
				i++;

			if(i != gr->grau[orig]){
				gr->grau[orig]--;
				gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
				if(gr->eh_ponderado)
					gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];
				if(eh_digrafo == 0)
					removeAresta(gr,dest,orig,1);

				resul = 1;
			}
		}
	}
	return resul;
}

void liberar_Grafo(Grafo *gr){
	if( gr != NULL){
		int i;
		for(i=0; i< gr->nro_vertices; i++)
			free(gr->arestas[i]);
		free(gr->arestas);

		if(gr->eh_ponderado == 1){
			for(i=0; i< gr->nro_vertices; i++)
				free(gr->pesos[i]);
			free(gr->pesos);
		}
		free(gr->grau);
		free(gr);
	}
}
*/

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
	for (int x=0; x < discos; x++){
		if(estadoTorre2[x] != estadoTorre1[x])
			pos = x;
	}
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


int main(){
 	
	int possibilidades = pow(pinos,discos);
	int **estadoTorre = gerarPossibilidades(possibilidades);

 	Grafo *grafo = cria_Grafo(possibilidades,pinos, 0);
	
	construirGrafo(grafo, estadoTorre, possibilidades);

	construirArestaNivel(grafo, possibilidades);

	/*	
	for(int x=0;x< possibilidades ;x++){
		printf("%d %d %d %d\n",estadoTorre[x][0],estadoTorre[x][1],estadoTorre[x][2],estadoTorre[x][3]);
	}*/

	for(int x=0; x< possibilidades; x++){
		printf("%d | ", x+1);
		for(int y=0; y < pinos; y++){
			printf("%d ",grafo->arestas[x][y] );
		}
		printf("(%d %d %d %d)\n",estadoTorre[x][0],estadoTorre[x][1],estadoTorre[x][2],estadoTorre[x][3]);
	}

	return 0;
}