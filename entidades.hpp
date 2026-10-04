#ifndef ENTIDADES_HPP_INCLUDED
#define ENTIDADES_HPP_INCLUDED

#include "dominios.hpp"

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

public:
    CartaoDeAtividade(const Identificador& id, const Nome& nome,
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

    void setNome(const Nome& novoNome);
    void setDescricao(const Texto& novaDescricao);
    void setPrioridade(const Prioridade& novaPrioridade);
    void setTamanho(const Tamanho& novoTamanho);

    void mover(const Timestamp& momento);

    float getTempoDeCiclo() const;
    float getLeadTime() const;
};

#endif