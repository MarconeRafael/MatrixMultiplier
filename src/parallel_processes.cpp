#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <iomanip> // Para definir a precisão de saída

using namespace std;
using namespace std::chrono;

// Função para multiplicar uma parte da matriz de forma concorrente
void multiplyPart(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2, vector<vector<int>>& result,
                  int startRow, int endRow) {
    int n1 = matrix1.size();
    int m1 = matrix1[0].size();
    int m2 = matrix2[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < m2; ++j) {
            for (int k = 0; k < m1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Verificar se o número de argumentos é válido
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " matrix1.txt matrix2.txt P" << endl;
        return 1;
    }

    // Abrir o arquivo da primeira matriz
    ifstream inFile1(argv[1]);
    if (!inFile1.is_open()) {
        cerr << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    // Abrir o arquivo da segunda matriz
    ifstream inFile2(argv[2]);
    if (!inFile2.is_open()) {
        cerr << "Erro ao abrir o arquivo " << argv[2] << endl;
        return 1;
    }

    // Ler as dimensões das matrizes
    int n1, m1, n2, m2;
    inFile1 >> n1 >> m1;
    inFile2 >> n2 >> m2;

    // Verificar se as dimensões são compatíveis para multiplicação
    if (m1 != n2) {
        cerr << "Número de colunas da primeira matriz não é igual ao número de linhas da segunda matriz." << endl;
        return 1;
    }

    // Ler as matrizes dos arquivos
    vector<vector<int>> matrix1(n1, vector<int>(m1, 0));
    vector<vector<int>> matrix2(n2, vector<int>(m2, 0));
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m1; ++j) {
            inFile1 >> matrix1[i][j];
        }
    }
    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < m2; ++j) {
            inFile2 >> matrix2[i][j];
        }
    }

    // Fechar os arquivos
    inFile1.close();
    inFile2.close();

    // Obter o número de processos a serem usados
    int P = atoi(argv[3]);

    // Calcular a quantidade de linhas por processo
    int chunkSize = n1 / P;

    // Vetor para armazenar os tempos de execução dos processos
    vector<milliseconds> processTimes(P);

    // Calcular a quantidade de arquivos de resultado
    int numFiles = n1 * m2 / P;

    // Calcular o tempo total de execução
    auto startTotal = high_resolution_clock::now();

    // Executar a multiplicação de matrizes utilizando processos
    for (int fileId = 0; fileId < numFiles; ++fileId) {
        // Abrir o arquivo para escrita
        string fileName = "result_" + to_string(fileId) + ".txt";
        ofstream outFile(fileName);
        if (!outFile.is_open()) {
            cerr << "Erro ao criar o arquivo de saída: " << fileName << endl;
            return 1;
        }

        // Escrever o tamanho da matriz total no arquivo
        outFile << n1 << " " << m2 << endl;

        // Vetor para armazenar os resultados parciais
        vector<vector<int>> result(n1, vector<int>(m2, 0));

        // Executar a multiplicação de matrizes utilizando processos
        auto start = high_resolution_clock::now();
        for (int i = 0; i < P; ++i) {
            pid_t pid = fork();
            if (pid == 0) { // Processo filho
                auto processStart = high_resolution_clock::now(); // Registrar o início do cálculo
                int startRow = i * chunkSize;
                int endRow = (i == P - 1) ? n1 : (i + 1) * chunkSize;
                multiplyPart(matrix1, matrix2, result, startRow, endRow);
                auto processEnd = high_resolution_clock::now(); // Registrar o fim do cálculo
                processTimes[i] = duration_cast<milliseconds>(processEnd - processStart);
                // Escrever o resultado no arquivo
                for (int row = startRow; row < endRow; ++row) {
                    for (int col = 0; col < m2; ++col) {
                        outFile << result[row][col] << " ";
                    }
                    outFile << endl;
                }
                exit(0);
            } else if (pid < 0) { // Erro na criação do processo
                cerr << "Erro ao criar processo filho." << endl;
                return 1;
            }
        }

        // Aguardar todos os processos filhos terminarem
        for (int i = 0; i < P; ++i) {
            wait(NULL);
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        processTimes[fileId] = duration;
    }

    auto stopTotal = high_resolution_clock::now();
    auto durationTotal = duration_cast<milliseconds>(stopTotal - startTotal);

    // Imprimir os tempos de execução dos processos com 4 casas decimais
    // cout << "Tempo total de execução (milissegundos): " << durationTotal.count() << endl;
    // cout << "Tempos de execução dos processos (milissegundos):" << endl;
    // cout << fixed << setprecision(4); // Definir a precisão para 4 casas decimais
    // for (int i = 0; i < P; ++i) {
    //     cout << "Processo " << i << ": " << processTimes[i].count() << endl;
    // }

    cout << endl;
    cout << "Multiplicação de matrizes concluída com sucesso." << endl;

    return 0;
}
