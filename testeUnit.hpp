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

#endif // TESTEUNIT_HPP_INCLUDED