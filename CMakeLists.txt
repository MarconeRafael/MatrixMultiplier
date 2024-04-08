cmake_minimum_required(VERSION 3.10)
project(matrix_multiplication)

# Configuração para usar C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Adiciona os executáveis dos programas auxiliar, sequencial, paralelo com threads e paralelo com processos
add_executable(auxiliar src/auxiliar.cpp)
add_executable(sequential src/sequential.cpp)
add_executable(parallel_threads src/parallel_threads.cpp)
add_executable(parallel_processes src/parallel_processes.cpp)
add_executable(etapa2 src/etapa2.cpp)
# Define o diretório de saída para os executáveis
set_target_properties(
    auxiliar sequential parallel_threads parallel_processes etapa2
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Adiciona o diretório de inclusão para os cabeçalhos
target_include_directories(etapa2 PUBLIC include)
target_include_directories(auxiliar PUBLIC include)
target_include_directories(sequential PUBLIC include)
target_include_directories(parallel_threads PUBLIC include)
target_include_directories(parallel_processes PUBLIC include)

# Se necessário, linka a biblioteca pthread ao executável paralelo com threads
target_link_libraries(parallel_threads PRIVATE pthread)
 # Adicione os alvos personalizados para executar os programas após a compilação
#add_custom_target(run_auxiliar COMMAND ./auxiliar int X int X int X int X)
#add_custom_target(run_sequential COMMAND ./sequential matrix1.txt matrix2.txt)
#add_custom_target(run_parallel_threads COMMAND ./parallel_threads matrix1.txt matrix2.txt int P)
#add_custom_target(run_parallel_processes COMMAND ./parallel_processes matrix1.txt matrix2.txt int P)

# Adicione todos os programas como dependências do alvo "run_all"
add_custom_target(run_all DEPENDS run_auxiliar run_sequential run_parallel_threads run_parallel_processes)


