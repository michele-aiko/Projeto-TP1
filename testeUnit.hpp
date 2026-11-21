#ifndef TESTEUNIT_HPP_INCLUDED
#define TESTEUNIT_HPP_INCLUDED

#include "servicos.hpp"
#include "apresentacao.hpp"

///
/// @file testeUnit.hpp
/// @brief Declara as classes de teste unitário do sistema Kanban (domínios, entidades, serviços e apresentação).
/// @details Cada método `executar()` roda todos os testes da sua classe. Se uma verificação falhar,
///          o programa mostra a mensagem de falha e é encerrado com `std::exit(1)`.
///          Os testes de apresentação usam objetos falsos (mocks) no lugar dos serviços e simulam
///          o teclado e a tela do console. Os testes de serviço usam os contêineres reais e
///          apagam os dados que criam, para não interferir nos demais testes.
///

// =============================================================================
// TESTES UNITÁRIOS DOS DOMÍNIOS E DAS ENTIDADES
// =============================================================================

///
/// @brief Teste unitário das 11 classes de domínio.
/// @details Para cada domínio testa um cenário com valor válido (o valor é guardado) e cenários com
///          valor inválido (`std::invalid_argument` é lançada e o valor antigo é mantido).
///
class TUDominios {
private:
    void testarEmail();       ///< Testa o domínio Email.
    void testarEstado();      ///< Testa o domínio Estado.
    void testarIdentificador(); ///< Testa o domínio Identificador.
    void testarLimite();      ///< Testa o domínio Limite.
    void testarNome();        ///< Testa o domínio Nome.
    void testarPapel();       ///< Testa o domínio Papel.
    void testarPrioridade();  ///< Testa o domínio Prioridade.
    void testarSenha();       ///< Testa o domínio Senha.
    void testarTamanho();     ///< Testa o domínio Tamanho.
    void testarTexto();       ///< Testa o domínio Texto.
    void testarTimestamp();   ///< Testa o domínio Timestamp (inclui ano bissexto).

public:
    /// @brief Executa todos os testes de domínio; encerra o programa se algum falhar.
    void executar();
};

///
/// @brief Teste unitário das 4 classes de entidade (Pessoa, Projeto, Quadro e CartaoDeAtividade).
/// @details Confere se o construtor, os métodos `set` e os métodos `get` guardam e devolvem
///          corretamente os objetos de domínio de cada atributo.
///
class TUEntidades {
private:
    void testarPessoa();  ///< Testa construtores, set e get de Pessoa.
    void testarProjeto(); ///< Testa construtores, set e get de Projeto.
    void testarQuadro();  ///< Testa construtores, set e get de Quadro.
    void testarCartao();  ///< Testa construtores, set e get de CartaoDeAtividade.

public:
    /// @brief Executa todos os testes de entidade; encerra o programa se algum falhar.
    void executar();
};

// =============================================================================
// TESTES UNITÁRIOS DA CAMADA DE SERVIÇO
// =============================================================================

///
/// @brief Teste unitário do serviço de Autenticação (`ServicoAutenticacao`).
/// @details Verifica login válido e os casos de falha (senha errada e e-mail inexistente),
///          que devem retornar `false` sem lançar exceção.
///
class TUServicoAutenticacao {
private:
    ServicoAutenticacao servico; ///< Serviço sob teste.
    void testarLoginValido();    ///< Credenciais corretas retornam true e preenchem a Pessoa.
    void testarLoginInvalido();  ///< Senha errada ou e-mail inexistente retornam false.

public:
    /// @brief Executa todos os testes de `ServicoAutenticacao`.
    void executar();
};

///
/// @brief Teste unitário do serviço de Pessoas (`ServicoPessoas`, Requisitos 1 a 5).
/// @details Valida Criar, Ler, Atualizar, Excluir e Listar, além dos `std::runtime_error` para
///          e-mail duplicado, pessoa inexistente e integridade com projetos (Requisito 15).
///
class TUServicoPessoas {
private:
    ServicoPessoas servicoPessoas; ///< Serviço sob teste.
    void testarCriarELerPessoa();          ///< Requisitos 1 e 2.
    void testarAtualizarPessoa();          ///< Requisito 3.
    void testarListarPessoas();            ///< Requisito 5.
    void testarExcluirPessoaEIntegridade(); ///< Requisitos 4 e 15.

public:
    /// @brief Executa todos os testes de `ServicoPessoas`.
    void executar();
};

