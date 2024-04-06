#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Função para multiplicar matrizes de forma sequencial
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& matrix1, const vector<vector<int>>& matrix2) {
    int n1 = matrix1.size();
    int m1 = matrix1[0].size();
    int n2 = matrix2.size();
    int m2 = matrix2[0].size();

    if (m1 != n2) {
        cerr << "Número de colunas da primeira matriz não é igual ao número de linhas da segunda matriz." << endl;
        exit(1);
    }

    vector<vector<int>> result(n1, vector<int>(m2, 0));

    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            for (int k = 0; k < m1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    // Verificar se o número de argumentos é válido
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " matrix1.txt matrix2.txt" << endl;
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

    // Fechar os arquivos/
    inFile1.close();
    inFile2.close();

    // Executar a multiplicação de matrizes
    auto start = high_resolution_clock::now();
    vector<vector<int>> result = multiplyMatrices(matrix1, matrix2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    // Salvar o resultado em um arquivo
    ofstream outFile("result_sequential.txt");
    if (!outFile.is_open()) {
        cerr << "Erro ao criar o arquivo de saída." << endl;
        return 1;
    }
    outFile << "Tempo de execução (milissegundos): " << duration.count() << endl;
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[0].size(); ++j) {
            outFile << result[i][j] << " ";
        }
        outFile << endl;
    }
    outFile.close();

    cout << "Multiplicação de matrizes concluída com sucesso. Resultado salvo em result_sequential.txt." << endl;

    return 0;
}
