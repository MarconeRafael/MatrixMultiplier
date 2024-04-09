# Multiplicação de Matrizes

Este projeto implementa a multiplicação de matrizes em diferentes cenários e faz comparações de desempenho entre implementações sequenciais e paralelas.

## Estrutura de Pastas

- `docs`: Documentação do projeto.
- `include`: Cabeçalhos necessários.
- `src`: Códigos-fonte dos programas.
- `tests`: Testes e dados de entrada.





├── author.md                      
├── CMakeLists.txt                 
├── docs                           
│   └── lista_template.pdf         
├── include                        
│   └── matrix_operations.h        
├── src                           
│   ├── auxiliar.cpp               
│   ├── sequential.cpp             
│   ├── parallel_threads.cpp      
│   └── parallel_processes.cpp     


## Como Compilar

O projeto utiliza CMake para configuração e compilação. Certifique-se de ter o CMake instalado em seu sistema. Para compilar, siga estes passos:

Observação: o arquivo build não pode ser anexado por completo ao github devido seu tamnho ultrapassar o limide definido pela plataforma

1. Crie um diretório de compilação: `mkdir build && cd build`
2. Configure o build com CMake: `cmake ..`
3. Compile o projeto: `cmake --build .`

## Cenários de Implementação

O projeto aborda três cenários principais para a multiplicação de matrizes:

1. **Implementação Sequencial**: Neste cenário, as operações de multiplicação são executadas em uma única thread, de forma sequencial, sem paralelismo. Isso serve como base de comparação para as implementações paralelas.

2. **Implementação Paralela com Threads**: Utiliza threads para paralelizar a multiplicação de matrizes, permitindo que várias operações sejam executadas simultaneamente em diferentes threads. Isso aumenta o desempenho, especialmente em sistemas multi-core.

3. **Implementação Paralela com Processos**: Usa processos para paralelizar a multiplicação de matrizes. Cada processo é uma instância separada do programa em execução, permitindo um paralelismo robusto e independente entre os processos.

Cada cenário oferece vantagens e desvantagens em termos de desempenho e complexidade. O objetivo do projeto é explorar esses cenários e comparar seus tempos de execução para diferentes tamanhos de matrizes, ajudando a entender melhor as características e trade-offs de cada abordagem.

## Autor

- [MarconeRafael](https://github.com/marconerafael): Desenvolvedor principal

## Licença

Este projeto está licenciado sob a [Licença MIT](LICENSE).
