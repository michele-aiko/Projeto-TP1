#ifndef TESTEFUMACA_HPP_INCLUDED
#define TESTEFUMACA_HPP_INCLUDED

#include "apresentacao.hpp"

///
/// @brief Declara os testes fumaça (smoke tests) do sistema Kanban.
/// @details Teste fumaça roda o sistema integrado (Apresentação -> Serviço -> Contêiner) e verifica se
///          as funcionalidades principais funcionam em um caso de sucesso.
///          Em caso de falha, a mensagem é mostrada e o programa termina com `std::exit(1)`.
///

///
/// @brief Teste fumaça do Módulo de Inicialização e Controle.
/// @details Usa a controladora ligada aos módulos REAIS e simula o teclado com um roteiro de entradas.
///          Cobre: criar conta e login, listagem de projetos, abertura de quadros,
///          exclusão de projeto em cascata, logout, bloqueio de permissão do
///          DESENVOLVEDOR e login recusado.
///
class TFumacaControle {
public:
    ///
    /// @brief Executa o teste fumaça da controladora.
    /// @param controle Controladora mestra já com todos os módulos injetados.
    ///
    void executar(ApresentacaoControle& controle);
};

///
/// @brief Teste fumaça do Módulo de Pessoas.
/// @details Executa um caso de sucesso para cada funcionalidade: Criar, Ler, Atualizar, Listar e
///          Excluir Pessoa, todas pela tela de Pessoas.
///
// Requisitos Funcionais 1 a 5
class TFumacaPessoas {
public:
    ///
    /// @brief Executa o teste fumaça do Módulo de Pessoas.
    /// @param apresentacaoPessoas Tela de Pessoas já ligada ao serviço real.
    ///
    void executar(IApresentacaoPessoas& apresentacaoPessoas);
};

///
/// @brief Teste fumaça do Módulo de Projetos.
/// @details Criar e Listar passam pela tela de Projetos. Ler, Atualizar e Excluir ainda não têm menu na
///          tela e são executados diretamente pela interface de serviço (comentado no código).
///
// Requisitos Funcionais 6 a 10
class TFumacaProjetos {
public:
    ///
    /// @brief Executa o teste fumaça do Módulo de Projetos.
    /// @param apresentacao Tela de Projetos já ligada ao serviço real.
    /// @param servico Serviço de Projetos real (usado nas funcionalidades sem menu na tela).
    ///
    void executar(IApresentacaoProjetos& apresentacao, IServicoProjetos& servico);
};

///
/// @brief Teste fumaça do Módulo de Quadros e Cartões.
/// @details Usa a tela de Quadros para criar/listar quadros, criar cartões, mover cartões (inclusive o
///          bloqueio por limite de WIP) e ver as métricas médias. As demais funcionalidades (ler,
///          atualizar e excluir quadro e cartão, tempo de ciclo e lead time por cartão) são executadas
///          pela interface de serviço.
///
// Requisitos Funcionais 11 a 26
class TFumacaQuadros {
public:
    ///
    /// @brief Executa o teste fumaça do Módulo de Quadros e Cartões.
    /// @param apresentacao Tela de Quadros já ligada ao serviço real.
    /// @param servicoQuadros Serviço de Quadros real.
    /// @param servicoProjetos Serviço de Projetos real (usado para criar o projeto do quadro).
    ///
    void executar(IApresentacaoQuadros& apresentacao, IServicoQuadros& servicoQuadros, IServicoProjetos& servicoProjetos);
};

#endif // TESTEFUMACA_HPP_INCLUDED
