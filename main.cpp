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
    Solucao s;
    construtivaAleatoria(s);
    calcularFO(s);
    escreverSolucao(s, 0);
    // apresentacao(argv[1]);
    //testar_heuConstrutivas(argv[1]);

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
    memset(&s.vetIdMocObj, -1, sizeof(s.vetIdMocObj));
    for (int i = 0; i < numObj; i++)
        s.vetIdMocObj[i] = (rand() % (numMoc + 1)) - 1;
}

void calcularFO(Solucao &s)
{
    s.funObj = 0;
    s.conflitos = 0;
    int aux1, aux2;
    for (int i = 0; i < numObj; i++)
    {
        aux2 = 0;
        if (s.vetIdMocObj[i] != -1)
        {
            aux1 = (i * numMoc) + s.vetIdMocObj[i];
           
            for (int j = 1; j < matConflitoPontos[aux1][0] + 1; j++)
                //verifica se é uma posição candidata do mesmo ponto    &&    verifica se a posição que causa sobreescrita foi escolhida
                if ((int)(matConflitoPontos[aux1][j] - 1) / numMoc != i && s.vetIdMocObj[(matConflitoPontos[aux1][j] - 1) / numMoc] == (numMoc - (matConflitoPontos[aux1][j] % numMoc)) - 1)
                    aux2++;
        }
        if (aux2 > 0)
            s.conflitos += aux2;
        else
            s.funObj++;
    }
    s.funObj -= PESO * MAX(0, s.conflitos);
}

void construtivaGulosa(Solucao &s)
{
    memset(&s.vetIdMocObj, 0, sizeof(s.vetIdMocObj));
    int flag = 0;
    int cont = 0;
    int aux1, aux2;
    for (int i = 0; i < numObj; i++)
    {
        if (s.vetIdMocObj[vetIndObjOrd[i]] == -1)
            continue;
        else
        {
            aux1 = (i * numMoc) + s.vetIdMocObj[i];

            for (int j = 1; j < matConflitoPontos[vetIndObjOrd[aux1]][0] + 1; j++)
                if (s.vetIdMocObj[matConflitoPontos[vetIndObjOrd[aux1]][j] - 1] != 1)
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
                    if (s.vetIdMocObj[matConflitoPontos[vetIndObjOrd[i]][j] - 1] == 0)
                    {
                        s.vetIdMocObj[matConflitoPontos[vetIndObjOrd[i]][j] - 1] = -1;
                    }
                s.vetIdMocObj[vetIndObjOrd[i]] = 1;
            }
        }
        cont = 0;
        flag = 0;
    }
}

void escreverSolucao(Solucao &s, const bool flag)
{
    printf("\nFO: %d\n", s.funObj);
    printf("CONFLITOS: %d\n", s.conflitos);
    if (flag)
    {
        printf("VETOR POSIÇÕES ESCOLHIDAS: ");
        for (int j = 0; j < numObj; j++)
            printf("%d  ", s.vetIdMocObj[j]);
    }
    printf("\n");
}

void clonarSolucao(SolucaoBIN &original, SolucaoBIN &clone)
{
    memcpy(&clone, &original, sizeof(original));
}
/*
void criarVetAux()
{
    for (int j = 0; j < (numObj * numMoc); j++)
    {
        vetIndObjOrd[j] = j;
        vetPesObjOrd[j] = (double)matConflitoPontos[j][0] / numObj;
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
    {
        s.vetPosicoesEscolhidas[j] = rand() % 2;
        //construtivaAleatoriaBIN(SolucaoBIN &s) Só escolhe 0 ou 1
        
    }
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

/*void escreverSolucaoBIN(SolucaoBIN &s, const bool flag)
{
    printf("FO: %d\n", s.funObj);
    printf("CONFLITOS: %d\n", s.conflitos);
    printf("PONTOS LIVRES: %d\n", s.pontosLivres);
    if (flag)
    {
        printf("VETOR POSIÇÕES ESCOLHIDAS: ");
        for (int j = 0; j < numObj * numMoc; j++)
            printf("%d  ", s.vetPosicoesEscolhidas[j]);
    }
    printf("\n");
}*/
/*
void escreverSolucaoBIN(SolucaoBIN &s, string arq, const bool flag)
{
    FILE *f;
    const char *vazio = "";
    if (arq == vazio)
    {
        f = stdout;
        fprintf(f, "FO: %d\n", s.funObj);
        fprintf(f, "CONFLITOS: %d\n", s.conflitos);
        fprintf(f, "PONTOS LIVRES: %d\n", s.pontosLivres);
        if (flag)
        {
            fprintf(f, "VETOR POSIÇÕES ESCOLHIDAS: ");
            for (int j = 0; j < numObj*numMoc; j++)
                fprintf(f, "%d  ", s.vetPosicoesEscolhidas[j]);
        }
        fprintf(f, "\n");
    }
    else
    {
        f = fopen(arq.c_str(), "w");
        fprintf(f, "%d\n%d\n%d\n", numObj, numMoc, s.funObj);
        for (int j = 0; j < numObj*numMoc; j++)
            if (s.vetPosicoesEscolhidas[j] == 1)
                fprintf(f, "%d\n", j);
    }

    fclose(f);
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
    //s.pontosLivres = MAX(0, s.funObj - s.conflitos);
    s.funObj -= PESO * MAX(0, s.conflitos);
}

void lerSolucao(SolucaoBIN &sol, string arq)
{

    FILE *f = fopen(arq.c_str(), "r");
    memset(&sol.vetPosicoesEscolhidas, 0, sizeof(sol.vetPosicoesEscolhidas));
    fscanf(f, "%d %d %d", &numObj, &numMoc, &sol.funObj);
    for (int i = 0; i < (numObj * numMoc); i++)
    {
        fscanf(f, "%d", &auxiliar);
        sol.vetPosicoesEscolhidas[auxiliar] = 1;
    }

    fclose(f);
}
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

    quickSort(0, (numMoc * numObj));
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

    quickSort(0, (numMoc * numObj));
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
    quickSort(0, (numMoc * numObj));
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





*/