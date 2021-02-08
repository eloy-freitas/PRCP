int **criaMatriz(int L, int C)
{
    printf("criando matriz...\n");
    int i = 0;
    int **M = (int **)malloc(L * sizeof(int *));
    for (i = 0; i < L; i++)
    {
        M[i] = (int *)malloc(C * sizeof(int));
    }

    return M;
}

int lerArquivo(int **M, int L, int C)
{
    printf("lendo arquivo...\n");
    int valor = 0, aux = 0, i = 0, j = 0, k = 0;

    for (i = 0; i < L; i++)
    {
        scanf("%d", &aux);
        for (j = 0; j < aux; j++)
        {

            scanf("%d", &valor);

            M[i][valor - 1] = valor - 1;
        }
    }

    return 0;
}

int imprimirMatriz(int **M, int L, int C)
{
    printf("imprimindo matriz...\n");
    int i = 0, j = 0;
    for (i = 0; i < L; i++)
    {
        for (j = 0; j < C; j++)
        {
            printf("%d \t", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}

//Preencher todas as posições da matriz de entrada com um inteiro
int padronizarMatriz(int **M, int L, int C, int inteiro)
{
    int i, j;

    for (i = 0; i < L; i++)
    {
        for (j = 0; j < C; j++)
        {
            M[i][j] = inteiro;
        }
    }
    return 0;
}