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



void lerSolucao(string arq){
    SolucaoBIN sol;
    memset(&sol.vetPosicoesEscolhidas, 0, sizeof(sol.vetPosicoesEscolhidas));
    FILE *f = fopen(arq.c_str(), "r");
   
    fscanf(f, "%d %d %d", &sol.numObj, &sol.numMoc, &sol.funObj);
    for (int i = 0; auxiliar == EOF; i++)
    {
        fscanf(f, "%d", &auxiliar);
        sol.vetPosicoesEscolhidas[auxiliar] = 1;
       
    }
    fclose(f);
}

void clonarSolucao(SolucaoBIN &original, SolucaoBIN &clone){
   
    clone.funObj = original.funObj;
    clone.conflitos = original.conflitos;
    clone.pontosLivres = original.pontosLivres;
    memcpy(&clone.vetPosicoesEscolhidas, &original.vetPosicoesEscolhidas, sizeof(original.vetPosicoesEscolhidas));

}

void criarVetAux()
{
    for (int j = 0; j < (numObj * numMoc); j++)
    {
        vetIndObjOrd[j] = j;
        vetPesObjOrd[j] = (double)matConflitoPontos[j][0] / (numObj * numMoc);
    }
}

void bubbleSort()
{
    double aux = 0;

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

void selectionSort()
{

    double aux;
    int min;
    for (int i = 0; i < (numObj * numMoc) - 1; i++)
    {
        min = i;
        for (int j = i + 1; j < (numObj * numMoc); j++)
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
    for (int i = 1; i < (numObj * numMoc); i++)
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

void construtivaGulosaBIN(SolucaoBIN &s)
{
    memset(&s.vetPosicoesEscolhidas, 0, sizeof(s.vetPosicoesEscolhidas));
    int flag = 0;
    int cont = 0;
    for (int i = 0; i < (numObj * numMoc); i++)
    {
        if (s.vetPosicoesEscolhidas[vetIndObjOrd[i]] != 0)
            continue;
        else
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
    for (int i = 0; i < (numObj * numMoc); i++)
        if (s.vetPosicoesEscolhidas[i] == -1)
            s.vetPosicoesEscolhidas[i] = 0;
}

void construtivaAleatoriaBIN(SolucaoBIN &s)
{
    for (int j = 0; j < numObj * numMoc; j++)
        s.vetPosicoesEscolhidas[j] = rand() % 2;
}

void construtivaGulAle(SolucaoBIN &s, const int percentual)
{
    int tam, pos, aux;
    int vetAuxInd[MAX_OBJ];

    memcpy(&vetAuxInd, &vetIndObjOrd, sizeof(vetIndObjOrd));

    tam = MAX(1, (percentual / 100.0) * (numObj * numMoc));
    for (int j = 0; j < tam; j++)
    {
        pos = j + rand() % ((numObj * numMoc) - j);
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

    for (int i = 0; i < (numObj * numMoc); i++)
        if (s.vetPosicoesEscolhidas[i] == -1)
            s.vetPosicoesEscolhidas[i] = 0;
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
    criarVetAux();
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Tempo de pré processamento...: %.5f seg.\n\n", tempo);

    //--
    h = clock();
    quickSort(0, (numMoc*numObj));
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Ordenação dos pesos por quick sort...: %.5f seg.\n", tempo);

   /* //--
    h = clock();
    insertionSort();
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Ordenação dos pesos por insertion sort...: %.5f seg.\n", tempo);
    //--
    h = clock();
    selectionSort();
    h = clock() - h;
    tempo = (double)h / CLOCKS_PER_SEC;
    printf("Ordenação dos pesos por selection sort...: %.5f seg.\n", tempo);*/

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

/*
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
*/