#ifndef ENTIDADES_HPP_INCLUDED
#define ENTIDADES_HPP_INCLUDED

#include "dominios.hpp"
#include <vector>

using namespace std;

class CartaoDeAtividade {
private:
    Identificador id;
    Nome nome;
    Texto descricao;
    Prioridade prioridade;
    Estado estado;
    Tamanho tamanho;
    Timestamp entrada;
    Timestamp inicio;
    Timestamp termino;

    static void verificarPermissao(const Papel& papel, const std::vector<std::string>& permitidos);

public:
    CartaoDeAtividade(const Papel& papel, const Identificador& id, const Nome& nome,
                       const Texto& descricao, const Prioridade& prioridade,
                       const Tamanho& tamanho, const Timestamp& entrada);

    Identificador getIdentificador() const { return id; }
    Nome getNome() const { return nome; }
    Texto getDescricao() const { return descricao; }
    Prioridade getPrioridade() const { return prioridade; }
    Estado getEstado() const { return estado; }
    Tamanho getTamanho() const { return tamanho; }
    Timestamp getEntrada() const { return entrada; }
    Timestamp getInicio() const { return inicio; }
    Timestamp getTermino() const { return termino; }

    void setNome(const Papel& papel, const Nome& novoNome);
    void setDescricao(const Papel& papel, const Texto& novaDescricao);
    void setPrioridade(const Papel& papel, const Prioridade& novaPrioridade);
    void setTamanho(const Papel& papel, const Tamanho& novoTamanho);

    void mover(const Papel& papel, const Timestamp& momento);

    float getTempoDeCiclo(const Papel& papel) const;
    float getLeadTime(const Papel& papel) const;
};

#endif