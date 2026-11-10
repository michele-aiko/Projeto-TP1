#include "apresentacao.hpp"
#include <iostream>
#include <limits>
#include <functional>
#include <stdexcept>
#include <string>

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


// =============================================================================
// APRESENTAÇÃO DE INICIALIZAÇÃO E CONTROLE
// =============================================================================

namespace {
// Executa uma ação de tela e traduz as exceções em mensagens para o usuário.
void executarSeguro(const std::function<void()>& acao) {
    try {
        acao();
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE ENTRADA (Formato Invalido): " << e.what() << "\n";
    } catch (const std::logic_error& e) {
        std::cerr << ">>> ERRO DE PERMISSAO: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}
} // namespace

void ApresentacaoControle::validarDependencias() const {
    if (apresentacaoAutenticacao == nullptr) throw std::runtime_error("Modulo de Autenticacao (apresentacao) indisponivel.");
    if (apresentacaoPessoas == nullptr)      throw std::runtime_error("Modulo de Pessoas (apresentacao) indisponivel.");
    if (apresentacaoProjetos == nullptr)     throw std::runtime_error("Modulo de Projetos (apresentacao) indisponivel.");
    if (apresentacaoQuadros == nullptr)      throw std::runtime_error("Modulo de Quadros (apresentacao) indisponivel.");
    if (servicoPessoas == nullptr)           throw std::runtime_error("Servico de Pessoas indisponivel.");
    if (servicoProjetos == nullptr)          throw std::runtime_error("Servico de Projetos indisponivel.");
    if (servicoQuadros == nullptr)           throw std::runtime_error("Servico de Quadros indisponivel.");
}

int ApresentacaoControle::lerOpcao() const {
    std::string linha;
    while (std::getline(std::cin, linha)) {
        if (linha.find_first_not_of(" \t\r") == std::string::npos) continue; // ignora linha vazia
        try {
            return std::stoi(linha);
        } catch (const std::exception&) {
            return -1;
        }
    }
    return 0; // fim da entrada: encerra os menus
}

std::string ApresentacaoControle::lerLinha(const std::string& rotulo) const {
    std::string linha;
    std::cout << rotulo;
    while (std::getline(std::cin, linha)) {
        if (linha.find_first_not_of(" \t\r") != std::string::npos) return linha;
    }
    return "";
}

void ApresentacaoControle::executar() {
    validarDependencias();
    menuInicial();
}

void ApresentacaoControle::menuInicial() {
    int opcao = -1;
    do {
        std::cout << "========================================\n";
        std::cout << " SISTEMA KANBAN - PROJETOS E QUADROS \n";
        std::cout << "========================================\n";
        std::cout << "1. Criar conta\n";
        std::cout << "2. Entrar (login)\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1:
                executarSeguro([this]() {
                    if (apresentacaoPessoas->criarConta()) {
                        std::cout << ">>> SUCESSO: Conta criada com sucesso! Faca o login. <<<\n";
                    }
                });
                break;
            case 2:
                executarSeguro([this]() {
                    Pessoa usuario;
                    if (apresentacaoAutenticacao->autenticar(usuario)) {
                        menuPrincipal(usuario);
                    } else {
                        std::cout << ">>> Login nao realizado. <<<\n";
                    }
                });
                break;
            case 0: break;
            default: std::cout << "Opcao invalida!\n";
        }
        std::cout << "\n";
    } while (opcao != 0);
}

void ApresentacaoControle::menuPrincipal(Pessoa& usuarioLogado) {
    int opcao = -1;
    do {
        const bool gestor = (usuarioLogado.getPapel().getPapel() == Papel::GESTOR);

        std::cout << "\n========================================\n";
        std::cout << " SISTEMA KANBAN - MENU PRINCIPAL \n";
        std::cout << " Usuario: " << usuarioLogado.getNome().getNome()
                  << " (" << usuarioLogado.getPapel().getPapel() << ")\n";
        std::cout << "========================================\n";
        std::cout << "1. Abrir Menu de Projetos\n";
        std::cout << "2. Abrir Quadro Kanban\n";
        if (gestor) {
            std::cout << "3. Excluir Projeto\n";
        }
        std::cout << "4. Minha Conta\n";
        std::cout << "0. Sair da conta\n";
        std::cout << "Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1:
                executarSeguro([&]() { apresentacaoProjetos->executar(usuarioLogado); });
                break;
            case 2:
                executarSeguro([&]() { abrirQuadro(usuarioLogado); });
                break;
            case 3:
                if (gestor) {
                    executarSeguro([&]() { excluirProjeto(usuarioLogado); });
                } else {
                    std::cout << "Apenas gestores podem excluir projetos!\n";
                }
                break;
            case 4: {
                executarSeguro([&]() { apresentacaoPessoas->gerenciarConta(usuarioLogado); });
                // A conta pode ter sido alterada ou excluida: confere a sessao.
                Pessoa atualizada;
                bool existe = false;
                executarSeguro([&]() { existe = servicoPessoas->ler(usuarioLogado.getEmail(), atualizada); });
                if (!existe) {
                    std::cout << ">>> Conta nao existe mais. Sessao encerrada. <<<\n";
                    return;
                }
                usuarioLogado = atualizada;
                break;
            }
            case 0: break;
            default: std::cout << "Opcao invalida!\n";
        }
    } while (opcao != 0);
}

void ApresentacaoControle::abrirQuadro(const Pessoa& usuarioLogado) {
    Identificador idProjeto(lerLinha("ID do projeto (ex: PRJ001): "));

    Projeto projeto;
    if (!servicoProjetos->ler(idProjeto, projeto)) {
        std::cout << "Projeto nao encontrado.\n";
        return;
    }

    std::vector<Quadro> quadros = servicoQuadros->listarQuadrosPorProjeto(idProjeto);
    std::cout << "\n--- QUADROS DO PROJETO " << projeto.getIdentificador().getIdentificador()
              << " (" << quadros.size() << ") ---\n";
    for (const auto& q : quadros) {
        std::cout << "ID: " << q.getIdentificador().getIdentificador()
                  << " | Nome: " << q.getNome().getNome()
                  << " | Limite WIP: " << q.getLimite().getLimite() << "\n";
    }
    if (quadros.empty()) {
        std::cout << "Este projeto ainda nao possui quadros.\n";
        return;
    }

    Identificador idQuadro(lerLinha("ID do quadro (ex: QDR001): "));
    apresentacaoQuadros->exibirQuadro(idQuadro, usuarioLogado);
}

void ApresentacaoControle::excluirProjeto(const Pessoa& usuarioLogado) {
    if (usuarioLogado.getPapel().getPapel() != Papel::GESTOR) {
        std::cout << "Apenas gestores podem excluir projetos!\n";
        return;
    }
    Identificador idProjeto(lerLinha("ID do projeto a excluir (ex: PRJ001): "));
    std::string confirmacao = lerLinha("Confirma a exclusao (S/N)? ");
    if (confirmacao == "S" || confirmacao == "s") {
        if (servicoProjetos->excluir(idProjeto)) {
            std::cout << ">>> SUCESSO: Projeto excluido com sucesso! <<<\n";
        } else {
            std::cout << "Projeto nao encontrado.\n";
        }
    } else {
        std::cout << "Exclusao cancelada.\n";
    }
}

