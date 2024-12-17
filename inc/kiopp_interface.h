#ifndef KIOPP_INTERFACE_H
#define KIOPP_INTERFACE_H

// Menu functions
int main_menu(int* input_buffer);

// Controller functions
int classic_divided_differences();
int max_LP();
int weighted_divided_differences();

// Printing functions
void print_simplex_tableau(double** tableau, int cols, int rows);
void print_DD_table(double** table, double* x, int n);
void print_wDD_table(double** table, double* d, int n);

#endif