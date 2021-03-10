#include <iostream>
#include <time.h>
#include <time.h>
#include "pmm.hpp"
#include <cstring>

#define MAX(X, Y) ((X > Y) ? X : Y)
using namespace std;

int PESO = 100;
int main(int argc, char *argv[])
{

    srand(time(NULL));
    SolucaoBIN sol;
    lerDados(argv[1]);
    testarDados("");
    construtivaAleatoriaBIN(sol);
    calcularFOBIN(sol);
    escreverSolucaoBIN(sol, 1);

    return 0;
}

void lerDados(string arq)
{
    FILE *f = fopen(arq.c_str(), "r");
    fscanf(f, "%d %d", &numObj, &numMoc);
    for (int j = 0; j < numObj; j++)
        fscanf(f, "%d", &vetValObj[j]);
    for (int j = 0; j < numObj; j++)
        fscanf(f, "%d", &vetPesObj[j]);
    for (int j = 0; j < numMoc; j++)
        fscanf(f, "%d", &vetCapMoc[j]);
    fclose(f);
}

void testarDados(const char *arq)
{
    FILE *f;
    const char *vazio = "";
    if (arq == vazio)
        f = stdout;
    else
        f = fopen(arq, "w");
    fprintf(f, "%d %d\n", numObj, numMoc);
    for (int j = 0; j < numObj; j++)
        fprintf(f, "%d ", vetValObj[j]);
    fprintf(f, "\n");
    for (int j = 0; j < numObj; j++)
        fprintf(f, "%d ", vetPesObj[j]);
    fprintf(f, "\n");
    for (int j = 0; j < numMoc; j++)
        fprintf(f, "%d ", vetCapMoc[j]);
    fprintf(f, "\n");
    if (arq != vazio)
        fclose(f);
}

/*INICIO MOCHILA MULTIPLA*/
void calcularFO(Solucao &s)
{
    memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
    s.funObj = 0;
    for (int i = 0; i < numObj; i++)
    {
        if (s.vetIdMocObj[i] != -1)
        {
            s.funObj += vetValObj[i];
            s.vetPesMoc[s.vetIdMocObj[i]] += vetPesObj[i];
        }
    }
    for (int j = 0; j < numMoc; j++)
        s.funObj -= PESO * MAX(0, s.vetPesMoc[j] - vetCapMoc[j]);
}

void construtivaAleatoria(Solucao &s)
{
    for (int i = 0; i < numObj; i++)
        s.vetIdMocObj[i] = (rand() % (numMoc + 1)) - 1;
}

void escreverSolucao(Solucao &s, const bool flag)
{
    printf("\nFO: %d\n", s.funObj);
    if (flag)
    {
        printf("VETOR ID MOCHILAS DE CADA OBJETO: ");
        for (int j = 0; j < numObj; j++)
            printf("%d  ", s.vetIdMocObj[j]);
        printf("\nVETOR PESO MOCHILAS: ");
        for (int i = 0; i < numMoc; i++)
            printf("%d  ", s.vetPesMoc[i]);
    }
}
/*FIM MOCHILA MULTIPLA*/

/*INICIO MOCHILA BINARIA*/

void calcularFOBIN(SolucaoBIN &s)
{
    int aux = numMoc * numObj, aux1 = 0, aux2 = 0;

    memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
    s.funObj = 0;
    for (int i = 0; i < aux; i++)
    {

        //cálculo para mapear o índice dos objetos escolhidos no vetor de mochilas
        aux1 = i % numObj;
        s.funObj += s.vetObjetos[i] * vetValObj[aux1];
        //cálculo para mapear o índices das mochilas no vetor de peso total de cada mochila
        aux2 = i / numObj;
        s.vetPesMoc[aux2] += vetPesObj[aux1];
    }

    for (int j = 0; j < numMoc; j++)
        s.funObj -= PESO * MAX(0, s.vetPesMoc[j] - vetCapMoc[j]);
}

void escreverSolucaoBIN(SolucaoBIN &s, const bool flag)
{
    printf("\nFO: %d\n", s.funObj);
    if (flag)
    {
        printf("VETOR ID MOCHILAS DE CADA OBJETO: ");
        for (int j = 0; j < numObj * numMoc; j++)
            printf("%d  ", s.vetObjetos[j]);
        printf("\nVETOR PESO MOCHILAS: ");
        for (int i = 0; i < numMoc; i++)
            printf("%d  ", s.vetPesMoc[i]);
    }
}

void construtivaAleatoriaBIN(SolucaoBIN &s)
{
    for (int i = 0; i < numObj * numMoc; i++)
        s.vetObjetos[i] = rand() % 2;
}


/*FIM MOCHILA BINARIA*/