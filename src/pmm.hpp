#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 106000
#define MAX_MOC 5000

#include <string>

int numObj, numMoc, auxiliar, auxiliar2;
int matConflitoPontos[MAX_OBJ][MAX_MOC];
int vetIndObjOrd[MAX_OBJ];
double vetPesObjOrd[MAX_OBJ];

void lerDados(std::string arq);
void testarDados(const char *arq);

typedef struct tSolucao
{
    int conflitos;
    int pontosLivres;
    int vetPosicoesEscolhidas[MAX_OBJ];
    int funObj;
}Solucao;

typedef struct tSolucaoBIN
{
    int conflitos;
    int pontosLivres;
    int vetPosicoesEscolhidas[MAX_OBJ];
    int funObj;
}SolucaoBIN;

void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);

void calcularFOBIN(SolucaoBIN &s);
void construtivaAleatoriaBIN(SolucaoBIN &s);
void escreverSolucaoBIN(SolucaoBIN &s, const bool flag);

void ordenarObjetos();
void construtivaGulosa(Solucao &s);
void construtivaGulosaBIN(SolucaoBIN &s);

#endif