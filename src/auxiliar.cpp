#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Função para gerar e salvar matriz aleatória em um arquivo
void generateAndSaveMatrix(int n, int m, const string& filename) {
    // Abrir o arquivo para escrita
    ofstream outFile(filename);

    // Verificar se o arquivo foi aberto corretamente
    if (!outFile.is_open()) {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        exit(1);
    }

    // Escrever as dimensões da matriz no arquivo
    outFile << n << " " << m << endl;

    // Gerar a matriz aleatória
    srand(time(nullptr));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            // Gerar um número aleatório entre 0 e 100
            int randomValue = rand() % 101; // Valor entre 0 e 100
            outFile << randomValue << " ";
        }
        outFile << endl;
    }

    // Fechar o arquivo
    outFile.close();
}

int main(int argc, char* argv[]) {
    // Verificar se o número de argumentos é válido
    if (argc != 5) {
        cerr << "Uso: " << argv[0] << " n1 m1 n2 m2" << endl;
        return 1;
    }

    // Obter as dimensões das matrizes a serem geradas
    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    // Gerar e salvar as matrizes em arquivos separados
    generateAndSaveMatrix(n1, m1, "matrix1.txt");
    generateAndSaveMatrix(n2, m2, "matrix2.txt");

    cout << "Matrizes geradas com sucesso e salvas nos arquivos matrix1.txt e matrix2.txt." << endl;

    return 0;
}
