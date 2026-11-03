#include "apresentacao.hpp"
#include <iostream>
#include <limits>

// =============================================================================
// APRESENTAÇÃO DE PROJETOS
// =============================================================================

void ApresentacaoProjetos::executar(const Pessoa& usuarioLogado) {
    int opcao = -1;
    do {
        std::cout << "\n========================================\n";
        std::cout << "         GERENCIAMENTO DE PROJETOS      \n";
        std::cout << " Usuario: " << usuarioLogado.getNome().getNome()
                  << " (" << usuarioLogado.getPapel().getPapel() << ")\n";
        std::cout << "========================================\n";
        std::cout << " 1. Listar Todos os Projetos\n";
        std::cout << " 2. Listar Meus Projetos (Gestor)\n";
        if (usuarioLogado.getPapel().getPapel() == Papel::GESTOR) {
            std::cout << " 3. Criar Novo Projeto\n";
        }
        std::cout << " 0. Voltar ao Menu Principal\n";
        std::cout << " Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao) {
            case 1: listarTodos(); break;
            case 2: listarMeusProjetos(usuarioLogado); break;
            case 3:
                if (usuarioLogado.getPapel().getPapel() == Papel::GESTOR) {
                    criarProjeto(usuarioLogado);
                } else {
                    std::cout << "Apenas gestores podem criar projetos!\n";
                }
                break;
            case 0: break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (opcao != 0);
}

void ApresentacaoProjetos::listarTodos() {
    auto lista = servico->listarTodos();
    std::cout << "\n--- PROJETOS CADASTRADOS (" << lista.size() << ") ---\n";
    for (const auto& p : lista) {
        std::cout << "ID: " << p.getIdentificador().getIdentificador()
                  << " | Nome: " << p.getNome().getNome()
                  << " | Descricao: " << p.getDescricao().getTexto() << "\n";
    }
}

void ApresentacaoProjetos::listarMeusProjetos(const Pessoa& usuarioLogado) {
    auto lista = servico->listarPorGestor(usuarioLogado.getEmail());
    std::cout << "\n--- SEUS PROJETOS (" << lista.size() << ") ---\n";
    for (const auto& p : lista) {
        std::cout << "ID: " << p.getIdentificador().getIdentificador()
                  << " | Nome: " << p.getNome().getNome() << "\n";
    }
}

void ApresentacaoProjetos::criarProjeto(const Pessoa& usuarioLogado) {
    std::string sId, sNome, sDesc, sIni, sFim;
    std::cout << "\n--- CRIAR NOVO PROJETO ---\n";
    std::cout << "ID (ex: PRJ001): "; std::cin >> sId;
    std::cout << "Nome (ate 15 chars): "; std::cin >> sNome;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Descricao (ate 30 chars com ponto final): "; std::getline(std::cin, sDesc);
    std::cout << "Inicio (ex: 01-JAN-2024-10:00): "; std::cin >> sIni;
    std::cout << "Termino (ex: 30-JUN-2024-18:00): "; std::cin >> sFim;

    try {
        // Validação sintática dos Domínios
        Projeto novo(Identificador(sId), Nome(sNome), Texto(sDesc), Timestamp(sIni), Timestamp(sFim));

        // Chamada de Serviço (Pode disparar logic_error ou runtime_error)
        servico->criar(novo, usuarioLogado.getEmail());
        std::cout << ">>> SUCESSO: Projeto criado e salvo com sucesso! <<<\n";

    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE ENTRADA (Formato Invalido): " << e.what() << "\n";
    } catch (const std::logic_error& e) {
        std::cerr << ">>> ERRO DE PERMISSAO: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}

