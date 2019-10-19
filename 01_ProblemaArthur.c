#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int eh_ponderado;
	int nro_vertices;
	int grau_max;
	int** arestas;
	float** pesos;
	int* grau;
}Grafo;

Grafo* cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado){
	Grafo *gr;
	gr = (Grafo*) malloc(sizeof(Grafo));

	if( gr != NULL){
		int i;
		gr->nro_vertices = nro_vertices;
		gr->grau_max = grau_max;
		gr->eh_ponderado = (eh_ponderado != 0) ? 1:0;
		gr->grau=(int*)calloc(nro_vertices, sizeof(int));
		
		gr->arestas = (int**)malloc(nro_vertices*sizeof(int*));
		for(i=0; i<nro_vertices; i++){
			gr->arestas[i] = (int*)malloc(grau_max*sizeof(int*));
			
			if(gr->eh_ponderado == 1){
				gr->pesos = (float**)malloc(nro_vertices*sizeof(float*));
				for(int j=0; j<nro_vertices; j++){
					gr->pesos[j] = (float*)malloc(grau_max*sizeof(float));
				}
			}
		}
	}
	return gr;
}

int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso){
	if(gr == NULL)
		return 0;
	if(orig < 0 || orig >= gr->nro_vertices)
		return 0;
	if(dest < 0 || dest >= gr->nro_vertices)
		return 0;

	gr->arestas[orig][gr->grau[orig]] = dest;
	if(gr->eh_ponderado)
		gr->pesos[orig][gr->grau[orig]] = peso;
	gr->grau[orig]++;

	if(eh_digrafo == 0)
		insereAresta(gr,dest,orig,1,peso);
	return 1;
}

int removeAresta(Grafo *gr, int orig, int dest, int eh_digrafo){
	if(gr == NULL)
		return 0;
	if(orig < 0 || orig >= gr->nro_vertices)
		return 0;
	if(dest < 0 || dest >= gr->nro_vertices)
		return 0;

	int i = 0;
	while(i<gr->grau[orig] && gr->arestas[orig][i] != dest)
		i++;

	if(i == gr->grau[orig])
		return 0;

	gr->grau[orig]--;
	gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];
	if(gr->eh_ponderado)
		gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];
	if(eh_digrafo == 0)
		removeAresta(gr,dest,orig,1);
	return 1;
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


int main(){
 	
 	int op, eh_ponderado, grau_max, nro_vertices;
 	int orig, dest, eh_digrafo, peso;
	Grafo *gr = NULL;

	printf("-----MENU-----\n");
	printf("[1]-Criar/Reiniciar Grafo\n");
	printf("[2]-Inserir Aresta\n");
	printf("[3]-Mostrar\n");
	printf("[4]-Remover\n");
	printf("[5]-Liberar\n");
	printf("[0]-SAIR\n");
	scanf("%d", &op);
	switch(op){
		case 1:
			printf("Numero de vertices: ");
			scanf("%d", &nro_vertices);

			printf("Numero grau máximo: ");
			scanf("%d", &grau_max);

			printf("Grafo Ponderado? (1-Sim |0-Não): ");
			scanf("%d", &eh_ponderado);

			if(gr!= NULL)
				liberar_Grafo(gr);
			gr = cria_Grafo(nro_vertices, grau_max, eh_ponderado);
			printf("Grafo CRIADO com Sucesso.\n");
			break;

		case 2:
			printf("Vertice de Origem: ");
			scanf("%d", &orig);

			printf("Vertice de Destino: ");
			scanf("%d", &dest);

			printf("É Digrafo? (1-Sim |0-Não): ");
			scanf("%d", &eh_digrafo);

			printf("Peso da relação: ");
			scanf("%d", &peso);

			insereAresta(gr, orig, dest, eh_digrafo, peso);
			printf("Aresta INSERIDA com Sucesso.\n");

			break;
		case 3:

			break;
		case 4:
			printf("Vertice de Origem: ");
			scanf("%d", &orig);

			printf("Vertice de Destino: ");
			scanf("%d", &dest);

			printf("É Digrafo? (1-Sim |0-Não): ");
			scanf("%d", &eh_digrafo);

			removeAresta(gr, orig, dest, eh_digrafo);
			printf("Aresta REMOVIDA com Sucesso.\n");

			break;
		case 5:
			if(gr!= NULL)
				liberar_Grafo(gr);
			printf("Grafo LIBERADO com Sucesso.\n");

			break;
		case 0:
			exit(0);
			break;
	}



	return 0;
}