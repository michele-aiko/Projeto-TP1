#ifndef SERVICOS_HPP_INCLUDED
#define SERVICOS_HPP_INCLUDED

#include "interfaces.hpp"
#include "containerDados.hpp"
#include <stdexcept>

///
/// @brief Implementacao dos servicos de negocio e persistencia para o Modulo de Pessoas.
/// @details Realiza a interface IServicoPessoas (Requisitos 1 a 5). Utiliza ContainerPessoas
///          e consulta ContainerProjetos para garantir integridade referencial na exclusao
///          e na alteracao de papel de gestor (Requisito 15).
///
class ServicoPessoas : public IServicoPessoas {
public:
    virtual ~ServicoPessoas() = default;

    ///
    /// @brief Cadastra uma nova pessoa no repositorio (Requisito 1).
    /// @param pessoa Objeto Pessoa preenchido e validado.
    /// @return true se cadastrada com sucesso.
    /// @throw std::runtime_error Se o e-mail ja estiver cadastrado.
    ///
    bool criar(const Pessoa& pessoa) override;

    ///
    /// @brief Consulta uma pessoa pelo e-mail (Requisito 2).
    /// @param email E-mail da conta pesquisada.
    /// @param pessoa Referencia onde os dados encontrados serao gravados.
    /// @return true se localizada; false caso contrario.
    ///
    bool ler(const Email& email, Pessoa& pessoa) override;

    ///
    /// @brief Atualiza Nome, Senha e Papel de uma pessoa (Requisito 3).
    /// @details Mantem o e-mail inalterado (Requisito 13) e impede que gestores com projetos
    ///          ativos alterem seu papel para desenvolvedor (Requisito 15).
    /// @param pessoa Objeto com o e-mail e os novos atributos.
    /// @return true se atualizada com sucesso.
    /// @throw std::runtime_error Se nao for localizada ou se violar integridade de gestor.
    ///
    bool atualizar(const Pessoa& pessoa) override;

    ///
    /// @brief Remove o cadastro de uma pessoa (Requisito 4).
    /// @details Impede a exclusao se a pessoa possuir projetos sob sua gestao (Requisito 15).
    /// @param email E-mail da conta a ser excluida.
    /// @return true se excluida com sucesso.
    /// @throw std::runtime_error Se nao existir ou se possuir projetos sob sua gestao.
    ///
    bool excluir(const Email& email) override;

    ///
    /// @brief Lista todas as pessoas cadastradas (Requisito 5).
    /// @return Vetor com as pessoas cadastradas.
    ///
    std::vector<Pessoa> listar() override;
};

///
/// @brief Implementação dos serviço conforme regras de negócio e persistência para Projetos.
/// @details Realiza a interface IServicoProjetos lançando exceções específicas para falhas de negócio.
///
class ServicoProjetos : public IServicoProjetos {
public:
    virtual ~ServicoProjetos() = default;

    ///
    /// @brief Cria um novo projeto associado ao Gestor criador.
    /// @throw std::logic_error Se o usuário criador não possuir o papel de GESTOR.
    /// @throw std::runtime_error Se o identificador do projeto já estiver cadastrado ou se o gestor não existir.
    ///
    bool criar(const Projeto& projeto, const Email& emailGestor) override;

    ///
    /// @brief Consulta um projeto pelo identificador.
    /// @throw std::runtime_error Se o projeto com o identificador informado não for encontrado.
    ///
    bool ler(const Identificador& id, Projeto& projeto) override;

    ///
    /// @brief Atualiza os dados de um projeto existente.
    /// @throw std::runtime_error Se o projeto a ser atualizado não existir.
    ///
    bool atualizar(const Projeto& projeto) override;

    ///
    /// @brief Remove um projeto e todos os seus quadros e cartões associados.
    /// @throw std::runtime_error Se o projeto não for encontrado para exclusão.
    ///
    bool excluir(const Identificador& id) override;

    std::vector<Projeto> listarPorGestor(const Email& emailGestor) override;
    std::vector<Projeto> listarTodos() override;
};


#endif // SERVICOS_HPP_INCLUDED
