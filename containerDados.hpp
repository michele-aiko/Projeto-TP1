#ifndef CONTAINERDADOS_HPP_INCLUDED
#define CONTAINERDADOS_HPP_INCLUDED

#include "dominios.hpp"
#include "entidades.hpp"
#include <map>
#include <vector>
#include <string>

///
/// @brief Contêineres padrão Singleton para persistência de dados sobre entidades e seus relacionamentos.
/// @details Implementa o armazenamento das quatro entidades do sistema (Pessoa, Projeto, Quadro, CartaoDeAtividade),
///          cada uma com sua própria tabela (std::map) indexada pelas respectivas chaves primárias.
///          Garante o suporte a todas as operações CRUD e consultas relacionais exigidas pelos serviços.
///          Os contêineres NÃO chamam uns aos outros: o cruzamento de informações entre eles (exclusão em cascata, validação de gestor etc.)
///          é realizado na camada de serviços usando os métodos de acesso de cada contêiner.
/// 

///
/// @brief Contêiner de dados da entidade Pessoa.
/// @details Armazena em memória a tabela de pessoas (std::map), indexada pelo e-mail (chave primária).
///          Contém métodos para operações CRUD e consultas relacionais.
///
class ContainerPessoas {
private:
    // Instância única para padrão Singleton
    static ContainerPessoas* instancia;

    // Construtor privado para evitar instanciação direta fora do Singleton
    ContainerPessoas() = default;

    // --- Tabela de Dados (Chave Primária -> Entidade) ---
    std::map<std::string, Pessoa> tabelaPessoas;    ///< Chave: Email (string)

public:
    // Destrutor padrão
    ~ContainerPessoas() = default;

    // Métodos que irão excluir qualquer tentativa de cópia e atribuição direta do contêiner para preservar a unicidade do Singleton
    ContainerPessoas(const ContainerPessoas&) = delete;
    ContainerPessoas& operator=(const ContainerPessoas&) = delete;

    ///
    /// @brief Método para obter a instância única do contêiner de pessoas.
    /// @return Ponteiro para ContainerPessoas.
    ///
    static ContainerPessoas* getInstancia();

    ///
    /// @brief Insere uma nova pessoa no contêiner.
    /// @param pessoa Objeto da classe Pessoa devidamente preenchido.
    /// @return true se cadastrada com sucesso; false caso o e-mail já exista.
    ///
    bool inserirPessoa(const Pessoa& pessoa);

    ///
    /// @brief Busca uma pessoa pelo e-mail (chave primária).
    /// @param email Objeto Email a ser consultado.
    /// @param pessoa Referência para armazenar o objeto localizado.
    /// @return true se encontrada; false caso contrário.
    ///
    bool buscarPessoa(const Email& email, Pessoa& pessoa) const;

    ///
    /// @brief Atualiza os dados de uma pessoa já existente.
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

    ///
    /// @brief verifica se existe uma pessoa com o e-mail informado.
    /// @details Usado pelos serviços de outros módulos (por exemplo, para validar o gestor de um projeto).
    /// @param email Objeto Email a ser consultado.
    /// @return true se existir; false caso contrário.
    ///
    bool existePessoa(const Email& email) const;
};

///
/// @brief Contêiner de dados da entidade Projeto.
/// @details Armazena em memória a tabela de projetos (std::map), indexada pelo identificador (chave primária),
///          e o vínculo de chave estrangeira Projeto -> Gestor.
///
class ContainerProjetos {
private:
    // Instância única para padrão Singleton
    static ContainerProjetos* instancia;

    // Construtor privado para evitar instanciação direta fora do Singleton
    ContainerProjetos() = default;

    // --- Tabela de Dados (Chave Primária -> Entidade) ---
    std::map<std::string, Projeto> tabelaProjetos;                   ///< Chave: Identificador (string)

    // --- Mapeamento Relacional de Chave Estrangeira ---
    std::map<std::string, std::string> vinculoProjetoGestor;         ///< ID Projeto -> Email Gestor

public:
    // Destrutor padrão
    ~ContainerProjetos() = default;

    // Métodos que irão excluir qualquer tentativa de cópia e atribuição direta do contêiner para preservar a unicidade do Singleton
    ContainerProjetos(const ContainerProjetos&) = delete;
    ContainerProjetos& operator=(const ContainerProjetos&) = delete;

    ///
    /// @brief Método para obter a instância única do contêiner de projetos.
    /// @return Ponteiro para ContainerProjetos.
    ///
    static ContainerProjetos* getInstancia();

