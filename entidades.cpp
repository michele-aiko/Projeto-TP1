#include "entidades.hpp"
#include <stdexcept>

CartaoDeAtividade::CartaoDeAtividade(const Identificador& id, const Nome& nome,
                                      const Texto& descricao, const Prioridade& prioridade,
                                      const Tamanho& tamanho, const Timestamp& entrada)
    : id(id), nome(nome), descricao(descricao), prioridade(prioridade),
      estado(Estado::PENDENTE), tamanho(tamanho), entrada(entrada) {}



void CartaoDeAtividade::mover(const Timestamp& momento) {
    if (estado.getValor() == Estado::PENDENTE) {
        estado.setValor(Estado::FAZENDO);
        inicio = momento;
    } else if (estado.getValor() == Estado::FAZENDO) {
        estado.setValor(Estado::CONCLUIDA);
        termino = momento;
    } else {
        throw std::logic_error("Cartao ja concluido.");
    }
}

float CartaoDeAtividade::getTempoDeCiclo() const {
    return static_cast<float>(termino.paraMinutos() - inicio.paraMinutos());
}

float CartaoDeAtividade::getLeadTime() const {
    return static_cast<float>(termino.paraMinutos() - entrada.paraMinutos());
}