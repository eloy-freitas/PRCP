#ifndef PMM_H_INCLUDED

#define PMM_H_INCLUDED
#define MAX_OBJ 25000
#define MAX_MOC 1000 

#include <string>

int numObj, numMoc;
int vetValObj[MAX_OBJ];
int vetPesObj[MAX_OBJ];
int vetCapMoc[MAX_MOC];
int vetIndObjOrd[MAX_OBJ];

void lerDados(std::string arq);
void testarDados(const char *arq);
void ordenarObjetos();

typedef struct tSolucao
{
    int vetIdMocObj[MAX_OBJ];
    int vetPesMoc[MAX_MOC];
    int funObj;
}Solucao;


void calcularFO(Solucao &s);
void construtivaAleatoria(Solucao &s);
void construtivaGulosa(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);

//---------------ESTRUTUTA DE DADOS AUXILIARES----------
//apenas para comparar as soluções
typedef struct tSolucaoBIN
{
    int vetObjetos[MAX_OBJ];
    int vetPesMoc[MAX_MOC];
    int funObj;
}SolucaoBIN;

void calcularFOBIN(SolucaoBIN &s);
void construtivaAleatoriaBIN(SolucaoBIN &s);
void escreverSolucaoBIN(SolucaoBIN &s, const bool flag);

void testar_heuConstrutivas(std::string arq);
void heuConAleGul(Solucao &s, const int percentual);





#endif