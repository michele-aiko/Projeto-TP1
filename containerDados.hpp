#ifndef CONTAINERDADOS_HPP_INCLUDED
#define CONTAINERDADOS_HPP_INCLUDED

#include "dominios.hpp"
#include "entidades.hpp"
#include <map>
#include <vector>
#include <string>

///
/// @brief Contêiner de dados centralizado em memória para persistência de dados sobre entidades e seus relacionamentos.
/// @details Implementa o armazenamento das quatro entidades do sistema (Pessoa, Projeto, Quadro, CartaoDeAtividade)
///          utilizando tabelas hash/árvores balanceadas (std::map) indexadas pelas respectivas chaves primárias textuais.
///          Garante o suporte a todas as operações CRUD e consultas relacionais exigidas pelos serviços.
///
class ContainerDados {
private:
    // Instância única para padrão Singleton
    static ContainerDados* instancia;

    // Construtor privado para evitar instanciação direta fora do Singleton
    ContainerDados() = default;

    // Tabelas de Dados (Chave Primária -> Entidade)
    std::map<std::string, Pessoa> tabelaPessoas;                     ///< Chave: Email (string)
    std::map<std::string, Projeto> tabelaProjetos;                   ///< Chave: Identificador (string)
    std::map<std::string, Quadro> tabelaQuadros;                     ///< Chave: Identificador (string)
    std::map<std::string, CartaoDeAtividade> tabelaCartoes;           ///< Chave: Identificador (string)

    // Mapeamentos Relacionais de Chave Estrangeira
    std::map<std::string, std::string> vinculoProjetoGestor;         ///< ID Projeto -> Email Gestor
    std::map<std::string, std::string> vinculoQuadroProjeto;         ///< ID Quadro -> ID Projeto
    std::map<std::string, std::string> vinculoCartaoQuadro;          ///< ID Cartão -> ID Quadro

public:
    // Destrutor padrão
    ~ContainerDados() = default;

    // Métodos que irão excluir qualquer tentativa de cópia e atribuição direta do contêiner para preservar a unicidade do Singleton
    ContainerDados(const ContainerDados&) = delete;
    ContainerDados& operator=(const ContainerDados&) = delete;

    ///
    /// @brief Obtém o ponteiro da instância única do contêiner de dados.
    /// @return Ponteiro para a instância única de ContainerDados.
    ///
    static ContainerDados* getInstancia();

    // =========================================================================
    // OPERAÇÕES DE PESSOA
    // =========================================================================

    ///
    /// @brief Insere uma nova pessoa no contêiner.
    // CONFIRMAR POSSIBILIDADE DE UTILIZAR O EMAIL COMO PARÂMETRO PARA CRIAÇÃO DA PESSOA
    /// @param pessoa Objeto da classe Pessoa devidamente preenchido.
    /// @return true se cadastrada com sucesso; false caso o e-mail já exista.
    ///
    bool inserirPessoa(const Pessoa& pessoa);

    ///
    /// @brief Busca uma pessoa pelo e-mail (chave primária).
    // CONFIRMAR NECESSIDADE DE ADICIONAR TAMBÉM O PARAMETRO PESSOA
    /// @param email Objeto Email a ser consultado.
    /// @param pessoa Referência para armazenar o objeto localizado.
    /// @return true se encontrada; false caso contrário.
    ///
    bool buscarPessoa(const Email& email) const;

    ///
    /// @brief Atualiza os dados de uma pessoa já existente.
    // CONFIRMAR POSSIBILIDADE DE UTILIZAR O EMAIL COMO PARÂMETRO PARA ATUALIZAÇÃO DA PESSOA
    /// @param pessoa Objeto Pessoa contendo o e-mail chave e os dados alterados.
    /// @return true se atualizada com sucesso; false se o e-mail não existir.
    ///
    bool atualizarPessoa(const Pessoa& pessoa);

    ///
    /// @brief Remove o registro de uma pessoa pelo e-mail.
    /// @param email Objeto Email da conta a ser excluída.
    /// @return true se removida com sucesso; false se inexistente.
    ///
    bool removerPessoa(const Email& email);

    ///
    /// @brief Retorna a listagem completa de todas as pessoas cadastradas.
    /// @return Vetor contendo todas as instâncias de Pessoa.
    ///
    std::vector<Pessoa> listarPessoas() const;

    // =========================================================================
    // OPERAÇÕES DE PROJETO
    // =========================================================================

    ///
    /// @brief Insere um novo projeto no containêr e associa ao Gestor criador.
    // CONFIRMAR NECESSIDADE DE ADICIONAR TAMBÉM O PARÂMETRO EMAIL DO GESTOR
    /// @param projeto Objeto Projeto a ser salvo.
    /// @param emailGestor Objeto Email do gestor responsável pelo projeto.
    /// @return true se inserido com sucesso; false se o identificador já existir.
    ///
    bool inserirProjeto(const Projeto& projeto);

    ///
    /// @brief Busca um projeto pelo seu código identificador.
    // CONFIRMAR NECESSIDADE DE ADICIONAR TAMBÉM O PARÂMETRO PROJETO
    /// @param id Objeto Identificador do projeto.
    /// @param projeto Referência para armazenar o objeto localizado.
    /// @return true se encontrado; false caso contrário.
    ///
    bool buscarProjeto(const Identificador& id) const;

