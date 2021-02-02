int **criaMatriz(int L, int C)
{
    int i = 0;
    int **M = (int **)malloc(L * sizeof(int *));
    for (i = 0; i < L; i++)
    {
        M[i] = (int *)malloc(C * sizeof(int));
    }
    
    return M;
}

int povoarMatriz(int **M, int L, int C)
{
    int valor = 0, i = 0, j = 0;
    for (i = 0; i < L; i++)
    {
        for (j = 0; j < C; j++)
        {
            scanf("%d", &valor);
            M[i][j] = valor;
        }
    }
    return 0;
}

int imprimirMatriz(int **M, int L, int C)
{
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