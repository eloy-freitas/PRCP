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

int **lerArquivo(int L, int C)
{
    int **M = criaMatriz(L * C, 1);
    
    printf("lendo arquivo...\n");
    int aux = 0;
     printf("aqui");
    for (int i = 0; i < L; i++)
    {
       
                scanf("%d", &aux);
        aux++;
       
        M[i] = (int *)malloc(aux  * sizeof(int));
        for (int j = 0; j < aux; j++)
        {
            M[i][j] = aux - 1;
            scanf("%d", &aux);
        }
    }

    return M;
}

int imprimirMatrizDoArquivo(int **M, int L)
{
    int aux = 0;
    for (int i = 0; i < L; i++)
    {
        aux = M[i][0];
        for (int j = 1; j < aux; j++)
        {
            printf("%d\t", M[i][j]);
        }
        printf("\n");
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