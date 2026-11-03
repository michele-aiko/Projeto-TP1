#ifndef ENTIDADES_HPP_INCLUDED
#define ENTIDADES_HPP_INCLUDED

#include "dominios.hpp"

///
/// @brief Representa a entidade Pessoa (Usuário) no sistema Kanban.
/// @details Modela um participante cadastrado no sistema, podendo assumir o papel de
///          GESTOR ou DESENVOLVEDOR. Seus atributos são estritamente instâncias de classes de domínio.
///
class Pessoa {
private:
    Email email;   ///< Instância do domínio Email (Identificador de Login / Chave Primária).
    Nome nome;     ///< Instância do domínio Nome (Nome completo do participante).
    Senha senha;   ///< Instância do domínio Senha (Senha alfanumérica de 5 caracteres).
    Papel papel;   ///< Instância do domínio Papel ("GESTOR" ou "DESENVOLVEDOR").

public:
    ///
    /// @brief Construtor padrão da entidade Pessoa
    /// @details Inicializa a pessoa utilizando os construtores padrão dos próprios domínios.
    ///
    Pessoa() = default;

    ///
    /// @brief Construtor parametrizado completo para a entidade Pessoa.
    /// @details Permite instanciar uma pessoa completa com todos os domínios previamente validados.
    /// @param email Objeto da classe Email.
    /// @param nome Objeto da classe Nome.
    /// @param senha Objeto da classe Senha.
    /// @param papel Objeto da classe Papel.
    ///
    Pessoa(const Email& email, const Nome& nome, const Senha& senha, const Papel& papel);

    ///
    /// @brief Atribui o e-mail da pessoa
    /// @param email Objeto da classe de domínio Email.
    ///
    void setEmail(const Email& email) { this->email = email; }

    ///
    /// @brief Obtém o e-mail da pessoa 
    /// @return Objeto da classe de domínio Email.
    ///
    Email getEmail() const { return email; }

    ///
    /// @brief Atribui o nome da pessoa 
    /// @param nome Objeto da classe de domínio Nome.
    ///
    void setNome(const Nome& nome) { this->nome = nome; }

    ///
    /// @brief Obtém o nome da pessoa
    /// @return Objeto da classe de domínio Nome.
    ///
    Nome getNome() const { return nome; }

    ///
    /// @brief Atribui a senha da pessoa
    /// @param senha Objeto da classe de domínio Senha.
    ///
    void setSenha(const Senha& senha) { this->senha = senha; }

    ///
    /// @brief Obtém a senha da pessoa
    /// @return Objeto da classe de domínio Senha.
    ///
    Senha getSenha() const { return senha; }

    ///
    /// @brief Atribui o papel funcional da pessoa .
    /// @param papel Objeto da classe de domínio Papel.
    ///
    void setPapel(const Papel& papel) { this->papel = papel; }

    ///
    /// @brief Obtém o papel funcional da pessoa
    /// @return Objeto da classe de domínio Papel.
    ///
    Papel getPapel() const { return papel; }
};

///
/// @brief Representa a entidade Projeto no sistema Kanban.
/// @details Modela um projeto gerenciado pelo Gestor, contendo identificador único,
///          nome, descrição textual e as marcações temporais de início e término.
///
class Projeto {
private:
    Identificador identificador; ///< Instância do domínio Identificador (Chave Primária do Projeto).
    Nome nome;                   ///< Instância do domínio Nome (Denominação do projeto).
    Texto descricao;             ///< Instância do domínio Texto (Descrição resumida de até 30 caracteres).
    Timestamp inicio;            ///< Instância do domínio Timestamp (Data/hora de início previsto/real).
    Timestamp termino;           ///< Instância do domínio Timestamp (Data/hora de término previsto/real).

public:
    ///
    /// @brief Construtor padrão da entidade Projeto
    ///
    Projeto() = default;

    ///
    /// @brief Construtor parametrizado completo da entidade Projeto.
    /// @param identificador Objeto da classe Identificador.
    /// @param nome Objeto da classe Nome.
    /// @param descricao Objeto da classe Texto.
    /// @param inicio Objeto da classe Timestamp (Início).
    /// @param termino Objeto da classe Timestamp (Término).
    ///
    Projeto(const Identificador& identificador, const Nome& nome, const Texto& descricao,
            const Timestamp& inicio, const Timestamp& termino);

    void setIdentificador(const Identificador& identificador) { this->identificador = identificador; }
    Identificador getIdentificador() const { return identificador; }

    void setNome(const Nome& nome) { this->nome = nome; }
    Nome getNome() const { return nome; }

    void setDescricao(const Texto& descricao) { this->descricao = descricao; }
    Texto getDescricao() const { return descricao; }

    void setInicio(const Timestamp& inicio) { this->inicio = inicio; }
    Timestamp getInicio() const { return inicio; }

