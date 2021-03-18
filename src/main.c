#include "stdio.h"
#include "stdlib.h"
#include "header/matriz.h"
#include "header/construtiva.h"
#include "header/solucao.h"
int main()
{

    int C, L;
    scanf("%d", &L);
    scanf("%d", &C);

    //Solucao *lista = criarLista();
    int **matriz = lerArquivo(L, C);
    /*Solucao *s1 = criarSolucao(1, 50, 10, 30, L, C);
    Solucao *s2 = criarSolucao(2, 50, 10, 30, L, C);
    Solucao *s3 = criarSolucao(3, 50, 10, 30, L, C);
    inserirSolucao(lista, s1);
    inserirSolucao(lista, s2);
    inserirSolucao(lista, s3);
    V
    imprimirSolucoes(lista);*/
    //limparSolucao(s1);
    //imprimirSolucoes(s1);
    imprimirMatrizDoArquivo(matriz, L*C);
    //imprimirMatriz(matrizPDC, aux, aux);
}