///
/// @brief Teste unitário do serviço de Projetos (`ServicoProjetos`, Requisitos 6 a 10).
/// @details Valida as exceções: `std::logic_error` (usuário não é GESTOR) e `std::runtime_error`
///          (gestor inexistente, ID duplicado, projeto inexistente), além da exclusão em cascata.
///
class TUServicoProjetos {
private:
    ServicoProjetos servico; ///< Serviço sob teste.
    void testarCriarProjeto();           ///< Requisito 6 e permissões.
    void testarLerEAtualizarProjeto();   ///< Requisitos 7, 8 e 13.
    void testarListarProjetos();         ///< Requisito 10.
    void testarExcluirProjetoEmCascata(); ///< Requisitos 9 e 15.

public:
    /// @brief Executa todos os testes de `ServicoProjetos`.
    void executar();
};

///
/// @brief Teste unitário do serviço de Quadros e Cartões (`ServicoQuadros`, Requisitos 11 a 26).
/// @details Cobre CRUD de quadros e cartões, movimentação, regra de WIP, métricas
///          (tempo de ciclo e lead time) e exclusão em cascata.
///
class TUServicoQuadros {
private:
    ServicoQuadros servico; ///< Serviço sob teste.
    void testarQuadros();           ///< Requisitos 11 a 15.
    void testarCartoes();           ///< Requisitos 16 a 20.
    void testarMoverCartaoEWip();   ///< Requisitos 21 e 26 (transições, horário e limite de WIP).
    void testarMetricas();          ///< Requisitos 22 a 25.
    void testarExclusaoEmCascata(); ///< Requisito 15 (excluir quadro remove seus cartões).

public:
    /// @brief Executa todos os testes de `ServicoQuadros`.
    void executar();
};

// =============================================================================
// TESTES UNITÁRIOS DA CAMADA DE APRESENTAÇÃO
// =============================================================================

///
/// @brief Teste unitário da tela de login (`ApresentacaoAutenticacao`).
/// @details Usa um serviço falso. Verifica serviço indisponível, login válido, login recusado
///          e dados de entrada com formato inválido.
///
class TUApresentacaoAutenticacao {
private:
    void testarServicoIndisponivel(); ///< Sem serviço injetado deve lançar std::runtime_error.
    void testarLoginValidoEInvalido(); ///< Sucesso, recusa do serviço e erro de formato.

public:
    /// @brief Executa todos os testes de `ApresentacaoAutenticacao`.
    void executar();
};

///
/// @brief Teste unitário da tela de Pessoas (`ApresentacaoPessoas`).
/// @details Usa um serviço falso e simula o console. Verifica serviço indisponível, validação dos
///          domínios de entrada e o acionamento de Criar, Ler, Atualizar, Excluir e Listar.
///
class TUApresentacaoPessoas {
private:
    void testarServicoIndisponivel();
    void testarCriarContaValidaEInvalida();
    void testarGerenciarContaLerAtualizarListar();
    void testarGerenciarContaExcluir();

public:
    /// @brief Executa todos os testes de `ApresentacaoPessoas`.
    void executar();
};

///
/// @brief Teste unitário da tela de Projetos (`ApresentacaoProjetos`).
/// @details Usa um serviço falso. Verifica listagens, criação (sucesso, formato inválido, erro de
///          negócio e erro de permissão) e as restrições do papel DESENVOLVEDOR.
///
class TUApresentacaoProjetos {
private:
    void testarServicoIndisponivel();
    void testarListagens();
    void testarCriarProjeto();
    void testarPermissaoDesenvolvedor();

public:
    /// @brief Executa todos os testes de `ApresentacaoProjetos`.
    void executar();
};

///
/// @brief Teste unitário da tela de Quadros e Cartões (`ApresentacaoQuadros`).
/// @details Usa um serviço falso. Verifica a listagem e abertura de quadros, a criação de quadros e
///          cartões, a movimentação (inclusive bloqueio de WIP) e a exibição das métricas.
///
class TUApresentacaoQuadros {
private:
    void testarServicoIndisponivel();
    void testarGerenciarQuadros();
    void testarCriarQuadro();
    void testarExibirQuadroCartoes();
    void testarMoverCartao();
    void testarMetricas();

public:
    /// @brief Executa todos os testes de `ApresentacaoQuadros`.
    void executar();
};

///
/// @brief Teste unitário da controladora mestra (`ApresentacaoControle`).
/// @details Isola a controladora com módulos falsos (mocks) e simula teclado e tela. Verifica menus,
///          permissões por papel, abertura de quadros e `std::runtime_error` para módulo indisponível.
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

