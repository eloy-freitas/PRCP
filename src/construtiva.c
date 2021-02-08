int solucaoSementeAleatoria(int **M, int L, int C)
{
    int i, sorteio;
    
    for (i = 0; i < L; i++)
    {
        sorteio = rand() % C;
        M[i][sorteio] = 1;
    }
    return 0;
}