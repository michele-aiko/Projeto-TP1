#ifndef SERVICOS_HPP_INCLUDED
#define SERVICOS_HPP_INCLUDED
#ifndef SERVICOS_PESSOAS_HPP_INCLUDED
#define SERVICOS_PESSOAS_HPP_INCLUDED

#include "interfaces.hpp"
#include "containerDados.hpp"
#include <stdexcept>
#include <vector>

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

// Na interface abstrata (IServicoPessoas)
class IServicoPessoas {
public:
    virtual ~IServicoPessoas() = default;
    virtual bool criar(const Pessoa& pessoa) = 0;
    virtual bool ler(const Email& email, Pessoa& pessoa) = 0;
    virtual bool atualizar(const Pessoa& pessoa) = 0;
    virtual bool excluir(const Email& email) = 0;
    virtual std::vector<Pessoa> listar() = 0;
    virtual bool autenticar(const Email& email, const Senha& senha) = 0;
};

// Na classe concreta (ServicoPessoas)
class ServicoPessoas : public IServicoPessoas {
public:
    virtual ~ServicoPessoas() = default;

    bool criar(const Pessoa& pessoa) override;
    bool ler(const Email& email, Pessoa& pessoa) override;
    bool atualizar(const Pessoa& pessoa) override;
    bool excluir(const Email& email) override;
    std::vector<Pessoa> listar() override;
    bool autenticar(const Email& email, const Senha& senha) override; 
};


#endif // SERVICOS_HPP_INCLUDED
#endif // SERVICOS_PESSOAS_HPP_INCLUDED