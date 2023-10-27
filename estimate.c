#include <stdio.h>
#include <stdlib.h>

double **multiply(double **matrix1, double **matrix2, int rows, int cols, int common)
{
    double **result = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        result[i] = malloc(cols * sizeof(double));
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i][j] = 0;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int x = 0; x < common; x++)
            {
                result[i][j] += matrix1[i][x] * matrix2[x][j];
            }
        }
    }
    return result;
}

double **trans(double **matrix, int rows, int cols)
{
    double **result = malloc(cols * sizeof(double *));
    for (int i = 0; i < cols; i++)
    {
        result[i] = malloc(rows * sizeof(double));
    }

    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}

double **inverse(double **matrix, int size)
{
    double **identity = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++)
    {
        identity[i] = malloc(size * sizeof(double));
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == j)
            {
                identity[i][j] = 1.0;
            }
            else
            {
                identity[i][j] = 0.0;
            }
        }
    }

    for (int p = 0; p < size; p++)
    {
        double pivot = matrix[p][p];
        for (int i = 0; i < size; i++)
        {
            matrix[p][i] /= pivot;
            identity[p][i] /= pivot;
        }

        for (int i = 0; i < size; i++)
        {
            if (i != p)
            {
                pivot = matrix[i][p];
                for (int j = 0; j < size; j++)
                {
                    matrix[i][j] -= matrix[p][j] * pivot;
                    identity[i][j] -= identity[p][j] * pivot;
                }
            }
        }
    }

    return identity;
}

void freeMatrix(double **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

double **getWeights(double **train, double **Y, int rows, int cols, int common)
{
    double **A=trans(train, rows, cols);
    double **B=multiply(A, train, cols, cols, rows);
    double **C = inverse(B, cols);
    double **D = multiply(C, A, cols, rows, cols);
    double **E = multiply(D, Y, cols, 1, rows);
    freeMatrix(A, cols);
    freeMatrix(B, cols);
    freeMatrix(C, cols);
    freeMatrix(D, cols);
    return E;
}

double **getEstimate(double **data, double **W, int rows, int common)
{
    return multiply(data, W, rows, 1, common);
}

int main(int argv, char **argc)
{
    if (argv != 3)
    {
        printf("error");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argc[1], "r");
    if (file == NULL)
    {
        printf("error");
        exit(EXIT_FAILURE);
    }

    char type[6];
    fscanf(file, "%5s", type);

    int trainK;
    fscanf(file, "%d", &trainK);

    int n;
    fscanf(file, "%d", &n);

    double **training = malloc((n) * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        training[i] = malloc((trainK + 1) * sizeof(double));
    }

    double **Y = malloc((n) * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        Y[i] = malloc(sizeof(double));
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < trainK + 2; j++)
        {
            if (j == 0)
            {
                training[i][j] = 1.0;
            }
            else if (j == trainK + 1)
            {
                double value;
                fscanf(file, "%lf", &value);
                Y[i][0] = value;
            }
            else
            {
                double value;
                fscanf(file, "%lf", &value);
                training[i][j] = value;
            }
        }
    }

    file = fopen(argc[2], "r");
    if (file == NULL)
    {
        printf("error");
        exit(EXIT_FAILURE);
    }

    char type2[6];
    fscanf(file, "%5s", type2);

    int dataK;
    fscanf(file, "%d", &dataK);

    int m;
    fscanf(file, "%d", &m);

    if (trainK != dataK)
    {
        printf("error");
        exit(EXIT_FAILURE);
    }

    double **data = malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++)
    {
        data[i] = malloc((dataK + 1) * sizeof(double));
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < dataK + 1; j++)
        {
            if (j == 0)
            {
                data[i][j] = 1;
            }
            else
            {
                double value;
                fscanf(file, "%lf", &value);
                data[i][j] = value;
            }
        }
    }
    double **weights = getWeights(training, Y, n, trainK + 1, trainK + 1);
    double **estimate = getEstimate(data, weights, m, dataK + 1);
    for (int i = 0; i < m; i++)
    {
        printf("%.0f\n", estimate[i][0]);
    }

    freeMatrix(training, n);
    freeMatrix(data, m);
    freeMatrix(Y, n);
    freeMatrix(weights, trainK+1);
    freeMatrix(estimate, m);
    fclose(file);
    exit(EXIT_SUCCESS);
}
