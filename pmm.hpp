#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 13206
#define MAX_MOC 8

#include <string>

int numObj, numMoc, auxiliar;
int matConflitoPontos[MAX_OBJ*MAX_MOC][200];
int vetIndObjOrd[MAX_OBJ*MAX_MOC];
double vetPesObjOrd[MAX_OBJ*MAX_MOC];

void lerDados(std::string arq);
void testarDados(const char *arq);

typedef struct tSolucaoBIN
{
    int conflitos;
    int vetPosicoesEscolhidas[MAX_OBJ*MAX_MOC];
    int funObj;
} SolucaoBIN;

void bubbleSort();
void selectionSort();
void quickSort(int left, int right);
void insertionSort();

void criarVetAux();
void clonarSolucao(SolucaoBIN &original, SolucaoBIN &clone);
void lerSolucao(SolucaoBIN &sol, std::string arq);

void calcularFOBIN(SolucaoBIN &sol);
//void escreverSolucaoBIN(SolucaoBIN &s, const bool flag);
void escreverSolucaoBIN(SolucaoBIN &s, std::string arq, const bool flag);

void construtivaAleatoriaBIN(SolucaoBIN &s);
void construtivaGulAle(SolucaoBIN &s, const int percentual);
void construtivaGulosaBIN(SolucaoBIN &s);
void testar_heuConstrutivas(std::string arq);
void apresentacao(std::string arq);

typedef struct tSolucao
{
    int conflitos;
    int vetPosicoesEscolhidas[MAX_OBJ];
    int funObj;
}Solucao;

void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);
void construtivaGulosa(Solucao &s);
#endif



