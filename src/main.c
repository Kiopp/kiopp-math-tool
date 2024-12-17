#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "kiopp_interface.h"

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