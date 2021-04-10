#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 13206
#define MAX_MOC 8

#include <string>

int pontos, posicoes, auxiliar;
int matConflitoPontos[MAX_OBJ * MAX_MOC][200];
int vetPosicoesCandidatas[MAX_OBJ * MAX_MOC];
int vetIndObjOrd[MAX_OBJ * MAX_MOC];
double vetPesObjOrd[MAX_OBJ * MAX_MOC];

typedef struct tSolucao
{
    int conflitos;
    int vetPosSel[MAX_OBJ];
    int funObj;
} Solucao;

void lerDados(std::string arq);
void testarDados(const char *arq);

void bubbleSort();
void selectionSort();
void quickSort(int left, int right);
void insertionSort();

void criarVetPosicoesCandidatas();
void clonarSolucao(Solucao &original, Solucao &clone);
void lerSolucao(Solucao &s, std::string arq);

void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void escreverSolucao(Solucao &s, std::string arq, const bool flag);
void construtivaGulosa(Solucao &s);
#endif
/*
typedef struct tSolucaoBIN
{
    int conflitos;
    int vetPosicoesEscolhidas[MAX_OBJ*MAX_MOC];
    int funObj;
} SolucaoBIN;

void calcularFOBIN(SolucaoBIN &sol);
//void escreverSolucaoBIN(SolucaoBIN &s, const bool flag);
void escreverSolucaoBIN(SolucaoBIN &s, std::string arq, const bool flag);
void construtivaAleatoriaBIN(SolucaoBIN &s);
void construtivaGulAle(SolucaoBIN &s, const int percentual);
void construtivaGulosaBIN(SolucaoBIN &s);
void testar_heuConstrutivas(std::string arq);
void apresentacao(std::string arq);

*/
