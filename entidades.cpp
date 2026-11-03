#include "entidades.hpp"

// Construtor completo da entidade Pessoa
Pessoa::Pessoa(const Email& email, const Nome& nome, const Senha& senha, const Papel& papel)
    : email(email), nome(nome), senha(senha), papel(papel) {
    // Todos os atributos de domínio são inicializados diretamente antes da abertura deste corpo
}

// Construtor completo da entidade Projeto
Projeto::Projeto(const Identificador& identificador, const Nome& nome, const Texto& descricao,
                 const Timestamp& inicio, const Timestamp& termino)
    : identificador(identificador), nome(nome), descricao(descricao),
      inicio(inicio), termino(termino) {
}

// Construtor completo da entidade Quadro
Quadro::Quadro(const Identificador& identificador, const Nome& nome, const Limite& limite)
    : identificador(identificador), nome(nome), limite(limite) {
}

// Construtor completo da entidade CartaoDeAtividade
CartaoDeAtividade::CartaoDeAtividade(const Identificador& identificador, const Nome& nome,
                                     const Texto& descricao, const Prioridade& prioridade,
                                     const Estado& estado, const Tamanho& tamanho,
                                     const Timestamp& entrada, const Timestamp& inicio,
                                     const Timestamp& termino)
    : identificador(identificador), nome(nome), descricao(descricao),
      prioridade(prioridade), estado(estado), tamanho(tamanho),
      entrada(entrada), inicio(inicio), termino(termino) {
}
