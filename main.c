#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Menu functions
int main_menu(int* input_buffer);

// Controller functions
int classic_divided_differences();
int max_LP();
int weighted_divided_differences();

// Calculating functions
double** DD(double* x, double* y, int n);
double** wDD(double* delta, double* f, int n);
double** simplex(double** tableau, int rows, int cols);
double determinant_2x2(double a, double b, double c, double d);

// Printing functions
void print_simplex_tableau(double** tableau, int cols, int rows);
void print_DD_table(double** table, double* x, int n);
void print_wDD_table(double** table, double* d, int n);

int main()
{
    int* selection = (int*)malloc(sizeof(int));
    if (selection == NULL) { 
        fprintf(stderr, "Memory allocation failed: int* selection\n"); 
        exit(1); }

    while (1)
    {
        printf("Choose a mode:\n1. Classic Divided Differences\n2. Weighted Divided Differences\n3. Normal Max LP Simplex\n4. Exit\n> ");
        if(main_menu(selection)) { 
            printf("An error has occured in main_menu. Closing program...\n"); 
            exit(1); }
        
        switch (*selection)
        {
            case 1:
                // DD
                classic_divided_differences();
                break;
            case 2: 
                // wDD
                weighted_divided_differences();
                break;
            case 3:
                // Max LP Simplex
                max_LP();
                break;
            case 4: 
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

int classic_divided_differences()
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
    printf("Number of data points: %d\n", *n);

    // Allocate and validate memory for data points
    double* x = (double*)malloc(*n * (int)sizeof(double));
    double* y = (double*)malloc(*n * (int)sizeof(double));
    if (x == NULL) { fprintf(stderr, "Memory allocation failed: double* x\n"); exit(1); }
    if (y == NULL) { fprintf(stderr, "Memory allocation failed: double* y\n"); exit(1); }

    // Scan user input
    for (int i = 0; i < *n; i++) 
    {
        printf("\nEnter data point #%d:\nFormat: x y\n> ", i+1);
        while (scanf("%lf %lf", &x[i], &y[i]) != 2)
        {
            // Inform user
            printf("\nFailed to read input. Try again!\n");
            printf("Enter data point #%d:\nFormat: x y\n> ", i+1);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
        }
    }

    // Calculate the divided differences table
    double **dd_table = DD(x, y, *n);

    // Print the divided differences table
    print_DD_table(dd_table, x,*n);

    // Cleanup and return
    for (int i = 0; i < *n; i++) {
        free(dd_table[i]);
    }
    free(dd_table);
    free(n);
    free(x);
    free(y);
    return 0;
}

int weighted_divided_differences()
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
    printf("Number of data points: %d\n", *n);

    // Allocate and validate memory for data points
    double* x = (double*)malloc(*n * (int)sizeof(double));
    double* y = (double*)malloc(*n * (int)sizeof(double));
    if (x == NULL) { fprintf(stderr, "Memory allocation failed: double* x\n"); exit(1); }
    if (y == NULL) { fprintf(stderr, "Memory allocation failed: double* y\n"); exit(1); }

    // Scan user input
    for (int i = 0; i < *n; i++) 
    {
        printf("\nEnter data point #%d:\nFormat: δ f\n> ", i+1);
        while (scanf("%lf %lf", &x[i], &y[i]) != 2)
        {
            // Inform user
            printf("\nFailed to read input. Try again!\n");
            printf("Enter data point #%d:\nFormat: δ f\n> ", i+1);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
        }
    }

    // Calculate the divided differences table
    double **dd_table = wDD(x, y, *n);

    // Print the divided differences table
    print_wDD_table(dd_table, x, *n);

    // Cleanup and return
    for (int i = 0; i < *n; i++) {
        free(dd_table[i]);
    }
    free(dd_table);
    free(n);
    free(x);
    free(y);
    return 0;
}

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
    for (int row = 0; row < n; row++) 
    {
        table[row] = (double *)malloc((n - row) * sizeof(double)); // (n - row) for better memory efficiency
        if (table[row] == NULL) 
        {
            fprintf(stderr, "Memory allocation failed: double* table[%d]\n", row);
            exit(1);
        }
    }

    // Initialize the first column with f values
    for (int i = 0; i < n; i++) 
    {
      table[i][0] = f[i];
    }

    // Calculate the weighted divided differences
    for (int j = 1; j < n; j++) 
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

int max_LP()
{
    // Memory allocation
    int* rows = (int*)malloc(sizeof(int));
    *rows = 0; // Assign initial value to avoid garbage
    int* cols = (int*)malloc(sizeof(int));
    *cols = 0; // Assign initial value to avoid garbage
    int* num_vars = (int*)malloc(sizeof(int));
    *num_vars = 0; // Assign initial value to avoid garbage
    int* num_slack = (int*)malloc(sizeof(int));
    *num_slack = 0; // Assign initial value to avoid garbage
    int* num_const = (int*)malloc(sizeof(int));
    *num_const = 0; // Assign initial value to avoid garbage

    // Validate memory allocation
    if (rows == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }
    if (cols == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }
    if (num_vars == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }
    if (num_slack == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }
    if (num_const == NULL) { fprintf(stderr, "Memory allocation failed: int* rows\n"); exit(1); }

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
    *num_vars = *cols;           // Save number of variables
    *num_slack = *rows - 1;      // Save number of slack variables
    *cols += 1;                  // Add RHS column
    *cols += *num_slack;         // Add columns for slack variables

    // Allocate memory for the DD table
    double** tableau = (double **)malloc(*rows * sizeof(double *));

    // Validate memory allocation
    if (tableau == NULL) { fprintf(stderr, "Memory allocation failed: double** tableau\n"); exit(1); }

    // Allocate memory for each column
    for (size_t i = 0; i < *rows; i++) 
    {
        // Allocate
        tableau[i] = (double *)malloc(*cols * sizeof(double));

        // Validate
        if (tableau[i] == NULL) { fprintf(stderr, "Memory allocation failed: double* tableau[%zu]\n", i); exit(1); }
    }

    // Get objective function from user
    printf("Enter objective function coefficients:\nFormat: ");
    for (size_t i = 1; i <= *cols - *rows; i++) {
        printf("c%zu ", i);
    }
    printf("\n> ");

    for (size_t i = 0; i < *cols - *rows; i++) 
    {
        while (scanf("%lf", &tableau[*rows-1][i]) != 1) 
        {
            // Inform user
            printf("Value c%zu failed to read. Please try again!\n> ", i + 1);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 

            // Reset for-loop
            i = 0;
        }
        //printf("Value %lf read to column %d!\n", tableau[*rows - 1][i], i);
        tableau[*rows - 1][i] *= -1; // Inverse coefficients for tableau
    }
    tableau[*rows - 1][*cols - 1] = 0; // objective function RHS always 0 at start
    
    // Get <= restrictions from user
    int reset_loop = 0;
    for (size_t i = 0; i < *rows - 1; i++) 
    {
        // User inserted invalid input on previous row
        if (reset_loop) 
        {
            reset_loop = 0;
            i -= 1;
        }

        // Get each row seperately
        printf("\nEnter restriction row #%zu:\nFormat: ", i + 1);
        for (size_t j = 1; j <= *cols - *rows; j++) {
            printf("c%zu ", j);
        }
        printf("b%zu\n> ", i + 1);

        // Get coefficients
        for (size_t j = 0; j < *cols - *rows; j++) 
        {
            while (scanf("%lf", &tableau[i][j]) != 1) 
            {
                // Inform user
                printf("\nValue c%zu failed to read. Please try again!\n", j + 1);
                printf("Enter restriction row #%zu:\n> ", i + 1);

                // Clear the input buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF); 

                // Reset for-loop
                j = 0;
            }
            //printf("Value %lf read to column %d!\n", tableau[i][j], j);
        }

        // Get RHS value
        if (scanf("%lf", &tableau[i][*cols - 1]) != 1) 
        {
            // Inform user
            printf("Value b%zu failed to read. Please try again!\n> ", i + 1);

            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 

            // Reset for-loop
            reset_loop = 1;
        }
        //printf("Value %lf read to column %d!\n", tableau[i][*cols], *cols);
    }
    

    // Insert slack variables
    for (size_t i = 0; i < *rows; i++) 
    {
        for (size_t j = *cols - *rows; j < *cols - 1; j++) 
        {   
            if (i == j - (*cols - *rows)) 
            {  
                // Fill the diagonal with 1
                tableau[i][j] = 1;
            } 
            else 
            {
                // Fill rest with 0
                tableau[i][j] = 0;
            }
            
            if (i == *rows - 1) 
            {
                // Final row
                tableau[i][j] = 0;
            }
        }
    }

    // Print initial tableau
    printf("Initial tableau:\n");
    print_simplex_tableau(tableau, *cols, *rows);

    double** final = simplex(tableau, *rows, *cols);

    // Print final tableau
    printf("Final tableau:\n");
    print_simplex_tableau(final, *cols, *rows);

    // Present Shadow Prices
    printf("Shadow Prices:\n");
    for (size_t i = *num_vars; i < *num_vars + *num_slack; i++) 
    {
        printf("#%zu: %lf\n", i - *num_vars + 1, tableau[*rows - 1][i]);
    }
    printf("\n");

    // Allocate temporary variable
    uint8_t has_reduced_costs = 0;

    // Determine if there are any reduced costs
    for (size_t i = 0; i < *num_vars; i++) 
    {
        if (tableau[*rows - 1][i] != 0) has_reduced_costs = 1;
    }

    // Present Reduced Costs
    printf("Reduced Costs:\n");
    if (has_reduced_costs) 
    {
        for (size_t i = 0; i < *num_vars; i++) 
        {
            if(tableau[*rows - 1][i] != 0)
            {
                // Only print relevant values
                printf("x%zu: %lf\n", i - *num_vars + 1, tableau[*rows - 1][i]);
            }
        }
        printf("\n");
    } 
    else 
    {
        printf("This solution does not have any Reduced Costs.\n\n");
    }

    // Cleanup and return
    for (size_t i = 0; i < *rows; i++) { free(tableau[i]); }
    free(tableau);
    free(rows);
    free(cols);
    free(num_vars);
    free(num_slack);
    free(num_const);
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

void print_simplex_tableau(double** tableau, int cols, int rows)
{
    for (int i = 0; i < cols - rows; i++) 
    {
        printf("\tx%d", i + 1);
    }
    for (int i = 0; i < (cols - 1) - (cols - rows); i++) 
    {
        printf("\ts%d", i + 1);
    }
    printf("\tRHS\n");
    for (int i = 0; i < rows; i++) 
    {
        if (i == rows - 1) 
        {
            printf(" z|\t");
        }
        else 
        {
            printf("#%d|\t", i + 1);
        }
        for (int j = 0; j < cols; j++) 
        {
            printf("%.3lf\t", tableau[i][j]);
        }
        printf("\n\n");
    }
}

void print_DD_table(double** table, double* x, int n)
{
    printf("\nDivided Differences Table:\n");
    printf("xi\tf(xi)\n");
    for (int i = 0; i < n; i++) {
        printf("%-6.3lf|\t", x[i]);
        for (int j = 0; j < n - i; j++) {
            printf("%-10lf\t", table[i][j]);
        }
        printf("\n\n");
    }
}

void print_wDD_table(double** table, double* d, int n)
{
    printf("\nWeighted Divided Differences Table:\n");
    printf("δi\tf(δi)\n");
    for (int i = 0; i < n; i++) {
        printf("%-6.3lf|\t", d[i]);
        for (int j = 0; j < n - i; j++) {
            printf("%-10lf\t", table[i][j]);
        }
        printf("\n\n");
    }
}