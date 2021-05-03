#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 13206
#define MAX_MOC 8
#define MAX_RCL 30

#include <string>

int pontos, posicoes, auxiliar;
int matConflitoPontos[MAX_OBJ * MAX_MOC][200];
int vetPosicoesCandidatas[MAX_OBJ * MAX_MOC];
int vetConflitosPosicao[MAX_OBJ * MAX_MOC];
int vetPontosLivres[MAX_OBJ];
int vetQtdConflitos[MAX_OBJ];

typedef struct tSolucao
{
    int conflitos;
    int vetPosSel[MAX_OBJ];
    int funObj;
} Solucao;

void lerDados(std::string arq);
void testarDados(const char *arq);

void criarVetAuxiliares();
void clonarSolucao(Solucao &original, Solucao &clone);
void lerSolucao(Solucao &s, std::string arq);

void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void escreverSolucao(Solucao &s, std::string arq, const bool flag);
void construtivaGulosa(Solucao &s);
void construtivaGulosaAleatoria(Solucao &s, int rcl);
void corrigirSolucao(Solucao &s);

void heuBLPM(Solucao &s, const double tempo_max, double &tempo_total);

void grasp(int rcl, const double tempo_max, Solucao &s, double &tempo_melhor, double &tempo_total);
#endif