    void setTermino(const Timestamp& termino) { this->termino = termino; }
    Timestamp getTermino() const { return termino; }
};

///
/// @brief Representa a entidade Quadro Kanban.
/// @details Modela o painel visual de cartões, associado a um projeto, contendo identificador,
///          nome e o limite máximo de trabalho em progresso (WIP) para a coluna FAZENDO.
///
class Quadro {
private:
    Identificador identificador; ///< Instância do domínio Identificador (Chave Primária do Quadro).
    Nome nome;                   ///< Instância do domínio Nome (Título do quadro).
    Limite limite;               ///< Instância do domínio Limite (Máximo de cartões em FAZENDO: 1 a 25).

public:
    ///
    /// @brief Construtor padrão da entidade Quadro
    ///
    Quadro() = default;

    ///
    /// @brief Construtor parametrizado da entidade Quadro.
    /// @param identificador Objeto da classe Identificador.
    /// @param nome Objeto da classe Nome.
    /// @param limite Objeto da classe Limite.
    ///
    Quadro(const Identificador& identificador, const Nome& nome, const Limite& limite);

    void setIdentificador(const Identificador& identificador) { this->identificador = identificador; }
    Identificador getIdentificador() const { return identificador; }

    void setNome(const Nome& nome) { this->nome = nome; }
    Nome getNome() const { return nome; }

    void setLimite(const Limite& limite) { this->limite = limite; }
    Limite getLimite() const { return limite; }
};

///
/// @brief Representa a entidade Cartão de Atividade no quadro Kanban.
/// @details Modela uma tarefa individual no fluxo do projeto, rastreando seu estado,
///          prioridade, tamanho e os timestamps de ciclo para cálculo de Lead Time e Cycle Time.
///
class CartaoDeAtividade {
private:
    Identificador identificador; ///< Instância do domínio Identificador (Chave Primária do Cartão).
    Nome nome;                   ///< Instância do domínio Nome (Título da tarefa).
    Texto descricao;             ///< Instância do domínio Texto (Descrição da atividade).
    Prioridade prioridade;       ///< Instância do domínio Prioridade ("ALTA", "MEDIA", "BAIXA").
    Estado estado;               ///< Instância do domínio Estado ("A FAZER", "FAZENDO", "FEITO").
    Tamanho tamanho;             ///< Instância do domínio Tamanho ("PEQUENO", "MEDIO", "GRANDE").
    Timestamp entrada;           ///< Instância do domínio Timestamp (Entrada em "A FAZER").
    Timestamp inicio;            ///< Instância do domínio Timestamp (Transição para "FAZENDO").
    Timestamp termino;           ///< Instância do domínio Timestamp (Conclusão em "FEITO").

public:
    ///
    /// @brief Construtor padrão da entidade CartaoDeAtividade
    ///
    CartaoDeAtividade() = default;

    ///
    /// @brief Construtor parametrizado completo da entidade Cartão de Atividade.
    /// @param identificador Objeto da classe Identificador.
    /// @param nome Objeto da classe Nome.
    /// @param descricao Objeto da classe Texto.
    /// @param prioridade Objeto da classe Prioridade.
    /// @param estado Objeto da classe Estado.
    /// @param tamanho Objeto da classe Tamanho.
    /// @param entrada Objeto da classe Timestamp (Criação).
    /// @param inicio Objeto da classe Timestamp (Início de execução).
    /// @param termino Objeto da classe Timestamp (Finalização).
    ///
    CartaoDeAtividade(const Identificador& identificador, const Nome& nome, const Texto& descricao,
                      const Prioridade& prioridade, const Estado& estado, const Tamanho& tamanho,
                      const Timestamp& entrada, const Timestamp& inicio, const Timestamp& termino);

    void setIdentificador(const Identificador& identificador) { this->identificador = identificador; }
    Identificador getIdentificador() const { return identificador; }

    void setNome(const Nome& nome) { this->nome = nome; }
    Nome getNome() const { return nome; }

    void setDescricao(const Texto& descricao) { this->descricao = descricao; }
    Texto getDescricao() const { return descricao; }

    void setPrioridade(const Prioridade& prioridade) { this->prioridade = prioridade; }
    Prioridade getPrioridade() const { return prioridade; }

    void setEstado(const Estado& estado) { this->estado = estado; }
    Estado getEstado() const { return estado; }

    void setTamanho(const Tamanho& tamanho) { this->tamanho = tamanho; }
    Tamanho getTamanho() const { return tamanho; }

    void setEntrada(const Timestamp& entrada) { this->entrada = entrada; }
    Timestamp getEntrada() const { return entrada; }

    void setInicio(const Timestamp& inicio) { this->inicio = inicio; }
    Timestamp getInicio() const { return inicio; }

    void setTermino(const Timestamp& termino) { this->termino = termino; }
    Timestamp getTermino() const { return termino; }
};

#endif // ENTIDADES_HPP_INCLUDED
