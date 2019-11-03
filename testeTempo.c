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
    struct timeval inicio, fim;
    clock_t x,y;  
    float tempoFinal, otofim;
    gettimeofday(&inicio, NULL);
    x = clock();
    int a = 1;
    for (int i = 0; i < 100000000; i++){
        a *= 10+5;
    }
    y = clock();
    gettimeofday(&fim, NULL);

    otofim = ((y-x)*1000)/CLOCKS_PER_SEC;
    tempoFinal = (fim.tv_sec - inicio.tv_sec) * (int)1e6 + (fim.tv_usec - inicio.tv_usec);
    printf("PRIMEIRO: %f\n", tempoFinal);
    printf("OTO: %f\n", otofim);
}