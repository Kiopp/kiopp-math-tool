#include "kiopp_math.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double** DD(double* x, double* y, int n)
{
    // Allocate memory for the DD table
    double** table = (double **)malloc(n * sizeof(double *));

    // Validate memory allocation
    if (table == NULL) 
    {
        fprintf(stderr, "Memory allocation failed: double** table\n");
        exit(1);
    }

    // Allocate memory for each column
    for (size_t row = 0; row < n; row++) 
    {
        table[row] = (double *)malloc((n - row) * sizeof(double)); // (n - row) for better memory efficiency
        if (table[row] == NULL) 
        {
            fprintf(stderr, "Memory allocation failed: double* table[%zu]\n", row);
            exit(1);
        }
    }

    // Initialize the first column with y values
    for (size_t row = 0; row < n; row++) 
    {
      table[row][0] = y[row];
    }

    // Calculate the divided differences
    for (size_t col = 1; col < n; col++) 
    {
      for (size_t row = 0; row < n - col; row++) 
      {
        table[row][col] = (table[row + 1][col - 1] - table[row][col - 1]) / (x[row + col] - x[row]);
      }
    }
    return table;
}

double** wDD(double* d, double* f, int n)
{
    // Allocate memory for the DD table
    double** table = (double **)malloc(n * sizeof(double *));

    // Validate memory allocation
    if (table == NULL) 
    {
        fprintf(stderr, "Memory allocation failed: double** table\n");
        exit(1);
    }

    // Allocate memory for each column
    for (size_t row = 0; row < n; row++) 
    {
        table[row] = (double *)malloc((n - row) * sizeof(double)); // (n - row) for better memory efficiency
        if (table[row] == NULL) 
        {
            fprintf(stderr, "Memory allocation failed: double* table[%zu]\n", row);
            exit(1);
        }
    }

    // Initialize the first column with f values
    for (size_t i = 0; i < n; i++) 
    {
      table[i][0] = f[i];
    }

    // Calculate the weighted divided differences
    for (size_t j = 1; j < n; j++) 
    {
        for (int i = 0; i < n - j; i++) 
        {
            table[i][j] = determinant_2x2(d[i], table[i][j - 1], d[i + j], table[i + 1][j - 1]) / (d[i] - d[i + j]);
        }
    }
    return table;
}

double determinant_2x2(double a, double b, double c, double d)
{
    return (a*d) - (b*c);
}

double** simplex(double** tableau, int rows, int cols)
{
    int pivot_row, pivot_col;
    double pivot_element;
    while (1)
    {   
        // 1. Find pivot column, most negative entry in objective row
        pivot_col = 0;
        double min_val = tableau[rows - 1][0];

        for (size_t i = 1; i < cols; i++) 
        {
            if (tableau[rows - 1][i] < min_val) 
            {
                min_val = tableau[rows - 1][i];
                pivot_col = i;
            }
        }

        // If all entries in objective row are non-negative the solution is optimal
        if (min_val >= 0) break;
        
        // 2. Find pivot row, smallest non-negative ratio
        pivot_row = -1;
        double min_ratio = INFINITY;

        for (size_t i = 0; i < rows - 1; i++) 
        {
            if (tableau[i][pivot_col] > 0) 
            {
                double ratio = tableau[i][cols - 1] / tableau[i][pivot_col];
                if (ratio < min_ratio) 
                {
                    min_ratio = ratio;
                    pivot_row = i;
                }
            }
        }

        // If no positive entry in pivot column the problem is unbounded
        if (pivot_row == -1) 
        {
            printf("Provided tableau input contains an unbounded solution!\n");
            return NULL;
        }

        // 3. Perform pivot operation
        pivot_element = tableau[pivot_row][pivot_col];

        // Divide pivot row by pivot element
        for (size_t i = 0; i < cols; i++) 
        {
            tableau[pivot_row][i] /= pivot_element;
        }

        // Eliminate other entries in the pivot column
        for (size_t i = 0; i < rows; i++) 
        {
            if (i != pivot_row) 
            {
                double factor = tableau[i][pivot_col];
                for (size_t j = 0; j < cols; j++) 
                {
                    tableau[i][j] -= factor * tableau[pivot_row][j];
                }
            }
        }
    }

    return tableau;
}