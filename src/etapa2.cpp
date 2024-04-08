#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include "matrix_operations.h" // Inclui o arquivo de cabeçalho com as operações de matriz

using namespace std;
using namespace std::chrono;

// Função para gerar matrizes aleatórias
void generateRandomMatrices(const string& filename1, const string& filename2, int n1, int m1, int n2, int m2) {
    ofstream file1(filename1);
    ofstream file2(filename2);

    file1 << n1 << " " << m1 << endl;
    file2 << n2 << " " << m2 << endl;

    srand(time(NULL));

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            file1 << rand() % 100 << " ";
        }
        file1 << endl;
    }

    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            file2 << rand() % 100 << " ";
        }
        file2 << endl;
    }

    file1.close();
    file2.close();
}

// Função para calcular o tempo médio da multiplicação de matrizes
milliseconds calculateAverageTime(const string& executable, const string& matrix1, const string& matrix2, int P) {
    milliseconds totalDuration = milliseconds::zero();

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        system((executable + " " + matrix1 + " " + matrix2 + " " + to_string(P)).c_str());
        auto stop = high_resolution_clock::now();
        totalDuration += duration_cast<milliseconds>(stop - start);
    }

    return totalDuration / 10;
}

milliseconds calculateAverageTimeSequential(const string& executable, const string& matrix1, const string& matrix2) {
    milliseconds totalDuration = milliseconds::zero();

    for (int i = 0; i < 10; ++i) {
        auto start = high_resolution_clock::now();
        system((executable + " " + matrix1 + " " + matrix2).c_str());
        auto stop = high_resolution_clock::now();
        totalDuration += duration_cast<milliseconds>(stop - start);
    }

    return totalDuration / 10;
}

int main() {
    vector<int> matrix_sizes = {100, 200, 400, 800, 1600, 3200}; // Tamanhos das matrizes a serem testadas
    int initial_size = 100; // Tamanho inicial da matriz
    int max_sequential_time = 120000; // Tempo sequencial máximo em milissegundos (2 minutos)
    string sequential_executable = "./bin/sequential"; // Executável sequencial
    string parallel_threads_executable = "./bin/parallel_threads"; // Executável paralelo com threads
    string parallel_processes_executable = "./bin/parallel_processes"; // Executável paralelo com processos

    // Abrir arquivo CSV para escrever os resultados
    ofstream outputFile("dados.csv");
    if (!outputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return 1;
    }

    // Escrever cabeçalho do arquivo CSV
    outputFile << "Tamanho da Matriz, P (Processos), P (Threads), Tempo (Processos), Tempo (Threads), Tempo (Sequencial)" << endl;

    // E1: Experimentação com vários tamanhos de matriz
    for (int size : matrix_sizes) {
        cout << "Tamanho da matriz: " << size << "x" << size << endl;

        string matrix1 = "matrix1_" + to_string(size) + ".txt";
        string matrix2 = "matrix2_" + to_string(size) + ".txt";
        generateRandomMatrices(matrix1, matrix2, size, size, size, size);

        milliseconds sequential_time = calculateAverageTimeSequential(sequential_executable, matrix1, matrix2);

        int P = size * size / 8; // Número de threads/processos
        milliseconds parallel_threads_time = calculateAverageTime(parallel_threads_executable, matrix1, matrix2, P);
        milliseconds parallel_processes_time = calculateAverageTime(parallel_processes_executable, matrix1, matrix2, P);

        // Escrever resultados no arquivo CSV
        outputFile << size << ", " << P << ", " << P << ", " << parallel_processes_time.count() << ", " << parallel_threads_time.count() << ", " << sequential_time.count() << endl;

        if (sequential_time.count() > max_sequential_time) break;
    }

    // E2: Experimentação com diferentes valores de P para a matriz 3200x3200
    int fixed_size = 3200;
    string fixed_matrix1 = "matrix1_" + to_string(fixed_size) + ".txt";
    string fixed_matrix2 = "matrix2_" + to_string(fixed_size) + ".txt";
    generateRandomMatrices(fixed_matrix1, fixed_matrix2, fixed_size, fixed_size, fixed_size, fixed_size);

    int initial_P = (fixed_size * fixed_size) / 32; // Começa com 1/4 de (3200*3200)/8
    int final_P = (fixed_size * fixed_size) / 2;
    int step = (final_P - initial_P) / 10; // Incremento/decremento de P

    for (int P = initial_P; P <= final_P; P += step) {
        milliseconds parallel_threads_time = calculateAverageTime(parallel_threads_executable, fixed_matrix1, fixed_matrix2, P);
        milliseconds parallel_processes_time = calculateAverageTime(parallel_processes_executable, fixed_matrix1, fixed_matrix2, P);

        // Escrever resultados no arquivo CSV
        outputFile << fixed_size << ", " << P << ", " << P << ", " << parallel_processes_time.count() << ", " << parallel_threads_time.count() << ", NULL" << endl;
    }

    // Fechar o arquivo CSV
    outputFile.close();

    cout << "Resultados salvos em dados.csv" << endl;

    return 0;
}
