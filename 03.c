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



int main(){
	Grafo *gr;
	
	int insereAresta(Grafo *gr, int orig, int dest,int eh_digrafo, float peso);
	insereAresta(gr,0,1,0,0);
	insereAresta(gr,1,3,0,0);

	int removeAresta(Grafo *gr, int orig, int dest, int eh_digrafo);
	removeAresta(gr,0,1,0);



	return 0;
}