#include "entidades.hpp"
#include <stdexcept>


//Verifica se a pessoa tem permissao (gerente ou desenvolvedor) pra realizar alguma acao
void CartaoDeAtividade::verificarPermissao(const Papel& papel,
                                            const vector<string>& permitidos) {
    for (const string& p : permitidos) {
        if (papel.getValor() == p) {
            return;
        }
    }
    throw logic_error("Papel sem permissao para esta operacao.");
}

CartaoDeAtividade::CartaoDeAtividade(const Papel& papel, const Identificador& id, const Nome& nome,
                                      const Texto& descricao, const Prioridade& prioridade,
                                      const Tamanho& tamanho, const Timestamp& entrada)
    : id(id), nome(nome), descricao(descricao), prioridade(prioridade),
      estado(Estado::PENDENTE), tamanho(tamanho), entrada(entrada) {
    verificarPermissao(papel, {Papel::GESTOR});
}

void CartaoDeAtividade::setNome(const Papel& papel, const Nome& novoNome) {
    verificarPermissao(papel, {Papel::GESTOR});
    nome = novoNome;
}

void CartaoDeAtividade::setDescricao(const Papel& papel, const Texto& novaDescricao) {
    verificarPermissao(papel, {Papel::GESTOR});
    descricao = novaDescricao;
}

void CartaoDeAtividade::setPrioridade(const Papel& papel, const Prioridade& novaPrioridade) {
    verificarPermissao(papel, {Papel::GESTOR});
    prioridade = novaPrioridade;
}

void CartaoDeAtividade::setTamanho(const Papel& papel, const Tamanho& novoTamanho) {
    verificarPermissao(papel, {Papel::GESTOR});
    tamanho = novoTamanho;
}

void CartaoDeAtividade::mover(const Papel& papel, const Timestamp& momento) {
    verificarPermissao(papel, {Papel::GESTOR, Papel::DESENVOLVEDOR});
    if (estado.getValor() == Estado::PENDENTE) {
        estado.setValor(Estado::FAZENDO);
        inicio = momento;
    } else if (estado.getValor() == Estado::FAZENDO) {
        estado.setValor(Estado::CONCLUIDA);
        termino = momento;
    } else {
        throw logic_error("Cartao ja concluido.");
    }
}

float CartaoDeAtividade::getTempoDeCiclo(const Papel& papel) const {
    verificarPermissao(papel, {Papel::GESTOR, Papel::DESENVOLVEDOR});
    return static_cast<float>(termino.paraMinutos() - inicio.paraMinutos());
}

float CartaoDeAtividade::getLeadTime(const Papel& papel) const {
    verificarPermissao(papel, {Papel::GESTOR, Papel::DESENVOLVEDOR});
    return static_cast<float>(termino.paraMinutos() - entrada.paraMinutos());
}