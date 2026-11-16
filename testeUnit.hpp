#ifndef TESTEUNIT_HPP_INCLUDED
#define TESTEUNIT_HPP_INCLUDED

#include "servicos.hpp"
#include "apresentacao.hpp"

///
/// @brief Classe de Teste Unitário para validação dos Serviços de Projetos.
///
class TUTesteServicos {
private:
    ServicoProjetos servicoProj;

    void testarCriacaoProjeto();

public:
    void executar();
};

// =============================================================================
// TESTES UNITÁRIOS DO MÓDULO DE INICIALIZAÇÃO E CONTROLE 
// =============================================================================

///
/// @brief Classe de Teste Unitário para a controladora ApresentacaoControle.
/// @details Isola a controladora usando módulos falsos (mocks) das interfaces abstratas e simula o
///          teclado e a tela do console, verificando o fluxo de menus, as permissões por papel
///          e o lançamento de std::runtime_error quando um módulo está indisponível.
///
class TUApresentacaoControle {
private:
    void testarModulosIndisponiveis();
    void testarCriarContaELogin();
    void testarLoginFalho();
    void testarOpcaoInvalida();
    void testarPermissaoDesenvolvedor();
    void testarExclusaoDeProjeto();
    void testarAbrirQuadro();
    void testarIdentificadorInvalido();
    void testarSessaoAposGerenciarConta();

public:
    /// @brief Executa todos os testes da controladora; encerra o programa se algum falhar.
    void executar();
};

// =============================================================================
// TESTES UNITÁRIOS DO MÓDULO DE PESSOAS
// =============================================================================

///
/// @brief Classe de Teste Unitário para a camada de serviço do Módulo de Pessoas (`ServicoPessoas`).
/// @details Valida todos os serviços de negócio (Criar, Ler, Atualizar, Excluir e Listar Pessoa -
///          Requisitos Funcionais 1 a 5), o armazenamento em `ContainerPessoas` e o lançamento de
///          `std::runtime_error` em cenários de e-mail duplicado, pessoa inexistente e tentativa de
///          exclusão de pessoa que possui projetos vinculados (Requisito 15).
///
class TUServicoPessoas {
private:
    ServicoPessoas servicoPessoas; ///< Instância do serviço de pessoas sob teste.

    void testarCriarELerPessoa();
    void testarAtualizarPessoa();
    void testarListarPessoas();
    void testarExcluirPessoaEIntegridade();

public:
    /// @brief Executa todos os testes unitários de `ServicoPessoas`.
    void executar();
};

///
/// @brief Classe de Teste Unitário para a controladora de apresentação do Módulo de Pessoas (`ApresentacaoPessoas`).
/// @details Isola `ApresentacaoPessoas` utilizando um mock de `IServicoPessoas` e redirecionamento de
///          fluxos de console (`std::cin`, `std::cout`, `std::cerr`), verificando o lançamento de
///          `std::runtime_error` quando o serviço não está injetado, a validação sintática dos domínios
///          de entrada e o acionamento dos serviços de Criar, Ler, Atualizar, Excluir e Listar Pessoas.
///
class TUApresentacaoPessoas {
private:
    void testarServicoIndisponivel();
    void testarCriarContaValidaEInvalida();
    void testarGerenciarContaLerAtualizarListar();
    void testarGerenciarContaExcluir();

public:
    /// @brief Executa todos os testes unitários de `ApresentacaoPessoas`.
    void executar();
};

#endif // TESTEUNIT_HPP_INCLUDED