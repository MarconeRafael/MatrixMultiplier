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

int main() {
    vector<int> matrix_sizes = {100, 200, 400, 800, 1600}; // Tamanhos das matrizes a serem testadas
    int initial_size = 100; // Tamanho inicial da matriz
    int max_sequential_time = 120000; // Tempo sequencial máximo em milissegundos (2 minutos)
    string sequential_executable = "./bin/sequential"; // Executável sequencial
    string parallel_threads_executable = "./bin/parallel_threads"; // Executável paralelo com threads
    string parallel_processes_executable = "./bin/parallel_processes"; // Executável paralelo com processos

    // E1: Experimentação com vários tamanhos de matriz
    cout << "E1: Experimentação com vários tamanhos de matriz" << endl;
    cout << "--------------------------------------------" << endl;

    for (int size : matrix_sizes) {
        if (size < initial_size) continue;

        cout << "Tamanho da matriz: " << size << "x" << size << endl;

        string matrix1 = "matrix1_" + to_string(size) + ".txt";
        string matrix2 = "matrix2_" + to_string(size) + ".txt";
        generateRandomMatrices(matrix1, matrix2, size, size, size, size);

        milliseconds sequential_time = calculateAverageTime(sequential_executable, matrix1, matrix2, 1);
        cout << "Tempo Sequencial: " << sequential_time.count() << " ms" << endl;

        int P = size * size / 8; // Número de threads/processos
        milliseconds parallel_threads_time = calculateAverageTime(parallel_threads_executable, matrix1, matrix2, P);
        cout << "Tempo Paralelo com Threads: " << parallel_threads_time.count() << " ms" << endl;

        milliseconds parallel_processes_time = calculateAverageTime(parallel_processes_executable, matrix1, matrix2, P);
        cout << "Tempo Paralelo com Processos: " << parallel_processes_time.count() << " ms" << endl;

        cout << endl;

        if (sequential_time.count() > max_sequential_time) break;
    }

    // E2: Experimentação com diferentes valores de P
    cout << "E2: Experimentação com diferentes valores de P" << endl;
    cout << "----------------------------------------------" << endl;

    string matrix1 = "matrix1_100.txt"; // Usando as mesmas matrizes anteriores

    for (int P = initial_size * initial_size / 8; P <= initial_size * initial_size / 2; P *= 2) {
        milliseconds parallel_threads_time = calculateAverageTime(parallel_threads_executable, matrix1, matrix1, P);
        cout << "P = " << P << ", Tempo Paralelo com Threads: " << parallel_threads_time.count() << " ms" << endl;

        milliseconds parallel_processes_time = calculateAverageTime(parallel_processes_executable, matrix1, matrix1, P);
        cout << "P = " << P << ", Tempo Paralelo com Processos: " << parallel_processes_time.count() << " ms" << endl;

        cout << endl;
    }

    return 0;
}
