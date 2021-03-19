#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 50000
#define MAX_MOC 5000

#include <string>

int numObj, numMoc, auxiliar;
int matConflitoPontos[MAX_OBJ][MAX_MOC];


void lerDados(std::string arq);
void testarDados(const char *arq);

typedef struct tSolucao
{
    int conflitos;
    int vetPosicoesEscolhidas[MAX_MOC];
    int funObj;
}Solucao;

void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);

#endif