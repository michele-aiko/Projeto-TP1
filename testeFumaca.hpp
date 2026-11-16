#ifndef TESTEFUMACA_HPP_INCLUDED
#define TESTEFUMACA_HPP_INCLUDED

#include "apresentacao.hpp"

///
/// @brief Teste Fumaça (smoke test) do Módulo de Inicialização e Controle.
/// @details Usa a controladora já ligada aos módulos REAIS (serviços e apresentações) e simula o teclado
///          com um roteiro de entradas. Verifica o fluxo completo: login de Gestor, listagem de projetos,
///          abertura de quadro, exclusão de projeto e logout, além do bloqueio de permissão para Desenvolvedor.
///
class TFumacaControle {
public:
    /// @brief Executa o teste fumaça; encerra o programa se alguma verificação falhar.
    /// @param controle Controladora mestra já com todos os módulos injetados.
    void executar(ApresentacaoControle& controle);
};

///
/// @brief Teste Fumaça (smoke test) integrado do Módulo de Pessoas.
/// @details Executa de ponta a ponta (Apresentação -> Serviço -> Contêiner) um caso de teste de sucesso
///          para cada uma das 5 funcionalidades do Módulo de Pessoas (Requisitos Funcionais 1 a 5:
///          Criar Pessoa, Ler Pessoa, Atualizar Pessoa, Listar Pessoa e Excluir Pessoa), utilizando
///          as implementações reais de `ApresentacaoPessoas`, `ServicoPessoas` e `ContainerPessoas`.
///
class TFumacaPessoas {
public:
    /// @brief Executa o teste fumaça integrado do Módulo de Pessoas.
    /// @param apresentacaoPessoas Controladora de apresentação de pessoas já ligada ao `ServicoPessoas` real.
    void executar(ApresentacaoPessoas& apresentacaoPessoas);
};

#endif // TESTEFUMACA_HPP_INCLUDED