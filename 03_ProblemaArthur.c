#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
	int resul = 0;
	if(gr != NULL){
		if(( orig >= 0 && orig < gr->nro_vertices) || (dest >= 0 && dest < gr->nro_vertices)){
			
			gr->arestas[orig][gr->grau[orig]] = dest;
			if(gr->eh_ponderado)
				gr->pesos[orig][gr->grau[orig]] = peso;
			gr->grau[orig]++;

			if(eh_digrafo == 0)
				insereAresta(gr,dest,orig,1,peso);
			resul = 1;
		}
	}
	return resul;
}

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

//Função auxiliar que faz cálculo da busca
void buscaProfundidade(Grafo *gr, int ini, int *visitado, int cont, int *visitados_pesos){

	int i;
	visitado[ini] = cont;
	for(i=0; i<gr->grau[ini]; i++){
		if(!visitado[gr->arestas[ini][i]]){
			visitados_pesos[gr->arestas[ini][i]] += gr->pesos[ini][i]+visitados_pesos[ini];
			buscaProfundidade(gr, gr->arestas[ini][i], visitado, cont+1, visitados_pesos);
		}
	}
}

//Função principal: Faz a interface com o usuário
void buscaProfundidade_Grafo(Grafo *gr, int ini, int *visitado, int *visitados_pesos){

	int i, cont = 1;
	for(i=0; i<gr->nro_vertices; i++)
		visitado[i] = 0;
		visitados_pesos[i] = 0;

	buscaProfundidade(gr, ini, visitado, cont, visitados_pesos);
}

void buscaLargura_Grafo(Grafo *gr, int ini, int *visitado, int *visitados_pesos){
	int i, vert, NV, cont=1, *fila, IF=0, FF=0;

	for(i=0; i<gr->nro_vertices; i++){
		visitado[i] = 0;
		visitados_pesos[i] = 0;
	}
	NV = gr->nro_vertices;
	fila = (int*) malloc(NV * sizeof(int));
	FF++;
	fila[FF] = ini;
	visitado[ini] = cont;
	while(IF != FF){
		IF = (IF+1)%NV;
		vert = fila[IF];
		cont++;
		for(i=0; i< gr->grau[vert]; i++){
			if(!visitado[gr->arestas[ vert][i] ]){
				FF = (FF+1)%NV;
				fila[FF] = gr->arestas[vert][i];
				visitado[gr->arestas[vert][i]] = cont;
				visitados_pesos[gr->arestas[vert][i]] += gr->pesos[vert][i]+visitados_pesos[vert];
			}
		}
	}
	free(fila);
}

int main(){
 	
 	int op, eh_ponderado, grau_max, nro_vertices;
 	int orig, dest, eh_digrafo, peso, Inicial;
 	int *visitados, *visitados_pesos;
	Grafo *gr = NULL;

	while(1){
		printf("-----MENU-----\n");
		printf("[1]-Criar/Reiniciar Grafo\n");
		printf("[2]-Inserir Aresta\n");
		printf("[3]-Mostrar\n");
		printf("[4]-Remover\n");
		printf("[5]-Busca Profundidade\n");
		printf("[6]-Busca Largura\n");
		printf("[7]-Liberar\n");
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
				/*
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
				*/

				/*
				TESTE SLIDE
				insereAresta(gr, 0, 1, 1, 1);
				insereAresta(gr, 1, 3, 1, 1);
				insereAresta(gr, 1, 2, 1, 1);
				insereAresta(gr, 2, 4, 1, 1);
				insereAresta(gr, 3, 0, 1, 1);
				insereAresta(gr, 3, 4, 1, 1);
				insereAresta(gr, 4, 1, 1, 1);
				*/
				insereAresta(gr, 0, 2, 1, 1);
				insereAresta(gr, 0, 4, 1, 1);
				insereAresta(gr, 1, 3, 1, 1);
				insereAresta(gr, 2, 7, 1, 1);
				insereAresta(gr, 3, 6, 1, 1);
				insereAresta(gr, 4, 5, 1, 1);
				insereAresta(gr, 4, 7, 1, 1);
				insereAresta(gr, 5, 1, 1, 1);
				insereAresta(gr, 5, 4, 1, 1);
				insereAresta(gr, 5, 7, 1, 1);
				insereAresta(gr, 6, 0, 1, 1);
				insereAresta(gr, 6, 2, 1, 1);
				insereAresta(gr, 6, 4, 1, 1);
				insereAresta(gr, 7, 3, 1, 1);
				insereAresta(gr, 7, 5, 1, 1);
		
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
				printf("Vertice Inicial: ");
				
				scanf("%d", &Inicial);
				visitados = (int*) calloc(nro_vertices, sizeof(int));
				visitados_pesos = (int*) calloc(nro_vertices, sizeof(int));

				buscaProfundidade_Grafo(gr, Inicial, visitados, visitados_pesos);

				printf("VISITADOS: ");
				for(int i=0; i<nro_vertices; i++){
					printf("[%d - %d {%d}] ",i, visitados[i], visitados_pesos[i] );
				}
				printf("\n");

				free(visitados);
				visitados = NULL;

				free(visitados_pesos);
				visitados_pesos = NULL;
				printf("Busca em PROFUNDIDADE realizada com Sucesso.\n");
				break;

			case 6:
				printf("Vertice Inicial: ");
				
				scanf("%d", &Inicial);
				visitados = (int*) calloc(nro_vertices, sizeof(int));
				visitados_pesos = (int*) calloc(nro_vertices, sizeof(int));

				buscaLargura_Grafo(gr, Inicial, visitados, visitados_pesos);

				printf("VISITADOS: ");
				for(int i=0; i<nro_vertices; i++){
					printf("[%d - %d {%d}] ",i, visitados[i], visitados_pesos[i] );
				}
				printf("\n");

				free(visitados);
				visitados = NULL;

				free(visitados_pesos);
				visitados_pesos = NULL;
				printf("Busca em LARGURA realizada com Sucesso.\n");
				break;
			case 7:
				if(gr!= NULL)
					liberar_Grafo(gr);
				printf("Grafo LIBERADO com Sucesso.\n");

				break;
			case 0:
				exit(0);
				break;
		}
	}
	return 0;
}