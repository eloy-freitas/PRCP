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
    //testarDados("");
    SolucaoBIN s;
    //construtivaAleatoriaBIN(s);
    ordenarObjetos();
    construtivaGulosaBIN(s);
    calcularFOBIN(s);
    escreverSolucaoBIN(s, 0);
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
    printf("CONFLITOS: %d\n", s.conflitos);
    printf("PONTOS LIVRES: %d\n", s.pontosLivres);
    if (flag)
    {
        printf("VETOR POSIÇÕES ESCOLHIDAS: ");
        for (int j = 0; j < numObj; j++)
            printf("%d  ", s.vetPosicoesEscolhidas[j]);
    }
    printf("\n");
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
    s.pontosLivres = MAX(0, s.funObj - s.conflitos);
    s.funObj -= PESO * MAX(0, s.conflitos);
}

void ordenarObjetos()
{
    double aux = 0;
    for (int j = 0; j < (numObj * numMoc); j++)
    {
        vetIndObjOrd[j] = j;
        vetPesObjOrd[j] = (double)matConflitoPontos[j][0] / (numObj * numMoc);
    }

    for (int i = 0; i < (numObj * numMoc); i++)
    {
        for (int j = 0; j < (numObj * numMoc) - 1; j++)
        {

            if (vetPesObjOrd[j] > vetPesObjOrd[j + 1])
            {

                auxiliar = vetIndObjOrd[j];
                vetIndObjOrd[j] = vetIndObjOrd[j + 1];
                vetIndObjOrd[j + 1] = auxiliar;

                aux = vetPesObjOrd[j];
                vetPesObjOrd[j] = vetPesObjOrd[j + 1];
                vetPesObjOrd[j + 1] = aux;
            }
        }
    }
}

void construtivaGulosa(Solucao &s)
{
    memset(&s.vetPosicoesEscolhidas, -1, sizeof(s.vetPosicoesEscolhidas));

    for (int i = 0; i < (numObj * numMoc); i++)
    {
        if (s.vetPosicoesEscolhidas[(vetIndObjOrd[i]) / numObj] == -1)
        {
            s.vetPosicoesEscolhidas[(vetIndObjOrd[i] - 1) / numObj] = (vetIndObjOrd[i] - 1) % numObj;
            for (int j = 1; j < matConflitoPontos[vetIndObjOrd[i]][0] + 1; j++)
            {
                s.vetPosicoesEscolhidas[(matConflitoPontos[auxiliar][j] - 1) / numObj] = -2;
            }
        }
    }
}

void construtivaGulosaBIN(SolucaoBIN &s)
{
    memset(&s.vetPosicoesEscolhidas, 0, sizeof(s.vetPosicoesEscolhidas));
    int flag = 0;
    int cont = 0;
    for (int i = 0; i < (numObj * numMoc); i++)
    {
        if (s.vetPosicoesEscolhidas[vetIndObjOrd[i]] == -1 || s.vetPosicoesEscolhidas[vetIndObjOrd[i]] == 1)
            continue;
        if (s.vetPosicoesEscolhidas[vetIndObjOrd[i]] == 0)
        {

            for (int j = 1; j < matConflitoPontos[vetIndObjOrd[i]][0] + 1; j++)
                if (s.vetPosicoesEscolhidas[matConflitoPontos[vetIndObjOrd[i]][j] - 1] != 1)
                {
                    cont++;
                    flag = 1;
                }
                else
                {
                    flag = 0;
                    cont = 0;
                    break;
                }

            if (flag == 1 && cont == matConflitoPontos[vetIndObjOrd[i]][0])
            {
                for (int j = 1; j < matConflitoPontos[vetIndObjOrd[i]][0] + 1; j++)
                    if (s.vetPosicoesEscolhidas[matConflitoPontos[vetIndObjOrd[i]][j] - 1] == 0)
                    {
                        s.vetPosicoesEscolhidas[matConflitoPontos[vetIndObjOrd[i]][j] - 1] = -1;
                    }
                s.vetPosicoesEscolhidas[vetIndObjOrd[i]] = 1;
            }
        }
        cont = 0;
        flag = 0;
    }
}

void construtivaAleatoriaBIN(SolucaoBIN &s)
{
    for (int j = 0; j < numObj * numMoc; j++)
        s.vetPosicoesEscolhidas[j] = rand() % 2;
}

void escreverSolucaoBIN(SolucaoBIN &s, const bool flag)
{
    printf("\nFO: %d\n", s.funObj);
    printf("CONFLITOS: %d\n", s.conflitos);
    printf("PONTOS LIVRES: %d\n", s.pontosLivres);
    if (flag)
    {
        printf("VETOR POSIÇÕES ESCOLHIDAS: ");
        for (int j = 0; j < numObj * numMoc; j++)
            printf("%d  ", s.vetPosicoesEscolhidas[j]);
    }
    printf("\n");
}

void calcularFOBIN(SolucaoBIN &s)
{
    s.conflitos = 0;
    s.funObj = 0;
    s.pontosLivres = 0;
    for (int i = 0; i < numObj * numMoc; i++)
        if (s.vetPosicoesEscolhidas[i] == 1)
        {
            s.funObj++;
            
        }
   
    for (int i = 0; i < numObj * numMoc; i++)
        if (s.vetPosicoesEscolhidas[i] == 1)
        {
            for (int j = 1; j < matConflitoPontos[i][0] + 1; j++)
            {
                if (s.vetPosicoesEscolhidas[matConflitoPontos[i][j] - 1] == 1)
                    s.conflitos++;
            }
        }
    s.pontosLivres = MAX(0, s.funObj - s.conflitos);
    s.funObj -= PESO * MAX(0, s.conflitos);
}
