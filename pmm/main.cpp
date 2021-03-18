#include <iostream>
#include <time.h>
#include "pmm.hpp"
#include <cstring>
#include <memory.h>

#define MAX(X, Y) ((X > Y) ? X : Y)
using namespace std;

int PESO = 100;
int main(int argc, char *argv[])
{
    srand(time(NULL));
    testar_heuConstrutivas(argv[1]);
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

void ordenarObjetos()
{
    int aux = 0;
    for (int j = 0; j < numObj; j++)
        vetIndObjOrd[j] = j;

    for (int j = 1; j < numObj; j++)
    {
        for (int i = 0; i < numObj - 1; i++)
        {
            if ((double)vetValObj[vetIndObjOrd[i]] / vetPesObj[vetIndObjOrd[i]] <
                (double)vetValObj[vetIndObjOrd[i + 1]] / vetPesObj[vetIndObjOrd[i + 1]])
            {
                aux = vetIndObjOrd[i];
                vetIndObjOrd[i] = vetIndObjOrd[i + 1];
                vetIndObjOrd[i + 1] = aux;
            }
        }
    }
}

void construtivaGulosa(Solucao &s)
{
    memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
    memset(&s.vetIdMocObj, -1, sizeof(s.vetIdMocObj));
    for (int j = 0; j < numObj; j++)
        for (int i = 0; i < numMoc; i++)
            if (s.vetPesMoc[i] + vetPesObj[vetIndObjOrd[j]] <= vetCapMoc[i])
            {
                s.vetIdMocObj[vetIndObjOrd[j]] = i;
                s.vetPesMoc[i] += vetPesObj[vetIndObjOrd[j]];
                break;
            }
}

void heuConAleGul(Solucao &s, const int percentual)
{
    int tam, pos, aux;
    int vetAux[MAX_OBJ];
    memcpy(&vetAux, &vetIndObjOrd, sizeof(vetIndObjOrd));
    tam = MAX(1, (percentual / 100.0) * numObj);
    for (int j = 0; j < tam; j++)
    {
        pos = j + rand() % (numObj - j);
        aux = vetAux[pos];
        vetAux[pos] = vetAux[j];
        vetAux[j] = aux;
    }
    memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
    memset(&s.vetIdMocObj, -1, sizeof(s.vetIdMocObj));
    for (int j = 0; j < numObj; j++)
        for (int i = 0; i < numMoc; i++)
            if (s.vetPesMoc[i] + vetPesObj[vetAux[j]] <= vetCapMoc[i])
            {
                s.vetIdMocObj[vetAux[j]] = i;
                s.vetPesMoc[i] += vetPesObj[vetAux[j]];
                break;
            }
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

void testar_heuConstrutivas(string argv)
{
    Solucao sol;
    clock_t h;
    double tempo;
    const int repeticoes = 1000;

    lerDados(argv);
    ordenarObjetos();
    printf("\n\n>>> TESTE - HEURISTICAS CONSTRUTIVAS - %d - %d repetições\n", argv[1], repeticoes);

     h = clock();
    for (int r = 0; r < repeticoes; r++)
        heuConAleGul(sol, 80);
    calcularFO(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("Construtiva Aleatória Gulosa...: %.10f seg.\n", tempo);

    h = clock();
    for (int r = 0; r < repeticoes; r++)
        construtivaGulosa(sol);
    calcularFO(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("Construtiva Gulosa...: %.10f seg.\n", tempo);

    h = clock();
    for (int r = 0; r < repeticoes; r++)
        construtivaAleatoria(sol);
    calcularFO(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("Construtiva Aleatória...: %.10f seg.\n", tempo);
}
