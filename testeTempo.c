#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}


int main(){
    float otofim;
    clock_t x,y;  
    x = clock();
    
    float tempoFinal;
    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);
    
    long testeI, testeF, teste_K;
    testeI = getMicrotime();
    
    int a = 1;
    for (int i = 0; i < 100000000; i++){
        a *= 10+5;
    }
    
    y = clock();
    otofim = ((y-x)*1000)/CLOCKS_PER_SEC;

    gettimeofday(&fim, NULL);
    tempoFinal = (fim.tv_sec - inicio.tv_sec) * (int)1e6 + (fim.tv_usec - inicio.tv_usec);

    testeF = getMicrotime();

    printf("JULI: %f\n", tempoFinal);
    printf("CLOCK: %f\n", otofim);
    printf("Function: %lo\n",testeF- testeI);
}
