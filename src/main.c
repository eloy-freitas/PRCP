#include "stdio.h"
#include "stdlib.h"
#include "header/matriz.h"

int main(){

    int L, C;
    scanf("%d", &L);
    scanf("%d", &C);

    int **matriz = criaMatriz(L, C);

    povoarMatriz(matriz, L, C);
    imprimirMatriz(matriz, L, C);
}