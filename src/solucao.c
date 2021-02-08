typedef struct solucao Solucao;

struct solucao
{
    int id, fo, pl, nc, L, C;
    int **matriz;
    Solucao *prox;
};

Solucao *criarLista()
{
    Solucao *aux = (Solucao *)malloc(sizeof(Solucao));
    aux->id = 0;
    aux->prox = NULL;
    return aux;
}

int inserirSolucao(Solucao *lista, Solucao *solucao)
{
    printf("inserindo solução...\n");
    if (lista->prox == NULL)
    {

        lista->prox = solucao;
    }
    else
    {

        Solucao *aux = lista->prox;

        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }

        aux->prox = solucao;
        return 0;
    }
}

Solucao *criarSolucao(int id, int fo, int pl, int nc, int L, int C)
{
    Solucao *aux = (Solucao *)malloc(sizeof(Solucao));
    aux->id = id;
    aux->fo = fo;
    aux->pl = pl;
    aux->nc = nc;
    aux->L = L;
    aux->C = C;
    aux->matriz = criaMatriz(L, C);
    solucaoSementeAleatoria(aux->matriz, L, C);
    aux->prox = NULL;
    return aux;
}

int imprimirPosicoesSelecionadas(int **matriz, int L, int C)
{
    int i, j;
    for (i = 0; i < L; i++)
    {
        for (j = 0; j < C; j++)
        {
            if (matriz[i][j] == 1)
            {
                printf("\t\tPosição selecionada para o ponto %d: %d\n", i + 1, j + 1);
                break;
            }
        }
    }

    return 0;
}

int imprimirSolucoes(Solucao *lista)
{
    Solucao *solucao;
    for (solucao = lista->prox; solucao != NULL; solucao = solucao->prox)
    {
        printf("solução: %d\n", solucao->id);
        printf("\tnúmero de pontos: %d\n", solucao->L);
        printf("\tnúmero de posições candidatas: %d\n", solucao->C);
        printf("\tnúmero de pontos livres: %d\n", solucao->pl);
        printf("\tnúmero de conflitos: %d\n", solucao->nc);
        printf("\tvalor da fo: %d\n", solucao->fo);
        printf("\tposições selecionadas:\n");
        imprimirPosicoesSelecionadas(solucao->matriz, solucao->L, solucao->C);
    }
    return 0;
}

int limparSolucao(Solucao *solucao)
{
    if (solucao != NULL)
    {
        Solucao *aux = solucao->prox;
        solucao->matriz = NULL;
        free(solucao);
        limparSolucao(aux);
    }
    return 0;
}