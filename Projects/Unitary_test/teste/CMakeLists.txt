cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(teste 
teste.c
../include/unity.c
../Teste_unitario.c)

target_include_directories(teste PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../include
)