#include <stdio.h>
#include <stdlib.h>

double **multiply(double **matrix1, double **matrix2)
{
    int rows = sizeof(matrix1) / sizeof(matrix1[0]);
    int cols = sizeof(matrix2[0]) / sizeof(matrix2[0][0]);
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
            result[i][j] += matrix1[i][j] * matrix2[j][i];
        }
    }
    return result;
}

double **trans(double **matrix)
{
    int rows = sizeof(matrix)/sizeof(matrix[0]);
    int cols = sizeof(matrix[0])/sizeof(matrix[0][0]);
    double **result = malloc(rows * sizeof(double *));
    for (int i = 0; i < cols; i++)
    {
        result[i] = malloc(rows * sizeof(double));
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

double **inverse(double **matrix)
{
    int size = sizeof(matrix)/sizeof(matrix[0]);
    double **identity = malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++)
    {
        identity[i] = malloc(size * sizeof(double));
    }

    for (int i = 0; i < size; i++)
    {
        int pivot = matrix[i][i];
        for (int x = i; x < size; x++)
        {
            matrix[i][x] /= pivot;
            identity[i][x] /= pivot;
        }
        for (int j = i + 1; j < size; j++)
        {
            pivot = matrix[j][i];
            for (int n = 0; n < size; n++)
            {
                matrix[j][n] -= matrix[i][n] * pivot;
                identity[j][n] -= identity[i][n] * pivot;
            }
        }
    }

    for (int i = size - 1; i > 0; i--)
    {
        for (int j = i; j > 0; j--)
        {
            matrix[i][j - 1] -= matrix[i][j - 1] * matrix[i][j];
        }
    }

    return identity;
}

double **getWeights(double **train, double **Y){
    return multiply(multiply(inverse(multiply(trans(train), train)), trans(train)), Y);
}

double **getEstimate(double **data, double **W){
    return multiply(data, W);
}

int main(int argv, char **argc)
{
    if (argv != 3)
    {
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argc[1], "r");
    if (file == NULL)
    {
        printf("error");
        exit(EXIT_FAILURE);
    }

    char *type;
    fscanf(file, "%5s", type);

    int trainK;
    fscanf(file, "%d", &trainK);

    int n;
    fscanf(file, "%d", &n);

    double **training = malloc((n) * sizeof(double*));
    for (int i = 0; i < n; i++)
    {
        training[i] = malloc((trainK + 1) * sizeof(double));
    }

    double **Y = malloc((n) * sizeof(double));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < trainK + 1; j++)
        {
            if (j == 0)
            {
                training[i][j] = 1;
            }
            else if (j == trainK)
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

    char *type2;
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

    double **estimate = getEstimate(data, getWeights(training, Y));
    for(int i=0; i<sizeof(estimate)/sizeof(estimate[0]); i++){
        printf("%lf\n", estimate[i][0]);
    }
    fclose(file);
    exit(EXIT_SUCCESS);
}
