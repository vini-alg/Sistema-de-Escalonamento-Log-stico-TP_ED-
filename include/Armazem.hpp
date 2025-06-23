#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Pilha.hpp"
#include "Pacote.hpp"

/**
 * @class Armazem
 * @brief Representa uma instalação de armazenamento para pacotes.
 *
 * Um armazém contém múltiplas seções, cada uma sendo uma pilha de pacotes
 * destinada a um armazém de destino específico.
 */
class Armazem {
private:
    int id; /// @brief Identificador único do armazém.
    
    // Cada índice do array corresponde a uma seção para um armazém adjacente.
    Pilha<Pacote*>* secoes; /// @brief Array de pilhas, onde cada pilha representa uma seção de pacotes.
    
    int num_secoes; // igual ao número total de armazéns na simulação

public:
    /**
     * @brief Constrói um novo objeto Armazem.
     *
     * @param id O identificador para o armazém.
     * @param total_armazens O número total de armazéns na simulação, usado para alocar as seções.
     */
    Armazem(int id, int total_armazens);

    /**
     * @brief Destrói o objeto Armazem, liberando os recursos alocados.
     */
    ~Armazem();

    /**
     * @brief Adiciona um pacote à seção de armazenamento apropriada.
     *
     * O pacote é colocado na pilha correspondente ao seu próximo destino na rota.
     * @param pacote O pacote a ser armazenado.
     */
    void armazenar_pacote(Pacote* pacote);
    
    /**
     * @brief Recupera a seção de empilhamento para um armazém de destino específico.
     *
     * @param id_armazem_destino O ID do armazém de destino.
     * @return Uma referência para a pilha de pacotes da seção solicitada.
     */
    Pilha<Pacote*>& obter_secao(int id_armazem_destino);
};

#endif // ARMAZEM_HPP
