#include <iostream>
#include <time.h>
#include <cstring>
#include <memory.h>
#include "pmm.hpp"

#define MAX(X, Y) ((X > Y) ? X : Y)
using namespace std;

int PESO = 100;
int main(int argc, const char *argv[])
{
    argc = 1;
    srand(time(NULL));
    lerDados(argv[1]);
    //testarDados("");
    Solucao s;
    
    construtivaAleatoria(s);
    calcularFO(s);
    escreverSolucao(s, "", 0);
 
   
    lerSolucao(s, argv[2]);
    calcularFO(s);
    escreverSolucao(s, "", 0);

    return 0;
}

void lerDados(string arq)
{
    FILE *f = fopen(arq.c_str(), "r");
    memset(&matConflitoPontos, -1, sizeof(matConflitoPontos));

    fscanf(f, "%d %d", &pontos, &posicoes);
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
    fprintf(f, "%d %d\n", pontos, posicoes);
    for (int i = 0; i < pontos * posicoes; i++)
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
    memset(&s.vetPosSel, 0, sizeof(s.vetPosSel));
    for (int i = 0; i < pontos; i++)
        s.vetPosSel[i] = 1 + (rand() % posicoes);
}

void lerSolucao(Solucao &s, string arq)
{
    int i = 0;
    memset(&s.vetPosSel, 0, sizeof(s.vetPosSel));

    FILE *f = fopen(arq.c_str(), "r");

    fscanf(f, "%d %d %d", &pontos, &posicoes, &s.funObj);
    fscanf(f, "%d", &auxiliar);
    while (i < pontos)
    {
        s.vetPosSel[i] = auxiliar;
        fscanf(f, "%d", &auxiliar);
        i++;
    }

    fclose(f);
}

void calcularFO(Solucao &s)
{
    s.funObj = pontos;
    s.conflitos = 0;
    int candidato, conflitos;
    criarVetPosicoesCandidatas();
    for (int i = 0; i < pontos; i++)
    {
        candidato = (i * posicoes) + (s.vetPosSel[i] - 1);
        conflitos = 0;
        for (int j = 1; j < matConflitoPontos[candidato][0] + 1; j++)
            if (vetPosicoesCandidatas[matConflitoPontos[candidato][j] - 1] != -1 && vetPosicoesCandidatas[matConflitoPontos[candidato][j] - 1] == s.vetPosSel[(matConflitoPontos[candidato][j] - 1) / posicoes])
                conflitos++;

        s.conflitos += conflitos;   
        vetPosicoesCandidatas[candidato] = -1;
    }
    if(s.conflitos > 0)
         s.funObj -= s.conflitos + 1;
    
}


/*
void construtivaGulosa(Solucao &s)
{
    memset(&s.vetPosSel, 0, sizeof(s.vetPosSel));
    int flag = 0;
    int cont = 0;
    int aux1, aux2;
    for (int i = 0; i < pontos; i++)
    {
        if (s.vetPosSel[vetIndObjOrd[i]] != -1)
            continue;
        else
        {
            aux1 = (vetIndObjOrd[i] * posicoes);

            for (int k = aux1; k < aux1 + posicoes; k++)
            {
                for (int j = 1; j < matConflitoPontos[k][0] + 1; j++)
                    if ((int)(matConflitoPontos[aux1][j] - 1) / posicoes != aux1 && )
                    {
                        cont++;
                        flag = 1;
                        aux2 = k;
                    }
                    else
                    {
                        flag = 0;
                        cont = 0;
                        break;
                    }
            }

            if (flag == 1 && cont == matConflitoPontos[aux1][0])
            {
                for (int j = 1; j < matConflitoPontos[aux1][0] + 1; j++)
                    if (s.vetPosSel[(matConflitoPontos[aux1][j] - 1) / posicoes] < 0)
                    {
                        s.vetPosSel[(matConflitoPontos[aux1][j] - 1) / posicoes1] = -2;
                    }
                s.vetPosSel[vetIndObjOrd[i]] = ;
            }
        }
        cont = 0;
        flag = 0;
    }
}
*/
void escreverSolucao(Solucao &s, string arq, const bool flag)
{
    FILE *f;
    const char *vazio = "";
    if (arq == vazio)
    {
        f = stdout;
        fprintf(f, "FO: %d\n", s.funObj);
        fprintf(f, "CONFLITOS: %d\n", s.conflitos);
        if (flag)
        {
            fprintf(f, "VETOR POSIÇÕES ESCOLHIDAS: ");
            for (int j = 0; j < pontos; j++)
                fprintf(f, "%d  ", s.vetPosSel[j]);
        }
        fprintf(f, "\n");
       
    }
    else
    {
        f = fopen(arq.c_str(), "w");
        fprintf(f, "%d\n%d\n%d\n", pontos, posicoes, s.funObj);
        for (int j = 0; j < pontos; j++)
            fprintf(f, "%d\n", s.vetPosSel[j]);
         fclose(f);  
    }
     
   
}



