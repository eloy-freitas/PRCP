#include "stdio.h"
#include "stdlib.h"
#include "header/matriz.h"
#include "header/construtiva.h"

int main(){

    int C, L, aux;
    scanf("%d", &L);
    scanf("%d", &C);
   
    aux = L*C;

    int **matrizPDC = criaMatriz(aux, aux);
    int **matrizSolucao = criaMatriz(aux, C);

    padronizarMatriz(matrizSolucao, L, C, 0);
    padronizarMatriz(matrizPDC, aux, aux, -1);

    solucaoSementeAleatoria(matrizSolucao, L, C);
    lerArquivo(matrizPDC, aux, aux);
    //imprimirMatriz(matrizSolucao, L, C);
    //imprimirMatriz(matrizPDC, aux, aux);
}