    ///
    /// @brief Insere um novo projeto associado a um Gestor criador.
    /// @param projeto Objeto Projeto a ser salvo.
    /// @param emailGestor Objeto Email do gestor responsável pelo projeto.
    /// @return true se inserido com sucesso; false se o identificador já existir.
    ///
    bool inserirProjeto(const Projeto& projeto, const Email& emailGestor);

    ///
    /// @brief Busca um projeto pelo seu código identificador.
    /// @param id Objeto Identificador do projeto.
    /// @param projeto Referência para armazenar o objeto localizado.
    /// @return true se encontrado; false caso contrário.
    ///
    bool buscarProjeto(const Identificador& id, Projeto& projeto) const;

    ///
    /// @brief Atualiza as informações de um projeto existente.
    /// @param projeto Objeto Projeto com os dados modificados.
    /// @return true se atualizado; false se o projeto não for encontrado.
    ///
    bool atualizarProjeto(const Projeto& projeto);

    ///
    /// @brief Remove um projeto e o seu vínculo com o gestor.
    /// @details Não remove os quadros nem os cartões do projeto: essa exclusão em cascata é feita
    ///          pelo serviço, usando ContainerQuadros::removerQuadrosPorProjeto e
    ///          ContainerCartoes::removerCartoesPorQuadro.
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

    ///
    /// @brief verifica se existe um projeto com o identificador informado.
    /// @details Usado pelo serviço de Quadros para validar o projeto antes de criar um quadro.
    /// @param id Objeto Identificador do projeto.
    /// @return true se existir; false caso contrário.
    ///
    bool existeProjeto(const Identificador& id) const;

    ///
    /// @brief recupera o e-mail do gestor responsável por um projeto.
    /// @param idProjeto Identificador do projeto pesquisado.
    /// @param emailGestor Referência onde será gravado o e-mail do gestor.
    /// @return true se o vínculo for localizado; false caso contrário.
    ///
    bool obterGestorDoProjeto(const Identificador& idProjeto, Email& emailGestor) const;
};

///
/// @brief Contêiner de dados da entidade Quadro.
/// @details Armazena em memória a tabela de quadros (std::map), indexada pelo identificador (chave primária),
///          e o vínculo de chave estrangeira Quadro -> Projeto.
///
class ContainerQuadros {
private:
    // Instância única para padrão Singleton
    static ContainerQuadros* instancia;

    // Construtor privado para evitar instanciação direta fora do Singleton
    ContainerQuadros() = default;

    // --- Tabela de Dados (Chave Primária -> Entidade) ---
    std::map<std::string, Quadro> tabelaQuadros;                     ///< Chave: Identificador (string)

    // --- Mapeamento Relacional de Chave Estrangeira ---
    std::map<std::string, std::string> vinculoQuadroProjeto;         ///< ID Quadro -> ID Projeto

public:
    // Destrutor padrão
    ~ContainerQuadros() = default;

    // Métodos que irão excluir qualquer tentativa de cópia e atribuição direta do contêiner para preservar a unicidade do Singleton
    ContainerQuadros(const ContainerQuadros&) = delete;
    ContainerQuadros& operator=(const ContainerQuadros&) = delete;

    ///
    /// @brief Método para obter a instância única do contêiner de quadros.
    /// @return Ponteiro para ContainerQuadros.
    ///
    static ContainerQuadros* getInstancia();

    ///
    /// @brief Insere um novo quadro associado a um projeto.
    /// @param quadro Objeto Quadro a ser salvo.
    /// @param idProjeto Objeto Identificador do projeto proprietário.
    /// @return true se inserido com sucesso; false se o identificador já existir.
    ///
    bool inserirQuadro(const Quadro& quadro, const Identificador& idProjeto);

    ///
    /// @brief Busca um quadro pelo seu código identificador.
    /// @param id Objeto Identificador do quadro.
    /// @param quadro Referência para armazenar o objeto localizado.
    /// @return true se encontrado; false caso contrário.
    ///
    bool buscarQuadro(const Identificador& id, Quadro& quadro) const;

    ///
    /// @brief Atualiza as informações de um quadro (como o limite de WIP ou nome).
    /// @param quadro Objeto Quadro atualizado.
    /// @return true se atualizado; false se o quadro não for encontrado.
    ///
    bool atualizarQuadro(const Quadro& quadro);

    ///
    /// @brief Remove um quadro e o seu vínculo com o projeto.
    /// @details Não remove os cartões do quadro: essa exclusão é feita pelo serviço, usando
    ///          ContainerCartoes::removerCartoesPorQuadro.
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

