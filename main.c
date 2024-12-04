#include <stdio.h>
#include <stdlib.h>

int main_menu(int* input_buffer);

double** DD_2D(double* x, double* y, int n);

int divided_differences_2D();

int main()
{
    int* selection = (int*)malloc(sizeof(int));
    if (selection == NULL) { 
        fprintf(stderr, "Memory allocation failed: int* selection\n"); 
        exit(1); }

    while (1)
    {
        printf("Choose a mode:\n1. 2D Divided Differences\n2. 2D Newton IP\n3. Exit\n> ");
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
                printf("Not yet implemented!\n\n");
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