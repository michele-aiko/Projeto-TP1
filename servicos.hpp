#ifndef SERVICOS_HPP_INCLUDED
#define SERVICOS_HPP_INCLUDED
 
#include "interfaces.hpp"
#include "containerDados.hpp"
#include <stdexcept>
#include <string>
#include <vector>

///
/// @brief Serviço de negócio do Módulo de Autenticação (processa o login).
/// @details Realiza a interface IServicoAutenticacao. Confere e-mail e senha usando o ContainerPessoas.
///
class ServicoAutenticacao : public IServicoAutenticacao {
public:
    virtual ~ServicoAutenticacao() = default;

    ///
    /// @brief Verifica se o e-mail e a senha informados pertencem a uma conta cadastrada.
    /// @details E-mail inexistente ou senha errada NÃO lançam exceção: o resultado é apenas false.
    /// @param email E-mail digitado no login.
    /// @param senha Senha digitada no login.
    /// @param pessoa Recebe e guarda os dados da conta informados pelo usuário somente se o login for válido.
    /// @return true se as credenciais estão corretas; false caso contrário.
    ///
    bool autenticar(const Email& email, const Senha& senha, Pessoa& pessoa) override;
};

///
/// @brief Implementacao dos servicos de negocio para o Modulo de Pessoas.
/// @details Realiza a interface IServicoPessoas. Utiliza ContainerPessoas e consulta
///          ContainerProjetos para garantir integridade referencial na exclusao e na
///          alteracao de papel de gestor.
///
// Requisitos 1 a 5
class ServicoPessoas : public IServicoPessoas {
public:
    virtual ~ServicoPessoas() = default;

    ///
    /// @brief Cadastra uma nova pessoa no repositorio.
    /// @param pessoa Objeto Pessoa preenchido e validado.
    /// @return true se cadastrada com sucesso.
    /// @throw std::runtime_error Se o e-mail ja estiver cadastrado.
    ///
    // Requisito 1
    bool criar(const Pessoa& pessoa) override;

    ///
    /// @brief Consulta uma pessoa pelo e-mail.
    /// @param email E-mail da conta pesquisada.
    /// @param pessoa Recebe e guarda os dados encontrados em objeto tipo Pessoa.
    /// @return true se localizada; false caso contrario.
    ///
    // Requisito 2
    bool ler(const Email& email, Pessoa& pessoa) override;

    ///
    /// @brief Atualiza Nome, Senha e Papel de uma pessoa.
    /// @details Mantem o e-mail inalterado e impede que gestores com projetos
    ///          ativos alterem seu papel para desenvolvedor.
    /// @param pessoa Recebe e guarda os dados atualizados em objeto Pessoa correspondente ao email informado.
    /// @return true se atualizada com sucesso.
    /// @throw std::runtime_error Se nao for localizada ou se alteração de Papel violar integridade de gestor.
    ///
    // Requisitos 3 e 13
    bool atualizar(const Pessoa& pessoa) override;

    ///
    /// @brief Remove o cadastro de uma pessoa.
    /// @details Impede a exclusao se a pessoa possuir projetos sob sua gestao.
    /// @param email E-mail da conta a ser excluida.
    /// @return true se excluida com sucesso.
    /// @throw std::runtime_error Se nao existir ou se possuir projetos sob sua gestao.
    ///
    // Requisitos 4 e 15
    bool excluir(const Email& email) override;

    ///
    /// @brief Lista todas as pessoas cadastradas.
    /// @return Vetor com as pessoas cadastradas (retorna vazio se não houver pessoas cadastradas no sistema).
    ///
    // Requisito 5
    std::vector<Pessoa> listar() override;
};

///
/// @brief Implementação dos serviço para Projetos, conforme regras de negócio e persistência.
/// @details Realiza a interface IServicoProjetos lançando exceções específicas para falhas de negócio.
///          Utiliza ContainerProjetos e, na exclusão em cascata, ContainerQuadros e ContainerCartoes.
///
// Requisitos de 6 a 10
class ServicoProjetos : public IServicoProjetos {
public:
    virtual ~ServicoProjetos() = default;

    ///
    /// @brief Cria um novo projeto associado ao Gestor criador.
    /// @param projeto Projeto já validado.
    /// @param emailGestor E-mail de quem está criando (precisa ter papel GESTOR).
    /// @return true se criou.
    /// @throw std::logic_error Se o usuário criador não possuir o papel de GESTOR.
    /// @throw std::runtime_error Se o identificador do projeto já estiver cadastrado ou se o gestor não existir.
    ///
    // Requisito 6
    bool criar(const Projeto& projeto, const Email& emailGestor) override;

    ///
    /// @brief Consulta um projeto pelo identificador.
    /// @param id Identificador do projeto.
    /// @param projeto Recebe e guarda os dados encontrados em objeto tipo Projeto.
    /// @return true se encontrou; false se não existe (sem exceção).
    ///
    // Requisito 7
    bool ler(const Identificador& id, Projeto& projeto) override;

    ///
    /// @brief Atualiza os dados de um projeto existente.
    /// @details DAdos atualizáveis são: nome, descrição, início e término. O identificador não pode ser alterado.
    /// @param projeto Projeto com o identificador chave e os novos dados informados pelo usuário.
    /// @return true se atualizou.
    /// @throw std::runtime_error Se o projeto a ser atualizado não existir.
    ///
    // Requisitos 8 e 13
    bool atualizar(const Projeto& projeto) override;

    ///
    /// @brief Remove um projeto e todos os seus quadros e cartões associados.
    /// @details Exclusão realizada em cascata, evitando comprometer a integridade dos containers de dados.
    /// @param id Identificador do projeto informado pelo usuário
    /// @throw std::runtime_error Se o projeto não for encontrado para exclusão.
    ///
    // Requisitos 9 e 15
    bool excluir(const Identificador& id) override;

    ///
    /// @brief Lista os projetos de um gestor.
    /// @param emailGestor E-mail do gestor informado, não precisa ser o mesmo do usuário logado.
    /// @return Vetor com os projetos do gestor (vazio se não houver).
    ///
    std::vector<Projeto> listarPorGestor(const Email& emailGestor) override;

    ///
    /// @brief Lista todos os projetos do sistema.
    /// @return Vetor com todos os projetos (vazio se não houver).
    ///
    // Requisito 10
    std::vector<Projeto> listarTodos() override;
};

#endif // SERVICOS_HPP_INCLUDED
