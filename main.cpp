///
/// @brief Ponto de entrada do Sistema Kanban.
/// @details Realiza a instanciação das camadas de serviço e apresentação, a disponibilização
///          de dependências via interfaces abstratas, a execução de todos os conjuntos de 
///          testes (unitários e fumaça) e a inicialização do loop principal do sistema.
///

#include "apresentacao.hpp"
#include "servicos.hpp"
#include "testeUnit.hpp"
#include "testeFumaca.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "====================================================\n";
    std::cout << "            INICIALIZANDO SISTEMA KANBAN            \n";
    std::cout << "====================================================\n\n";

    // -------------------------------------------------------------------------
    // 1. INSTANCIAÇÃO DAS CAMADAS CONCRETAS (Serviços e Apresentação)
    // -------------------------------------------------------------------------
    ServicoAutenticacao servicoAutenticacao;
    ServicoPessoas      servicoPessoas;
    ServicoProjetos     servicoProjetos;
    ServicoQuadros      servicoQuadros;

    ApresentacaoAutenticacao apresentacaoAutenticacao;
    ApresentacaoPessoas      apresentacaoPessoas;
    ApresentacaoProjetos     apresentacaoProjetos;
    ApresentacaoQuadros      apresentacaoQuadros;
    ApresentacaoControle     apresentacaoControle;

    // -------------------------------------------------------------------------
    // 2. INJEÇÃO DE DEPENDÊNCIAS (Ligação pelas Interfaces Abstratas)
    // -------------------------------------------------------------------------
    apresentacaoAutenticacao.setServico(&servicoAutenticacao);
    apresentacaoPessoas.setServico(&servicoPessoas);
    apresentacaoProjetos.setServico(&servicoProjetos);
    apresentacaoQuadros.setServico(&servicoQuadros);

    apresentacaoControle.setApresentacaoAutenticacao(&apresentacaoAutenticacao);
    apresentacaoControle.setApresentacaoPessoas(&apresentacaoPessoas);
    apresentacaoControle.setApresentacaoProjetos(&apresentacaoProjetos);
    apresentacaoControle.setApresentacaoQuadros(&apresentacaoQuadros);

    apresentacaoControle.setServicoPessoas(&servicoPessoas);
    apresentacaoControle.setServicoProjetos(&servicoProjetos);
    apresentacaoControle.setServicoQuadros(&servicoQuadros);

    // -------------------------------------------------------------------------
    // 3. EXECUÇÃO DOS TESTES UNITÁRIOS (Domínios, Entidades, Serviços, Telas)
    // -------------------------------------------------------------------------
    TUDominios tuDominios;
    tuDominios.executar();

    TUEntidades tuEntidades;
    tuEntidades.executar();

    TUServicoAutenticacao tuServAut;
    tuServAut.executar();

    TUServicoPessoas tuServPes;
    tuServPes.executar();

    TUServicoProjetos tuServProj;
    tuServProj.executar();

    TUServicoQuadros tuServQuad;
    tuServQuad.executar();

    TUApresentacaoAutenticacao tuApAut;
    tuApAut.executar();

    TUApresentacaoPessoas tuApPes;
    tuApPes.executar();

    TUApresentacaoProjetos tuApProj;
    tuApProj.executar();

    TUApresentacaoQuadros tuApQuad;
    tuApQuad.executar();

    TUApresentacaoControle tuApCtrl;
    tuApCtrl.executar();

    // -------------------------------------------------------------------------
    // 4. EXECUÇÃO DOS TESTES FUMAÇA (Integração Apresentação -> Serviço -> Dados)
    // -------------------------------------------------------------------------
    TFumacaPessoas tfPessoas;
    tfPessoas.executar(apresentacaoPessoas);

    TFumacaProjetos tfProjetos;
    tfProjetos.executar(apresentacaoProjetos, servicoProjetos);

    TFumacaQuadros tfQuadros;
    tfQuadros.executar(apresentacaoQuadros, servicoQuadros, servicoProjetos);

    TFumacaControle tfControle;
    tfControle.executar(apresentacaoControle);

    std::cout << ">>> TODOS OS TESTES PASSARAM COM SUCESSO! INICIANDO INTERFACE... <<<\n\n";

    // -------------------------------------------------------------------------
    // 5. EXECUÇÃO DO SISTEMA (Loop Principal de Menus do Usuário)
    // -------------------------------------------------------------------------
    try {
        apresentacaoControle.executar();
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO FATAL NO SISTEMA: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nEncerrando o programa com sucesso.\n";
    return 0;
}
