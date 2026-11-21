///
/// @brief Ponto de entrada do Sistema Kanban e menu de testes/execução.
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
#include <limits>

//Função auxiliar para pausar a tela durante demonstrações na gravação.
void pausar() {
    std::cout << "\nPressione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
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
    // 2. INJEÇÃO DE DEPENDÊNCIAS (Conectando Telas aos Serviços)
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
    // 3. MENU DE EXECUÇÃO E GRAVAÇÃO DO TP1
    // -------------------------------------------------------------------------
    int opcao = -1;
    do {
        std::cout << "\n====================================================\n";
        std::cout << "            SISTEMA KANBAN  -  MENU PRINCIPAL       \n";
        std::cout << "====================================================\n";
        std::cout << " 1. Executar Sistema Interativo (Interface do Usuario)\n";
        std::cout << " 2. Executar Todos os Testes Unitarios\n";
        std::cout << " 3. Executar Teste Fumaca de Pessoas (RF 1 a 5)\n";
        std::cout << " 4. Executar Teste Fumaca de Projetos (RF 6 a 10)\n";
        std::cout << " 5. Executar Teste Fumaca de Quadros e Cartoes (RF 11 a 26)\n";
        std::cout << " 6. Executar Teste Fumaca de Inicialização e Controle\n";
        std::cout << " 7. Executar TODOS os Testes (Unitarios e Fumaca)\n";
        std::cout << " 0. Sair\n";
        std::cout << " Escolha uma opcao: ";

        if (!(std::cin >> opcao)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcao) {
            case 1:
                try {
                    apresentacaoControle.executar();
                } catch (const std::exception& e) {
                    std::cerr << ">>> ERRO NO SISTEMA: " << e.what() << "\n";
                }
                break;

            case 2:
                std::cout << "\n--- EXECUTANDO CONJUNTO DE TESTES UNITARIOS ---\n";
                TUDominios().executar();
                TUEntidades().executar();
                TUServicoAutenticacao().executar();
                TUServicoPessoas().executar();
                TUServicoProjetos().executar();
                TUServicoQuadros().executar();
                TUApresentacaoAutenticacao().executar();
                TUApresentacaoPessoas().executar();
                TUApresentacaoProjetos().executar();
                TUApresentacaoQuadros().executar();
                TUApresentacaoControle().executar();
                std::cout << "\n>>> TODOS OS TESTES UNITARIOS PASSARAM COM SUCESSO! <<<\n";
                pausar();
                break;

            case 3:
                TFumacaPessoas().executar(apresentacaoPessoas);
                pausar();
                break;

            case 4:
                TFumacaProjetos().executar(apresentacaoProjetos, servicoProjetos);
                pausar();
                break;

            case 5:
                TFumacaQuadros().executar(apresentacaoQuadros, servicoQuadros, servicoProjetos);
                pausar();
                break;

            case 6:
                TFumacaControle().executar(apresentacaoControle);
                pausar();
                break;

            case 7:
                std::cout << "\n--- EXECUTANDO LISTA COMPLETA DE TESTES ---\n";

                // 1. Executa conjunto de testes unitários
                TUDominios().executar();
                TUEntidades().executar();
                TUServicoAutenticacao().executar();
                TUServicoPessoas().executar();
                TUServicoProjetos().executar();
                TUServicoQuadros().executar();
                TUApresentacaoAutenticacao().executar();
                TUApresentacaoPessoas().executar();
                TUApresentacaoProjetos().executar();
                TUApresentacaoQuadros().executar();
                TUApresentacaoControle().executar();

                // 2. Limpeza preventiva dos contêineres Singleton para os testes fumaça
                ContainerPessoas::getInstancia()->removerPessoa(Email("fumaca.pessoa@unb.br"));
                ContainerProjetos::getInstancia()->removerProjeto(Identificador("PRJ701"));
                ContainerProjetos::getInstancia()->removerProjeto(Identificador("PRJ702"));
                ContainerProjetos::getInstancia()->removerProjeto(Identificador("PRJ777"));

                // 3. Executa conjunto de testes fumaça
                TFumacaPessoas().executar(apresentacaoPessoas);
                TFumacaProjetos().executar(apresentacaoProjetos, servicoProjetos);
                TFumacaQuadros().executar(apresentacaoQuadros, servicoQuadros, servicoProjetos);
                TFumacaControle().executar(apresentacaoControle);

                std::cout << "\n>>> LISTA COMPLETA CONCLUIDA COM SUCESSO! <<<\n";
                pausar();
                break;


            case 0:
                std::cout << "Encerrando o programa.\n";
                break;

            default:
                std::cout << "Opcao invalida!\n";
                break;
        }
    } while (opcao != 0);

    return 0;
}
