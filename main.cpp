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
    int seed = 0, lrc;
    string instancia = "", saida = "", aux = "GRASP";
    double tempo, tempo_limite = 5, melhor_tempo, tempo_total;
    Solucao s;
    clock_t h;

    if (argc > 1)
    {
        seed = stoi(argv[1]);
        instancia = argv[2];
        tempo_limite = stof(argv[3]);
        saida = argv[4];
        lrc = stoi(argv[5]);
        srand(seed);

        lerDados(instancia);
        h = clock();
        criarVetPosicoesCandidatas();
        criarVetAuxiliares();
        selectionSort();
        h = clock() - h;
        tempo = (double)h / CLOCKS_PER_SEC;
        printf("Tempo de pré-processamento...: %.5f seg.\n\n", tempo);

        grasp(lrc, tempo_limite, s, melhor_tempo, tempo_total);
        
        FILE *f = fopen(saida.c_str(),"at");
        fprintf(f, "%s\t%s\t%d\t\t%d\t%d\t%d\t%.5f\t\t%.5f\n", aux.c_str(), instancia, seed, s.funObj, s.conflitos, lrc, melhor_tempo, tempo_total);
        fclose(f);
    }

    return 0;
}

void lerDados(string arq)
{
    FILE *f = fopen(arq.c_str(), "r");
    memset(&matConflitoPontos, -1, sizeof(matConflitoPontos));
    memset(&vetConflitosPosicao, 0, sizeof(vetConflitosPosicao));
    fscanf(f, "%d %d", &pontos, &posicoes);
    for (int i = 0; i < (pontos * posicoes) * 2; i++)
    {
        fscanf(f, "%d", &vetConflitosPosicao[i]);

        for (int j = 0; j < vetConflitosPosicao[i]; j++)
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
        for (int j = 0; j < vetConflitosPosicao[i]; j++)
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

    for (int i = 0; i < pontos; i++)
    {
        candidato = (i * posicoes) + (s.vetPosSel[i] - 1);
        conflitos = 0;
        for (int j = 0; j < vetConflitosPosicao[candidato]; j++)
            if (vetPosicoesCandidatas[matConflitoPontos[candidato][j] - 1] == s.vetPosSel[(matConflitoPontos[candidato][j] - 1) / posicoes])
                conflitos++;

        s.conflitos += conflitos;
        if (conflitos > 0)
            s.funObj--;
    }
    s.conflitos /= 2;
}

void construtivaGulosa(Solucao &s)
{
    memset(&s.vetPosSel, 0, sizeof(s.vetPosSel));
    int flag = 0;
    int cont = 0;
    int candidatos = 0;

    for (int i = 0; i < pontos * posicoes; i++)
    {
        if (s.vetPosSel[vetIndPosicoesOrd[i] / posicoes] == 0 && vetIndPosicoesOrd[i] != -1)
        {
            for (int k = 0; k < vetConflitosPosicaoOrd[vetIndPosicoesOrd[i]]; k++)
            {
                if (vetPosicoesCandidatas[matConflitoPontos[vetIndPosicoesOrd[i]][k] - 1] != s.vetPosSel[(matConflitoPontos[vetIndPosicoesOrd[i]][k] - 1) / posicoes])
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
            }

            if (flag == 1 && cont == vetConflitosPosicaoOrd[vetIndPosicoesOrd[i]])
            {
                for (int k = 0; k < vetConflitosPosicaoOrd[vetIndPosicoesOrd[i]]; k++)
                    vetIndPosicoesOrd[matConflitoPontos[vetIndPosicoesOrd[i]][k] - 1] = -1;

                s.vetPosSel[vetIndPosicoesOrd[i] / posicoes] = vetPosicoesCandidatas[i];
                candidatos++;
            }
        }
        cont = 0;
        flag = 0;
    }
}

void construtivaGulosaAleatoria(Solucao &s, const int lrc)
{
    memset(&s.vetPosSel, 0, sizeof(s.vetPosSel));
    int flag = 0;
    int cont = 0;
    int tam, pos;
  

    tam = MAX(1, (lrc / 100.0) * pontos);
    for (int j = 0; j < tam; j++)
    {
        pos = rand() % pontos;
        s.vetPosSel[pos] = 1 + (rand() % posicoes);
    }

    for (int i = 0; i < pontos * posicoes; i++)
    {
        if (s.vetPosSel[vetIndPosicoesOrd[i] / posicoes] == 0)
        {
            for (int k = 0; k < vetConflitosPosicao[vetIndPosicoesOrd[i]]; k++)
            {
                if (vetPosicoesCandidatas[matConflitoPontos[vetIndPosicoesOrd[i]][k] - 1] != s.vetPosSel[(matConflitoPontos[vetIndPosicoesOrd[i]][k] - 1) / posicoes])
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
            }

            if (flag == 1 && cont == vetConflitosPosicao[vetIndPosicoesOrd[i]])
                s.vetPosSel[vetIndPosicoesOrd[i] / posicoes] = vetPosicoesCandidatas[vetIndPosicoesOrd[i]];
               
        }
        cont = 0;
        flag = 0;
    }
}

void grasp(int lrc, const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total)
{
    int aux;
    clock_t hI, hF;
    Solucao s_vizinha;
    printf("\n\n>>> EXECUTANDO O GRASP...\n\n");
    tempo_total = tempo_melhor = 0;
    hI = clock();
    s.funObj = 0;
    while (tempo_total < tempo_max)
    {
        construtivaGulosaAleatoria(s_vizinha, lrc);
        calcularFO(s_vizinha);
        heuBLRA(s_vizinha);
           
        if (s_vizinha.funObj > s.funObj)
        {
            memcpy(&s, &s_vizinha, sizeof(s_vizinha));
            hF = clock();
            tempo_melhor = ((double)(hF - hI)) / CLOCKS_PER_SEC;
        }
        hF = clock();
        tempo_total = ((double)(hF - hI)) / CLOCKS_PER_SEC;
    }
}

//lrc aleatorio
/*void graspA(const int lrc, const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total)
{
    int aux;
    clock_t hI, hF;
    Solucao s_vizinha;
    printf("\n\n>>> EXECUTANDO O GRASP...\n\n");
    tempo_total = tempo_melhor = 0;
    hI = clock();
    s.funObj = 0;
    while (tempo_total < tempo_max)
    {
        construtivaGulosaAleatoria(s_vizinha, lrc);
        calcularFO(s_vizinha);
        aux = rand() % 3;
        switch (aux)
        {
        case 0:
            heuBLPM(s_vizinha);
            break;
        case 1:
            heuBLMM(s_vizinha);
            break;
        case 2:
            heuBLRA(s_vizinha, 100);
            break;
        default:
            heuBLPM(s_vizinha);
            break;
        }

        if (s_vizinha.funObj > s.funObj)
        {
            memcpy(&s, &s_vizinha, sizeof(s_vizinha));
            hF = clock();
            tempo_melhor = ((double)(hF - hI)) / CLOCKS_PER_SEC;
        }
        hF = clock();
        tempo_total = ((double)(hF - hI)) / CLOCKS_PER_SEC;

        aux = 1;
    }
}
*/
void heuBLPM(Solucao &s)
{
    int vetObjAux[MAX_OBJ]; // usado para evitar determinismo na ordem de teste dos objetos
    int mocOri, foOri, indice, aux;
    int melFO = s.funObj;
    for (int j = 0; j < pontos; j++)
        vetObjAux[j] = j;
INICIO:;
    foOri = s.funObj;
    for (int j = 0; j < pontos; j++)
    {
        indice = j + rand() % (pontos - j);
        mocOri = s.vetPosSel[vetObjAux[indice]];
        for (int i = 1; i < posicoes + 1; i++)
        {
            if (i != mocOri)
            {
                s.vetPosSel[vetObjAux[indice]] = i;
                calcularFO(s);
                if (s.funObj > melFO)
                {
                    melFO = s.funObj;

                    goto INICIO;
                }
                else
                {
                    s.vetPosSel[vetObjAux[indice]] = mocOri;
                    s.funObj = foOri;
                }
            }
        }
        aux = vetObjAux[j];
        vetObjAux[j] = vetObjAux[indice];
        vetObjAux[indice] = aux;
    }
    calcularFO(s);
}

void heuBLMM(Solucao &s)
{
    int mocOri, foOri, melObj, melMoc, flag;
    int melFO = s.funObj;
    while (true)
    {
        flag = 0;
        foOri = s.funObj;
        for (int j = 0; j < pontos; j++)
        {
            mocOri = s.vetPosSel[j];
            for (int i = 0; i < posicoes + 1; i++)
            {
                if (i != mocOri)
                {
                    s.vetPosSel[j] = i;
                    calcularFO(s);
                    if (s.funObj > melFO)
                    {
                        melFO = s.funObj;
                        melObj = j;
                        melMoc = i;
                        flag = 1;
                    }
                }
            }
            s.vetPosSel[j] = mocOri;
            s.funObj = foOri;
        }
        if (flag)
        {
            s.vetPosSel[melObj] = melMoc;
            s.funObj = melFO;
        }
        else
            break;
    }
    calcularFO(s);
}

void heuBLRA(Solucao &s, const int iteracoes)
{
    int obj, moc, mocOri, foOri, flag;
    int melFO = s.funObj;
    while (true)
    {
        flag = 1;
        for (int t = 0; t < iteracoes; t++)
        {
            foOri = s.funObj;
            obj = rand() % pontos;
            do
                moc = rand() % (posicoes + 1) - 1;
            while (moc == s.vetPosSel[obj]);
            mocOri = s.vetPosSel[obj];
            s.vetPosSel[obj] = moc;
            calcularFO(s);
            if (s.funObj > melFO)
            {
                melFO = s.funObj;
                flag = 0;
            }
            else
            {
                s.vetPosSel[obj] = mocOri;
                s.funObj = foOri;
            }
        }
        if (flag)
            break;
    }
    calcularFO(s);
}

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

void criarVetAuxiliares()
{

    for (int j = 0; j < pontos * posicoes; j++)
        vetIndPosicoesOrd[j] = j;
}

void bubbleSort()
{
    double aux = 0;
    for (int i = 0; i < (pontos * posicoes); i++)
    {
        for (int j = 0; j < (pontos * posicoes) - 1; j++)
        {

            if (vetConflitosPosicaoOrd[j] > vetConflitosPosicaoOrd[j + 1])
            {

                auxiliar = vetIndPosicoesOrd[j];
                vetIndPosicoesOrd[j] = vetIndPosicoesOrd[j + 1];
                vetIndPosicoesOrd[j + 1] = auxiliar;

                aux = vetConflitosPosicaoOrd[j];
                vetConflitosPosicaoOrd[j] = vetConflitosPosicaoOrd[j + 1];
                vetConflitosPosicaoOrd[j + 1] = aux;
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
            if (vetConflitosPosicaoOrd[j] < vetConflitosPosicaoOrd[min])
                min = j;

        aux = vetConflitosPosicaoOrd[i];
        vetConflitosPosicaoOrd[i] = vetConflitosPosicaoOrd[min];
        vetConflitosPosicaoOrd[min] = aux;

        auxiliar = vetIndPosicoesOrd[i];
        vetIndPosicoesOrd[i] = vetIndPosicoesOrd[min];
        vetIndPosicoesOrd[min] = auxiliar;
    }
}

void insertionSort()
{
    int key, j;
    double peso;
    for (int i = 1; i < (pontos * posicoes); i++)
    {
        peso = vetConflitosPosicaoOrd[i];
        key = vetIndPosicoesOrd[i];
        j = i - 1;
        while (j >= 0 && vetConflitosPosicaoOrd[j] > peso)
        {
            vetConflitosPosicaoOrd[j + 1] = vetConflitosPosicaoOrd[j];
            vetIndPosicoesOrd[j + 1] = vetIndPosicoesOrd[j];
            j = j - 1;
        }
        vetConflitosPosicaoOrd[j + 1] = peso;
        vetIndPosicoesOrd[j + 1] = key;
    }
}

void quickSort(int left, int right)
{
    int i, j, y;
    double x;
    i = left;
    j = right;
    x = vetConflitosPosicaoOrd[(left + right) / 2];

    while (i <= j)
    {
        while (vetConflitosPosicaoOrd[i] < x && i < right)
        {
            i++;
        }
        while (vetConflitosPosicaoOrd[j] > x && j > left)
        {
            j--;
        }
        if (i <= j)
        {
            y = vetIndPosicoesOrd[i];
            vetIndPosicoesOrd[i] = vetIndPosicoesOrd[j];
            vetIndPosicoesOrd[j] = y;

            x = vetConflitosPosicaoOrd[i];
            vetConflitosPosicaoOrd[i] = vetConflitosPosicaoOrd[j];
            vetConflitosPosicaoOrd[j] = x;

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

    memcpy(&vetAuxInd, &vetIndPosicoesOrd, sizeof(vetIndPosicoesOrd));

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