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

#endif // TESTEFUMACA_HPP_INCLUDED