    ///
    /// @brief remove todos os quadros vinculados a um projeto.
    /// @details Usado pelo serviço de Projetos na exclusão em cascata.
    /// @param idProjeto Objeto Identificador do projeto.
    /// @return Quantidade de quadros removidos.
    ///
    int removerQuadrosPorProjeto(const Identificador& idProjeto);

    ///
    /// @brief verifica se existe um quadro com o identificador informado.
    /// @details Usado pelo serviço de Cartões para validar o quadro antes de criar um cartão.
    /// @param id Objeto Identificador do quadro.
    /// @return true se existir; false caso contrário.
    ///
    bool existeQuadro(const Identificador& id) const;

    ///
    /// @brief recupera o identificador do projeto ao qual um quadro pertence.
    /// @param idQuadro Identificador do quadro pesquisado.
    /// @param idProjeto Referência onde será gravado o identificador do projeto.
    /// @return true se o vínculo for localizado; false caso contrário.
    ///
    bool obterProjetoDoQuadro(const Identificador& idQuadro, Identificador& idProjeto) const;
};

///
/// @brief Contêiner de dados da entidade CartaoDeAtividade.
/// @details Armazena em memória a tabela de cartões (std::map), indexada pelo identificador (chave primária),
///          e o vínculo de chave estrangeira Cartão -> Quadro.
///
class ContainerCartoes {
private:
    // Instância única para padrão Singleton
    static ContainerCartoes* instancia;

    // Construtor privado para evitar instanciação direta fora do Singleton
    ContainerCartoes() = default;

    // --- Tabela de Dados (Chave Primária -> Entidade) ---
    std::map<std::string, CartaoDeAtividade> tabelaCartoes;          ///< Chave: Identificador (string)

    // --- Mapeamento Relacional de Chave Estrangeira ---
    std::map<std::string, std::string> vinculoCartaoQuadro;          ///< ID Cartão -> ID Quadro

public:
    // Destrutor padrão
    ~ContainerCartoes() = default;

     // Métodos que irão excluir qualquer tentativa de cópia e atribuição direta do contêiner para preservar a unicidade do Singleton
    ContainerCartoes(const ContainerCartoes&) = delete;
    ContainerCartoes& operator=(const ContainerCartoes&) = delete;

    ///
    /// @brief Método para obter a instância única do contêiner de cartões.
    /// @return Ponteiro para ContainerCartoes.
    ///
    static ContainerCartoes* getInstancia();

    ///
    /// @brief Insere um novo cartão de atividade vinculado a um determinado quadro.
    /// @param cartao Objeto CartaoDeAtividade a ser salvo.
    /// @param idQuadro Objeto Identificador do quadro de destino.
    /// @return true se inserido com sucesso; false se o ID já existir.
    ///
    bool inserirCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro);

    ///
    /// @brief Busca um cartão pelo seu identificador único.
    /// @param id Objeto Identificador do cartão.
    /// @param cartao Referência para armazenar o objeto localizado.
    /// @return true se encontrado; false caso contrário.
    ///
    bool buscarCartao(const Identificador& id, CartaoDeAtividade& cartao) const;

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

    /// @brief Lista todos os cartões vinculados a uma determinada pessoa, independente de estarem no mesmo projeto.
    /// @param email Objeto Email da pessoa pesquisada.
    /// @return Vetor com todos os cartões cadastrados para o usuário informado.
    ///
    std::vector<CartaoDeAtividade> listarCartoesPorPessoa(const Email& email) const;

    ///
    /// @brief Recupera o identificador do quadro ao qual um cartão pertence.
    /// @param idCartao Identificador do cartão pesquisado.
    /// @param idQuadro Referência onde será gravado o identificador do quadro.
    /// @return true se o vínculo for localizado; false caso contrário.
    ///
    bool obterQuadroDoCartao(const Identificador& idCartao, Identificador& idQuadro) const;

    ///
    /// @brief remove todos os cartões vinculados a um quadro.
    /// @details Usado pelos serviços de Quadros e Projetos na exclusão em cascata.
    /// @param idQuadro Objeto Identificador do quadro.
    /// @return Quantidade de cartões removidos.
    ///
    int removerCartoesPorQuadro(const Identificador& idQuadro);

    ///
    /// @brief verifica se existe um cartão com o identificador informado.
    /// @param id Objeto Identificador do cartão.
    /// @return true se existir; false caso contrário.
    ///
    bool existeCartao(const Identificador& id) const;
};

#endif // CONTAINERDADOS_HPP_INCLUDED
