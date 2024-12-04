#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main_menu(int* input_buffer);

double** DD_2D(double* x, double* y, int n);

int divided_differences_2D();

double** simplex(double** tableau, int rows, int cols);

int max_LP();

int main()
{
    int* selection = (int*)malloc(sizeof(int));
    if (selection == NULL) { 
        fprintf(stderr, "Memory allocation failed: int* selection\n"); 
        exit(1); }

    while (1)
    {
        printf("Choose a mode:\n1. 2D Divided Differences\n2. Max LP Simplex\n3. Exit\n> ");
        if(main_menu(selection)) { 
            printf("An error has occured in main_menu. Closing program...\n"); 
            exit(1); }
        
        switch (*selection)
        {
            case 1:
                // Divided Differences
                divided_differences_2D();
                break;
            case 2:
                // Max LP Simplex
                max_LP();
                break;
            case 3: 
                // Exit
                exit(0);
                break;
            default:
                printf("No action found for choice: %d\n\n", *selection);
                break;
        }
    } 

    return 0;
}

int main_menu(int* input_buffer)
{
    // Scan user input
    while (scanf("%d", input_buffer) != 1)
    {
        // Inform user
        printf("Failed to read input. Try again!\n> ");

        // Clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 
    }

    // Send feedback to user
    printf("Selected choice: %d\n\n", *input_buffer);

    // Leave main menu
    return 0;
}

int divided_differences_2D()
{
    // Memory allocation
    int* n = (int*)malloc(sizeof(int));
    *n = 0; // Assign initial value to avoid garbage

    // Validate memory allocation
    if (n == NULL) { fprintf(stderr, "Memory allocation failed: int* n\n"); exit(1); }

    // Scan user input
    printf("How many data points? (1 or more required)\n> ");
    while (scanf("%d", n) != 1 || *n < 1)
    {
        // Inform user
        printf("Failed to read input. 1 or more data points required. Try again!\n> ");

        // Clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 
    }
    
    printf("Number of data points: %d\n\n", *n);

    // Allocate and validate memory for data points
    double* x = (double*)malloc(*n * (int)sizeof(double));
    double* y = (double*)malloc(*n * (int)sizeof(double));
    if (x == NULL) { fprintf(stderr, "Memory allocation failed: double* x\n"); exit(1); }
    if (y == NULL) { fprintf(stderr, "Memory allocation failed: double* y\n"); exit(1); }

    // Scan user input
    printf("Data point input format: x y\n");
    for (int i = 0; i < *n; i++) 
    {
        printf("Data point #%d: x y\n> ", i+1);
        while (scanf("%lf %lf", &x[i], &y[i]) != 2)
        {
            // Inform user
            printf("Failed to read input. Try again!\n");
            printf("Data point number %d: x y\n> ", i+1);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
        }
    }

    // Print read data points
    for (int i = 0; i < *n; i++) 
    {
        printf("Data point #%d: (%lf, %lf)\n", i+1, x[i], y[i]);
    }

    // Calculate the divided differences table
    double **dd_table = DD_2D(x, y, *n);

    // Print the divided differences table
    printf("\nDivided Differences Table:\n");
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n - i; j++) {
            printf("%.3lf\t", dd_table[i][j]);
        }
        printf("\n\n");
    }

    // Cleanup and return
    for (int i; i < *n; i++) {
        free(dd_table[i]);
    }
    free(dd_table);
    free(n);
    free(x);
    free(y);
    return 0;
}

double** DD_2D(double* x, double* y, int n)
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
    for (int row = 0; row < n; row++) 
    {
        table[row] = (double *)malloc((n - row) * sizeof(double)); // (n - row) for better memory efficiency
        if (table[row] == NULL) 
        {
            fprintf(stderr, "Memory allocation failed: double* table[%d]\n", row);
            exit(1);
        }
    }

    // Initialize the first column with y values
    for (int row = 0; row < n; row++) 
    {
      table[row][0] = y[row];
    }

    // Calculate the divided differences
    for (int col = 1; col < n; col++) 
    {
      for (int row = 0; row < n - col; row++) 
      {
        table[row][col] = (table[row + 1][col - 1] - table[row][col - 1]) / (x[row + col] - x[row]);
      }
    }
    return table;
}

int max_LP()
{
    // Memory allocation
    int* rows = (int*)malloc(sizeof(int));
    *rows = 0; // Assign initial value to avoid garbage
    int* cols = (int*)malloc(sizeof(int));
    *cols = 0; // Assign initial value to avoid garbage

    // Validate memory allocation
    if (rows == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }
    if (cols == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }

    // Get number of constraints
    printf("How many constraint rows?\n> ");
    while (scanf("%d", rows) != 1 || *rows < 1)
    {
        // Inform user
        printf("Failed to read input. 1 or more constraints required. Try again!\n> ");

        // Clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 
    }
    printf("Number of constraints: %d\n\n", *rows);
    *rows  += 1; // Add row for objective function

    // Get number of variables
    printf("How many variables?\n> ");
    while (scanf("%d", cols) != 1 || *cols < 1)
    {
        // Inform user
        printf("Failed to read input. 1 or more variables required. Try again!\n> ");

        // Clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF); 
    }
    printf("Number of variables: %d\n\n", *cols);
    *cols += 1; // Add RHS column
    *cols += *rows - 1; // Add columns for slack variables

    // Allocate memory for the DD table
    double** tableau = (double **)malloc(*rows * sizeof(double *));

    // Validate memory allocation
    if (tableau == NULL) { fprintf(stderr, "Memory allocation failed: double** tableau\n"); exit(1); }

    // Allocate memory for each column
    for (int i = 0; i < *rows; i++) 
    {
        // Allocate
        tableau[i] = (double *)malloc(*cols * sizeof(double));

        // Validate
        if (tableau[i] == NULL) { fprintf(stderr, "Memory allocation failed: double* tableau[%d]\n", i); exit(1); }
    }

    // Get tableau values from the user
    printf("Enter the initial tableau values row by row, each value seperated by spaces:\n> ");
    for (int i = 0; i < *rows; i++) 
    {
        for (int j = 0; j < *cols; j++) 
        {
            while (scanf("%lf", &tableau[i][j]) != 1) 
            {
                // Inform user
                printf("Failed to read value at index [%d][%d]. Please try again!\n", i, j);

                // Clear the input buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF); 
            }
        }
    }

    // Print initial tableau
    printf("Initial tableau:\n");
    for (int i = 0; i < *rows; i++) 
    {
        for (int j = 0; j < *cols; j++) 
        {
            printf("%.3lf\t", tableau[i][j]);
        }
        printf("\n\n");
    }

    double** final = simplex(tableau, *rows, *cols);

    // Print final tableau
    printf("Final tableau:\n");
    for (int i = 0; i < *rows; i++) 
    {
        for (int j = 0; j < *cols; j++) 
        {
            printf("%.3lf\t", final[i][j]);
        }
        printf("\n\n");
    }

    // Cleanup and return
    for (int i; i < *rows; i++) {
        free(tableau[i]);
    }
    free(tableau);
    free(rows);
    free(cols);
    return 0;
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