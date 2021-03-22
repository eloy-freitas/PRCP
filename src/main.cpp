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
    lerDados(argv[1]);
    testarDados("");
    Solucao s;
    construtivaAleatoria(s);
    calcularFO(s);
    escreverSolucao(s, 1);
    return 0;
}

void lerDados(string arq)
{
    FILE *f = fopen(arq.c_str(), "r");
    memset(&matConflitoPontos, -1, sizeof(matConflitoPontos));

    fscanf(f, "%d %d", &numObj, &numMoc);
    for (int i = 0; matConflitoPontos[i][0] == EOF; i++)
    {
        fscanf(f, "%d", &matConflitoPontos[i][0]);
        for (int j = 1; j < matConflitoPontos[i][0] + 1; j++)
            fscanf(f, "%d", &matConflitoPontos[i][j]);
    }
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
    for (int i = 0; matConflitoPontos[i][0] != -1; i++)
    {
        for (int j = 1; j < matConflitoPontos[i][0] + 1; j++)
            fprintf(f, "%d ", matConflitoPontos[i][j]);
        fprintf(f, "\n");
    }

    if (arq != vazio)
        fclose(f);
}

void construtivaAleatoria(Solucao &s)
{
    memset(&s.vetPosicoesEscolhidas, -1, sizeof(s.vetPosicoesEscolhidas));
    for (int i = 0; i < numObj; i++)
        s.vetPosicoesEscolhidas[i] = (rand() % (numMoc + 1)) - 1;
}

void escreverSolucao(Solucao &s, const bool flag)
{
    printf("\nFO: %d\n", s.funObj);
    if (flag)
    {
        printf("VETOR ID MOCHILAS DE CADA OBJETO: ");
        for (int j = 0; j < numObj; j++)
            printf("%d  ", s.vetPosicoesEscolhidas[j]);
    }
}

void calcularFO(Solucao &s)
{
    s.funObj = 0;
    s.conflitos = 0;
    auxiliar = 0;
    for (int i = 0; i < numObj; i++)
    {
        if (s.vetPosicoesEscolhidas[i] != -1)
        {
            auxiliar = (i * numMoc) + s.vetPosicoesEscolhidas[i];
            for (int j = 1; j < matConflitoPontos[auxiliar][0] + 1; j++)
            {
                auxiliar2 = (matConflitoPontos[auxiliar][j] - 1) / numMoc;
                if (s.vetPosicoesEscolhidas[auxiliar2] != -1 && s.vetPosicoesEscolhidas[auxiliar2] == (matConflitoPontos[auxiliar][j] - 1) % numMoc)
                {
                    s.conflitos++;
                }
            }
        }
    }

    for (int i = 0; i < numObj; i++)
        if (s.vetPosicoesEscolhidas[i] != -1)
        {
            s.funObj++;
        }
    printf("conflitos =  %d, fo= %d \n ", s.conflitos, s.funObj);
    for (int j = 0; j < numMoc; j++)
        s.funObj -= PESO * MAX(0, s.conflitos);
}