    ///
    /// @brief Atualiza as informações de um projeto existente.
    /// @param projeto Objeto Projeto com os dados modificados.
    /// @return true se atualizado; false se o projeto não for encontrado.
    ///
    bool atualizarProjeto(const Projeto& projeto);

    ///
    /// @brief Remove um projeto e todos os seus vínculos associados.
    /// @param id Objeto Identificador do projeto a ser excluído.
    /// @return true se removido com sucesso; false se inexistente.
    ///
    bool removerProjeto(const Identificador& id);

    ///
    /// @brief Lista todos os projetos associados a um determinado gestor.
    /// @param emailGestor Objeto Email do gestor pesquisado.
    /// @return Vetor contendo os projetos do gestor.
    ///
    std::vector<Projeto> listarProjetosPorGestor(const Email& emailGestor) const;

    ///
    /// @brief Lista todos os projetos cadastrados no sistema.
    /// @return Vetor com todos os projetos existentes.
    ///
    std::vector<Projeto> listarTodosProjetos() const;

    // =========================================================================
    // OPERAÇÕES DE QUADRO
    // =========================================================================

    ///
    /// @brief Insere um novo quadro associado a um projeto.
    /// @param quadro Objeto Quadro a ser salvo.
    /// @param idProjeto Objeto Identificador do projeto proprietário.
    /// @return true se inserido com sucesso; false se o identificador já existir.
    ///
    bool inserirQuadro(const Quadro& quadro, const Identificador& idProjeto);

    ///
    /// @brief Busca um quadro pelo seu código identificador.
    // CONFIRMAR NECESSIDADE DE ADICIONAR TAMBÉM O PARÂMETRO QUADRO
    /// @param id Objeto Identificador do quadro.
    /// @param quadro Referência para armazenar o objeto localizado.
    /// @return true se encontrado; false caso contrário.
    ///
    bool buscarQuadro(const Identificador& id) const;

    ///
    /// @brief Atualiza as informações de um quadro (como o limite de WIP ou nome).
    /// @param quadro Objeto Quadro atualizado.
    /// @return true se atualizado; false se o quadro não for encontrado.
    ///
    bool atualizarQuadro(const Quadro& quadro);

    ///
    /// @brief Remove um quadro pelo identificador.
    /// @param id Objeto Identificador do quadro a ser excluído.
    /// @return true se removido com sucesso; false caso contrário.
    ///
    bool removerQuadro(const Identificador& id);

    ///
    /// @brief Lista todos os quadros vinculados a um projeto específico.
    /// @param idProjeto Objeto Identificador do projeto.
    /// @return Vetor contendo os quadros do projeto.
    ///
    std::vector<Quadro> listarQuadrosPorProjeto(const Identificador& idProjeto) const;

    // =========================================================================
    // OPERAÇÕES DE CARTÃO DE ATIVIDADE
    // =========================================================================

    ///
    /// @brief Insere um novo cartão de atividade vinculado a um determinado quadro.
    /// @param cartao Objeto CartaoDeAtividade a ser salvo.
    /// @param idQuadro Objeto Identificador do quadro de destino.
    /// @return true se inserido com sucesso; false se o ID já existir.
    ///
    bool inserirCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro);

    ///
    /// @brief Busca um cartão pelo seu identificador único.
    // CONFIRMAR NECESSIDADE DE ADICIONAR TAMBÉM O PARÂMETRO CARTÃO 
    /// @param id Objeto Identificador do cartão.
    /// @param cartao Referência para armazenar o objeto localizado.
    /// @return true se encontrado; false caso contrário.
    ///
    bool buscarCartao(const Identificador& id) const;

    ///
    /// @brief Atualiza os dados de um cartão de atividade.
    /// @param cartao Objeto CartaoDeAtividade contendo as alterações.
    /// @return true se atualizado com sucesso; false se não for localizado.
    ///
    bool atualizarCartao(const CartaoDeAtividade& cartao);

    ///
    /// @brief Remove um cartão pelo identificador.
    /// @param id Objeto Identificador do cartão a ser excluído.
    /// @return true se removido com sucesso; false caso contrário.
    ///
    bool removerCartao(const Identificador& id);

    ///
    /// @brief Lista todos os cartões vinculados a um determinado quadro.
    /// @param idQuadro Objeto Identificador do quadro.
    /// @return Vetor com todos os cartões cadastrados naquele quadro.
    ///
    std::vector<CartaoDeAtividade> listarCartoesPorQuadro(const Identificador& idQuadro) const;

    ///
    /// @brief Lista todos os cartões vinculados a uma determinada pessoa, independente de estarem no mesmo projeto.
    /// @param email Objeto Email da pessoa pesquisada.
    /// @return Vetor com todos os cartões cadastrados para o usuário informado.
    ///
    std::vector<CartaoDeAtividade> listarCartoesPorPessoa(const Email& email) const;

    /// CONFIRMAR NECESSIDADE DESSE MÉTODO
    /// @brief Recupera o identificador do quadro ao qual um cartão pertence.
    /// @param idCartao Identificador do cartão pesquisado.
    /// @param idQuadro Referência onde será gravado o identificador do quadro.
    /// @return true se o vínculo for localizado; false caso contrário.
    ///
    //bool obterQuadroDoCartao(const Identificador& idCartao, Identificador& idQuadro) const;
};

#endif // CONTAINERDADOS_HPP_INCLUDED

