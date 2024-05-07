#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    if(a.rows!=b.rows || a.cols!=b.cols)
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");

        return create_matrix(0,0);
    }

    Matrix result = create_matrix(a.rows, a.cols);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }

    return result;
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    if(a.rows!=b.rows || a.cols!=b.cols)
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");

        return create_matrix(0,0);
    }

    Matrix result = create_matrix(a.rows, a.cols);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }

    return result;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    if (a.cols != b.rows)
    {
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");

        return create_matrix(0, 0);
    }

     Matrix result = create_matrix(a.rows, b.cols);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < b.cols; j++)
        {
            result.data[i][j] = 0;
            for (int k = 0; k < a.cols; k++)
            {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }

    return result;
}

Matrix scale_matrix(Matrix a, double k)
{
    Matrix result = create_matrix(a.rows, a.cols);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] * k;
        }
    }

    return result;
}

Matrix transpose_matrix(Matrix a)
{
    Matrix result = create_matrix(a.cols, a.rows);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[j][i] = a.data[i][j];
        }
    }

    return result;
}

double det_matrix(Matrix a)
{
    double det = 0;

    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");

        return 0;
    }

    if (a.rows == 1)
    {
        return a.data[0][0];
    }else if (a.rows == 2)
    {
        return a.data[0][0] * a.data[1][1] - a.data[0][1] * a.data[1][0];
    }else {
        for (int i = 0; i < a.cols; i++)
        {
            Matrix temp = create_matrix(a.rows - 1, a.cols - 1);

            for (int j = 1; j < a.rows; j++)
            {
                for (int k = 0; k < a.cols; k++)
                {
                    if (k < i)
                    {
                        temp.data[j - 1][k] = a.data[j][k];
                    }else if (k > i)
                    {
                        temp.data[j - 1][k - 1] = a.data[j][k];
                    }
                }
            }

            det += a.data[0][i] * (i % 2 ? -1 : 1) * det_matrix(temp);
        }
    }

    return det;
}

Matrix inv_matrix(Matrix a)
{
    int n = a.rows;
    double det = det_matrix(a);
    int i,j;

    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");

        return create_matrix(0, 0);
    }

    if (det == 0)
    {
        printf("Error: The matrix is singular.\n");

        return create_matrix(0, 0);
    }

    Matrix result = create_matrix(n, n);
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){

            Matrix temp = create_matrix(n-1, n-1);
            int x,y;
            for (x = 0; x < n; x++){
                for (y = 0; y < n; y++){
                    if (x < i && y < j){
                        temp.data[x][y] = a.data[x][y];
                    }else if (x < i && y > j){
                        temp.data[x][y-1] = a.data[x][y];
                    }else if (x > i && y < j){
                        temp.data[x-1][y] = a.data[x][y];
                    }else if (x > i && y > j){
                        temp.data[x-1][y-1] = a.data[x][y];
                    }
                }
            }
            result.data[j][i] = det_matrix(temp) / det * ((i+j)%2 ? -1 : 1);
        }
    }

    return result;
    
}

int rank_matrix(Matrix a)
{
    int rank = 0;
    int i,j;
    int row = a.rows;
    int col = a.cols;

    for (i = 0; i < row; i++){

        int max = i;
        for (j = i+1; j < row; j++){
            if (fabs(a.data[j][i]) > fabs(a.data[max][i])){
                max = j;
            }
        }   

        for (j = 0; j < col; j++){
            double temp = a.data[i][j];
            a.data[i][j] = a.data[max][j];
            a.data[max][j] = temp;
        }

        for (j = 0; j < row; j++){
            if (j != i){
                double temp = a.data[j][i] / a.data[i][i];
                for (int k = i; k < col; k++){
                    a.data[j][k] -= a.data[i][k] * temp;
                }
            }
        }
    }

    for (i = 0; i < row; i++){
        int all_zero_flag = 1;
        for (j = 0; j < col; j++){
            if (a.data[i][j] != 0){
                all_zero_flag = 0;
                break;
            }
        }
        if (!all_zero_flag){
            rank++;
        }
}

    return rank;
}

double trace_matrix(Matrix a)
{
    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");

        return 0;
    }

    double trace = 0;

    for (int i = 0; i < a.rows; i++)
    {
        trace += a.data[i][i];
    }

    return trace;
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}