#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

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

    // Obter o número de threads a serem usadas
    int P = atoi(argv[3]);

    // Calcular a quantidade de linhas por thread
    int chunkSize = n1 / P;

    // Vetor para armazenar os identificadores das threads
    vector<thread> threads;

    // Matriz para armazenar o resultado
    vector<vector<int>> result(n1, vector<int>(m2, 0));

    // Vetor para armazenar os tempos de execução das threads
    vector<milliseconds> threadTimes(P);

    // Mutex para proteger a variável de resultado
    mutex resultMutex;

    // Executar a multiplicação de matrizes utilizando threads
    auto start = high_resolution_clock::now();
    for (int i = 0; i < P; ++i) {
        int startRow = i * chunkSize;
        int endRow = (i == P - 1) ? n1 : (i + 1) * chunkSize;
        threads.emplace_back([&, startRow, endRow, i]() {
            auto threadStart = high_resolution_clock::now();
            multiplyPart(matrix1, matrix2, result, startRow, endRow);
            auto threadEnd = high_resolution_clock::now();
            threadTimes[i] = duration_cast<milliseconds>(threadEnd - threadStart);
        });
    }

    // Aguardar todas as threads terminarem
    for (auto& t : threads) {
        t.join();
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Salvar o resultado em um arquivo
    ofstream outFile("result_parallel_threads.txt");
    if (!outFile.is_open()) {
        cerr << "Erro ao criar o arquivo de saída." << endl;
        return 1;
    }
    outFile << "Tempo total de execução (milissegundos): " << duration.count() << endl;
    outFile << "Tempos de execução das threads (milissegundos):" << endl;
    for (int i = 0; i < P; ++i) {
        outFile << "Thread " << i << ": " << threadTimes[i].count() << endl;
    }
    outFile << endl;
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[0].size(); ++j) {
            outFile << result[i][j] << " ";
        }
        outFile << endl;
    }
    outFile.close();

    cout << "Multiplicação de matrizes concluída com sucesso. Resultado salvo em result_parallel_threads.txt." << endl;

    return 0;
}
