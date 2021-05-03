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
    int seed = 0, rcl;
    string instancia = "", saida = "", aux = "GRASP", sol = "";
    double tempo, tempo_limite = 5, melhor_tempo, tempo_total;
    Solucao s;
    clock_t h;

    if (argc > 1)
    {
        seed = stoi(argv[1]);
        instancia = argv[2];
        tempo_limite = stof(argv[3]);
        saida = argv[4];
        rcl = stoi(argv[5]);
        sol = argv[6];
        srand(seed);

        lerDados(instancia);
        //testarDados("");
        h = clock();

        criarVetAuxiliares();
        h = clock() - h;
        tempo = (double)h / CLOCKS_PER_SEC;
        printf("Tempo de pré-processamento...: %.5f seg.\n\n", tempo);
        grasp(rcl, tempo_limite, s, melhor_tempo, tempo_total);
        escreverSolucao(s, sol.c_str(), 1);
        FILE *f = fopen(saida.c_str(), "at");
        fprintf(f, "%s\t%s\t%d\t\t%d\t%d\t%.5f\t\t%.5f\n", aux.c_str(), instancia.c_str(), seed, s.funObj, rcl, melhor_tempo, tempo_total);
        fclose(f);
    }

    return 0;
}

void lerDados(string arq)
{
    int valor;
    FILE *f = fopen(arq.c_str(), "r");
    memset(&matConflitoPontos, -1, sizeof(matConflitoPontos));
    memset(&vetConflitosPosicao, 0, sizeof(vetConflitosPosicao));
    fscanf(f, "%d %d", &pontos, &posicoes);
    for (int i = 0; i < (pontos * posicoes); i++)
    {
        fscanf(f, "%d", &vetConflitosPosicao[i]);
        for (int j = 0; j < vetConflitosPosicao[i]; j++)
        {
            fscanf(f, "%d", &valor);
            matConflitoPontos[i][j] = valor - 1;
        }
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
        s.vetPosSel[i] = rand() % posicoes;
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
        s.vetPosSel[i] = auxiliar - 1;
        fscanf(f, "%d", &auxiliar);
        i++;
    }

    fclose(f);
}

void calcularFO(Solucao &s)
{
    s.funObj = 0;
    s.conflitos = 0;
    int candidato, conflitos, pontosLivres = 0;
    memset(&vetPontosLivres, 0, sizeof(vetPontosLivres));
    memset(&vetQtdConflitos, 0, sizeof(vetQtdConflitos));
    for (int i = 0; i < pontos; i++)
    {
        candidato = (i * posicoes) + s.vetPosSel[i];
        conflitos = 0;
        for (int j = 0; j < vetConflitosPosicao[candidato]; j++)
            if (vetPosicoesCandidatas[matConflitoPontos[candidato][j]] == s.vetPosSel[matConflitoPontos[candidato][j] / posicoes])
                conflitos++;

        vetQtdConflitos[i] = conflitos;
        if (conflitos == 0)
            vetPontosLivres[i] = 1;
    }

    for (int j = 0; j < pontos; j++)
    {
        s.conflitos += vetQtdConflitos[j];
        if (vetPontosLivres[j] == 1)
            pontosLivres++;
    }

    s.funObj = pontosLivres;
    s.conflitos /= 2;
}

void construtivaGulosa(Solucao &s)
{
    memset(&s.vetPosSel, -1, sizeof(s.vetPosSel));
    int posMenorConflito, candidato;
    for (int i = 0; i < pontos; i++)
        if (s.vetPosSel[i] == -1)
        {
            candidato = i * posicoes;
            posMenorConflito = vetConflitosPosicao[candidato];
            for (int j = candidato; j < posicoes + candidato - 1; j++)
            {
                if (vetConflitosPosicao[j] < vetConflitosPosicao[j + 1])
                    posMenorConflito = j;
            }
            s.vetPosSel[i] = vetPosicoesCandidatas[posMenorConflito];
        }
}

void construtivaGulosaAleatoria(Solucao &s, int rcl)
{
    memset(&s.vetPosSel, -1, sizeof(s.vetPosSel));
    int tam, pos;
    tam = (int)MAX(1, (rcl / 100.0) * pontos);

    for (int j = 0; j < tam; j++)
    {
        pos = j + rand() % (pontos - j);
        s.vetPosSel[pos] = rand() % posicoes;
    }

    int posMenorConflito, candidato;
    for (int i = 0; i < pontos; i++)
        if (s.vetPosSel[i] == -1)
        {
            candidato = i * posicoes;
            posMenorConflito = vetConflitosPosicao[candidato];
            for (int j = candidato; j < posicoes + candidato - 1; j++)
            {
                if (vetConflitosPosicao[j] < vetConflitosPosicao[j + 1])
                    posMenorConflito = j;
            }
            s.vetPosSel[i] = vetPosicoesCandidatas[posMenorConflito];
        }
}

void grasp(int rcl, const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total)
{

    clock_t hI, hF;
    Solucao s_vizinha;
    printf("\n\n>>> EXECUTANDO O GRASP...\n\n");
    tempo_total = tempo_melhor = 0;
    hI = clock();
    s.funObj = 0;

    while (tempo_total < tempo_max)
    {
        construtivaGulosaAleatoria(s_vizinha, rcl);
        calcularFO(s_vizinha);
        heuBLPM(s_vizinha, tempo_max, tempo_total);
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

void heuBLPM(Solucao &s, const double tempo_max, double &tempo_total)
{
    int vetObjAux[MAX_OBJ]; // usado para evitar determinismo na ordem de teste dos objetos
    int mocOri, foOri, indice, aux;
    clock_t hI, hF;
    int melFO = s.funObj;
    for (int j = 0; j < pontos; j++)
        vetObjAux[j] = j;
    hI = clock();
INICIO:;
    hF = clock();
    tempo_total = ((double)(hF - hI)) / CLOCKS_PER_SEC;
    if (tempo_total < tempo_max)
    {
        foOri = s.funObj;
        for (int j = 0; j < pontos; j++)
        {
            indice = j + rand() % (pontos - j);
            mocOri = s.vetPosSel[vetObjAux[indice]];
            for (int i = 0; i < posicoes; i++)
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
                fprintf(f, "%d  ", s.vetPosSel[j] + 1);
        }
        fprintf(f, "\n");
    }
    else
    {
        f = fopen(arq.c_str(), "w");
        fprintf(f, "%d\n%d\n%d\n", pontos, posicoes, s.funObj);
        for (int j = 0; j < pontos; j++)
            fprintf(f, "%d\n", s.vetPosSel[j] + 1);
        fclose(f);
    }
}

void clonarSolucao(Solucao &original, Solucao &clone)
{
    memcpy(&clone, &original, sizeof(original));
}

void criarVetAuxiliares()
{
    for (int j = 0; j < pontos * posicoes; j++)
        vetPosicoesCandidatas[j] = j % posicoes;
}








