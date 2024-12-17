# kiopps-math-tool

## Features
- **Classic Divided Differences(DD):** A simple tool for calculating an entire DD table. Can take 1 or more data points as input. The data points consist of a variable(x) and the function(y) value at that point. Top row of table represents the coefficients of a Newton interpolation through the data points.

- **Weighted Divided Differences(wDD):** A simple tool for calculating the entire wDD table. Can take 1 or more data points as input. The data points consist of a variable(δ) and the function(f) value at that point. Used to evaluate the interpolation at a given point, also called Neville scheme.

- **Max LP Simplex:** A simple tool for calculating normal max LP problems using the Simplex method. Input the objective function's coefficients, the restriction's coefficients and RHS. Prints both the initial tableau and the final tableau. 

## Compilation
Compilation should be easy and straight-forward on any system with CMake.

### Linux
1. Create build folder: 
```
mkdir build && cd build
```
2. Create Makefile using CMake:
```
cmake ..
```
3. Compile using Make:
```
make
```
4. Run program:
```
./math_tool
```