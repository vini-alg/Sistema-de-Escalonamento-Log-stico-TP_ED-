#include <iostream>
#include <fstream>
#include "../include/TAD.hpp"

int main(int argc, char* argv[]) {
    try {

        std::ifstream arquivo(argv[1]);
        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir o arquivo.\n";
        return 1;
        }
        
        return 0;
    }
    catch (...) {
        std::cout << "Erro inesperado durante a execução" << std::endl;
        return 99;
    }
}
