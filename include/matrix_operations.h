#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <vector>

// Função para multiplicar matrizes de forma sequencial
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2);

// Função para multiplicar uma parte da matriz de forma concorrente
void multiplyPart(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2, std::vector<std::vector<int>>& result,
                  int startRow, int endRow);

#endif // MATRIX_OPERATIONS_H
