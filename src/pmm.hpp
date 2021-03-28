#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 106000
#define MAX_MOC 5000

#include <string>

int numObj, numMoc, auxiliar;
int matConflitoPontos[MAX_OBJ][MAX_MOC];
int vetIndObjOrd[MAX_OBJ];
double vetPesObjOrd[MAX_OBJ];

void lerDados(std::string arq);
void testarDados(const char *arq);

typedef struct tSolucaoBIN
{
    int numObj;
    int numMoc;
    int conflitos;
    int pontosLivres;
    int vetPosicoesEscolhidas[MAX_OBJ];
    int funObj;
} SolucaoBIN;

void criarVetAux();
void clonarSolucao(SolucaoBIN &original, SolucaoBIN &clone);
void lerSolucao(std::string arq);

void bubbleSort();
void selectionSort();
void quickSort(int left, int right);
void insertionSort();

void escreverSolucaoBIN(SolucaoBIN &s, const bool flag);
void escreverSolucaoBINArquivo(SolucaoBIN &s, std::string arq);

void construtivaAleatoriaBIN(SolucaoBIN &s);
void construtivaGulAle(SolucaoBIN &s, const int percentual);
void construtivaGulosaBIN(SolucaoBIN &s);
void testar_heuConstrutivas(std::string arq);
void apresentacao(std::string arq);
#endif

/*
typedef struct tSolucao
{
    int conflitos;
    int pontosLivres;
    int vetPosicoesEscolhidas[MAX_OBJ];
    int funObj;
}Solucao;

void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);
void construtivaGulosa(Solucao &s);
*/