void clonarSolucao(Solucao &original, Solucao &clone)
{
    memcpy(&clone, &original, sizeof(original));
}

void criarVetPosicoesCandidatas()
{
    for (int j = 0; j < pontos * posicoes; j++)
        vetPosicoesCandidatas[j] = (j + 1) % posicoes == 0 ? posicoes : (j + 1) % posicoes;
}

void bubbleSort()
{
    double aux = 0;
    for (int i = 0; i < (pontos * posicoes); i++)
    {
        for (int j = 0; j < (pontos * posicoes) - 1; j++)
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

void selectionSort()
{
    double aux;
    int min;
    for (int i = 0; i < (pontos * posicoes) - 1; i++)
    {
        min = i;
        for (int j = i + 1; j < (pontos * posicoes); j++)
            if (vetPesObjOrd[j] < vetPesObjOrd[min])
                min = j;

        aux = vetPesObjOrd[i];
        vetPesObjOrd[i] = vetPesObjOrd[min];
        vetPesObjOrd[min] = aux;

        auxiliar = vetIndObjOrd[i];
        vetIndObjOrd[i] = vetIndObjOrd[min];
        vetIndObjOrd[min] = auxiliar;
    }
}

void insertionSort()
{
    int key, j;
    double peso;
    for (int i = 1; i < (pontos * posicoes); i++)
    {
        peso = vetPesObjOrd[i];
        key = vetIndObjOrd[i];
        j = i - 1;
        while (j >= 0 && vetPesObjOrd[j] > peso)
        {
            vetPesObjOrd[j + 1] = vetPesObjOrd[j];
            vetIndObjOrd[j + 1] = vetIndObjOrd[j];
            j = j - 1;
        }
        vetPesObjOrd[j + 1] = peso;
        vetIndObjOrd[j + 1] = key;
    }
}

void quickSort(int left, int right)
{
    int i, j, y;
    double x;
    i = left;
    j = right;
    x = vetPesObjOrd[(left + right) / 2];

    while (i <= j)
    {
        while (vetPesObjOrd[i] < x && i < right)
        {
            i++;
        }
        while (vetPesObjOrd[j] > x && j > left)
        {
            j--;
        }
        if (i <= j)
        {
            y = vetIndObjOrd[i];
            vetIndObjOrd[i] = vetIndObjOrd[j];
            vetIndObjOrd[j] = y;

            x = vetPesObjOrd[i];
            vetPesObjOrd[i] = vetPesObjOrd[j];
            vetPesObjOrd[j] = x;

            i++;
            j--;
        }
    }

    if (j > left)
    {
        quickSort(left, j);
    }
    if (i < right)
    {
        quickSort(i, right);
    }
}

/*

void construtivaGulAle(SolucaoBIN &s, const int percentual)
{
    int tam, pos, aux;
    int vetAuxInd[MAX_OBJ];

    memcpy(&vetAuxInd, &vetIndObjOrd, sizeof(vetIndObjOrd));

    tam = MAX(1, (percentual / 100.0) * (pontos * posicoes));
    for (int j = 0; j < tam; j++)
    {
        pos = j + rand() % ((pontos * posicoes) - j);
        aux = vetAuxInd[pos];
        vetAuxInd[pos] = vetAuxInd[j];
        vetAuxInd[j] = aux;
    }

    memset(&s.vetPosicoesEscolhidas, 0, sizeof(s.vetPosicoesEscolhidas));
    int flag = 0;
    int cont = 0;
    for (int i = 0; i < tam; i++)
    {
        if (s.vetPosicoesEscolhidas[vetAuxInd[i]] != 0)
            continue;
        else
        {

            for (int j = 1; j < matConflitoPontos[vetAuxInd[i]][0] + 1; j++)
                if (s.vetPosicoesEscolhidas[matConflitoPontos[vetAuxInd[i]][j] - 1] != 1)
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

            if (flag == 1 && cont == matConflitoPontos[vetAuxInd[i]][0])
            {
                for (int j = 1; j < matConflitoPontos[vetAuxInd[i]][0] + 1; j++)
                    if (s.vetPosicoesEscolhidas[matConflitoPontos[vetAuxInd[i]][j] - 1] == 0)
                    {
                        s.vetPosicoesEscolhidas[matConflitoPontos[vetAuxInd[i]][j] - 1] = -1;
                    }
                s.vetPosicoesEscolhidas[vetAuxInd[i]] = 1;
            }
        }
        cont = 0;
        flag = 0;
    }

    for (int i = 0; i < (pontos * posicoes); i++)
        if (s.vetPosicoesEscolhidas[i] == -1)
            s.vetPosicoesEscolhidas[i] = 0;
}


/*



*/

/*
void apresentacao(string arq)
{
    SolucaoBIN sol;

    clock_t h;
    double tempo;
    const int repeticoes = 1000;
    //const int percentual = 50;
    lerDados(arq);

    cout << "\n>>> TESTE - HEURISTICAS CONSTRUTIVAS - " << arq << " - " << 1 << " REPETICOES \n";

    //--
    h = clock();
    criarVetAux();

    quickSort(0, (posicoes * pontos));
    //insertionSort();
    //selectionSort();
    //bubbleSort();

    construtivaGulosaBIN(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("\nTempo de cálculo da solução inicial...: %.5f seg.\n\n", tempo);

    h = clock();
    calcularFOBIN(sol);
    escreverSolucaoBIN(sol, 0);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Tempo de cálculo da função objetivo...: %.5f seg.\n\n", tempo);

    cout << "\n>>> TESTE - HEURISTICAS CONSTRUTIVAS - " << arq << " - " << repeticoes << " REPETICOES \n";
    h = clock();
    criarVetAux();

    quickSort(0, (posicoes * pontos));
    //insertionSort();
    //selectionSort();
    //bubbleSort();

    for (int r = 0; r < repeticoes; r++)
        construtivaGulosaBIN(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("\nTempo de cálculo da solução inicial...: %.5f seg.\n\n", tempo);

    h = clock();
    for (int r = 0; r < repeticoes; r++)
        calcularFOBIN(sol);
    escreverSolucaoBIN(sol, 0);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Tempo de cálculo da função objetivo...: %.5f seg.\n\n", tempo);
}
*/
/*
void testar_heuConstrutivas(string arq)
{
    SolucaoBIN sol;

    clock_t h;
    double tempo;
    const int repeticoes = 1000;
    const int percentual = 50;

    cout << "\n>>> TESTE - HEURISTICAS CONSTRUTIVAS - " << arq << " - " << repeticoes << " REPETICOES \n";

    //--
    h = clock();
    lerDados(arq);

    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Tempo de pré processamento...: %.5f seg.\n\n", tempo);

    //--
    h = clock();
    criarVetAux();
    quickSort(0, (posicoes * pontos));
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Ordenação dos pesos por quick sort...: %.5f seg.\n", tempo);

    /* //--
    h = clock();
    criarVetAux();
    insertionSort();
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Ordenação dos pesos por insertion sort...: %.5f seg.\n", tempo);
    //--
    h = clock();
    criarVetAux();
    selectionSort();
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Ordenação dos pesos por selection sort...: %.5f seg.\n", tempo);

    //--
    h = clock();
    for (int r = 0; r < repeticoes; r++)
        construtivaAleatoriaBIN(sol);
    calcularFOBIN(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    escreverSolucaoBIN(sol, 0);
    printf("Construtiva Aleatória...: %.5f seg.\n", tempo);

    //--
    h = clock();
    for (int r = 0; r < repeticoes; r++)
        construtivaGulAle(sol, percentual);
    calcularFOBIN(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    escreverSolucaoBIN(sol, 0);
    printf("Construtiva Gulosa %d porcento Aleatória...: %.5f seg.\n", (100 - percentual), tempo);

    //--
    h = clock();
    for (int r = 0; r < repeticoes; r++)
        construtivaGulosaBIN(sol);
    calcularFOBIN(sol);
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    escreverSolucaoBIN(sol, 0);
    printf("Construtiva Gulosa...: %.5f seg.\n", tempo);
}
*/
/*




*/