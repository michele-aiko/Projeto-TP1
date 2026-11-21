#include "apresentacao.hpp"
#include <iostream>
#include <limits>
#include <functional>
#include <stdexcept>
#include <string>

// =============================================================================
// MÓDULO DE AUTENTICAÇÃO (ApresentacaoAutenticacao)
// =============================================================================

void ApresentacaoAutenticacao::validarDependencia() const {
    if (servico == nullptr) {
        throw std::runtime_error("Servico de Autenticacao indisponivel.");
    }
}

std::string ApresentacaoAutenticacao::lerLinha(const std::string& rotulo) const {
    std::string linha;
    std::cout << rotulo;
    while (std::getline(std::cin, linha)) {
        if (linha.find_first_not_of(" \t\r\n") != std::string::npos) return linha;
    }
    return "";
}


bool ApresentacaoAutenticacao::autenticar(Pessoa& usuario) {
    validarDependencia();
    std::cout << "\n=== TELA DE LOGIN ===" << std::endl;
    std::string sEmail = lerLinha("E-mail: ");
    std::string sSenha = lerLinha("Senha: ");

    try {
        Email email(sEmail);
        Senha senha(sSenha);
        return servico->autenticar(email, senha, usuario);
    } catch (const std::invalid_argument& e) {

        std::cout << "\n>>> ERRO DE ENTRADA: Formato invalido nas credenciais de login e senha: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "\n>>> ERRO DE NEGOCIO: " << e.what() << "\n";
        return false;
    }
}

// =============================================================================
// APRESENTAÇÃO DE PESSOAS
// =============================================================================

void ApresentacaoPessoas::validarDependencia() const {
    if (servico == nullptr) {
        throw std::runtime_error("Servico de Pessoas indisponivel.");
    }
}

int ApresentacaoPessoas::lerOpcao() const {
    std::string linha;
    while (std::getline(std::cin, linha)) {
        if (linha.find_first_not_of(" \t\r") == std::string::npos) continue;
        try {
            return std::stoi(linha);
        } catch (const std::exception&) {
            return -1;
        }
    }
    return 0;
}

std::string ApresentacaoPessoas::lerLinha(const std::string& rotulo) const {
    std::string linha;
    std::cout << rotulo;
    while (std::getline(std::cin, linha)) {
        std::size_t ini = linha.find_first_not_of(" \t\r");
        if (ini != std::string::npos) {
            std::size_t fim = linha.find_last_not_of(" \t\r");
            return linha.substr(ini, fim - ini + 1);
        }
    }
    return "";
}

bool ApresentacaoPessoas::criarConta() {
    validarDependencia();

    std::cout << "\n--- CRIAR NOVA CONTA DE USUARIO ---\n";
    std::string sEmail = lerLinha("E-mail (ex: usuario@dominio.com): ");
    std::string sNome  = lerLinha("Nome (ate 15 caracteres, apenas letras e espacos): ");
    std::string sSenha = lerLinha("Senha (5 caracteres alfanumericos, com letra e digito): ");
    std::string sPapel = lerLinha("Papel (GESTOR ou DESENVOLVEDOR): ");

    try {
        Email email(sEmail);
        Nome nome(sNome);
        Senha senha(sSenha);
        Papel papel(sPapel);

        Pessoa novaPessoa(email, nome, senha, papel);
        servico->criar(novaPessoa);
        std::cout << ">>> SUCESSO: Conta cadastrada com sucesso! <<<\n";
        return true;
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE ENTRADA (Formato Invalido): " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
    return false;
}

void ApresentacaoPessoas::gerenciarConta(Pessoa& usuarioLogado) {
    validarDependencia();

    int opcao = -1;
    do {
        std::cout << "\n========================================\n";
        std::cout << "       GERENCIAMENTO DE CONTA           \n";
        std::cout << " Usuario: " << usuarioLogado.getNome().getNome()
                  << " | E-mail: " << usuarioLogado.getEmail().getEmail()
                  << " (" << usuarioLogado.getPapel().getPapel() << ")\n";
        std::cout << "========================================\n";
        std::cout << " 1. Ler Dados da Conta\n";
        std::cout << " 2. Atualizar Dados da Conta (Nome, Senha, Papel)\n";
        std::cout << " 3. Excluir Minha Conta\n";
        std::cout << " 4. Listar Pessoas Cadastradas\n";
        std::cout << " 0. Voltar ao Menu Principal\n";
        std::cout << " Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1:
                lerDadosConta(usuarioLogado);
                break;
            case 2:
                atualizarConta(usuarioLogado);
                break;
            case 3:
                if (excluirConta(usuarioLogado)) {
                    return; // Conta excluída: encerra o menu de gerenciamento
                }
                break;
            case 4:
                listarPessoas();
                break;
            case 0:
                break;
            default:
                std::cout << "Opcao invalida!\n";
                break;
        }
    } while (opcao != 0);
}

void ApresentacaoPessoas::lerDadosConta(const Pessoa& usuarioLogado) {
    try {
        Pessoa lida;
        if (!servico->ler(usuarioLogado.getEmail(), lida)) {
            std::cout << "Conta nao encontrada.\n";
            return;
        }
        std::cout << "\n--- DADOS DA PESSOA ---\n";
        std::cout << "E-mail (PK): " << lida.getEmail().getEmail() << "\n";
        std::cout << "Nome:        " << lida.getNome().getNome() << "\n";
        std::cout << "Papel:       " << lida.getPapel().getPapel() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}

void ApresentacaoPessoas::atualizarConta(Pessoa& usuarioLogado) {
    std::cout << "\n--- ATUALIZAR DADOS DA CONTA ---\n";
    std::cout << "(Nota: O e-mail '" << usuarioLogado.getEmail().getEmail()
              << "' e chave primaria e nao pode ser alterado)\n";

    std::string sNome  = lerLinha("Novo Nome (ate 15 caracteres): ");
    std::string sSenha = lerLinha("Nova Senha (5 caracteres alfanumericos): ");
    std::string sPapel = lerLinha("Novo Papel (GESTOR ou DESENVOLVEDOR): ");

    try {
        Nome novoNome(sNome);
        Senha novaSenha(sSenha);
        Papel novoPapel(sPapel);

        // Requisito 13: Não é possível editar dado que identifica entidade (chave primária: Email)
        Pessoa atualizada(usuarioLogado.getEmail(), novoNome, novaSenha, novoPapel);
        servico->atualizar(atualizada);
        usuarioLogado = atualizada;
        std::cout << ">>> SUCESSO: Dados da conta atualizados com sucesso! <<<\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE ENTRADA (Formato Invalido): " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}

bool ApresentacaoPessoas::excluirConta(const Pessoa& usuarioLogado) {
    std::cout << "\n--- EXCLUIR MINHA CONTA ---\n";
    std::string confirmacao = lerLinha("Confirma a exclusao permanente da conta (S/N)? ");
    if (confirmacao != "S" && confirmacao != "s") {
        std::cout << "Exclusao cancelada.\n";
        return false;
    }

    try {
        servico->excluir(usuarioLogado.getEmail());
        std::cout << ">>> SUCESSO: Conta excluida com sucesso! <<<\n";
        return true;
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE ENTRADA (Formato Invalido): " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
    return false;
}

void ApresentacaoPessoas::listarPessoas() {
    try {
        std::vector<Pessoa> lista = servico->listar();
        std::cout << "\n--- PESSOAS CADASTRADAS (" << lista.size() << ") ---\n";
        for (const auto& p : lista) {
            // Requisito 11: Listagem de entidade resulta na apresentação do identificador de cada entidade
            std::cout << "E-mail: " << p.getEmail().getEmail()
                      << " | Nome: " << p.getNome().getNome()
                      << " | Papel: " << p.getPapel().getPapel() << "\n";
        }
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}

// =============================================================================
// APRESENTAÇÃO DE PROJETOS
// =============================================================================

void ApresentacaoProjetos::validarDependencia() const {
    if (servico == nullptr) {
        throw std::runtime_error("Servico de Projetos indisponivel.");
    }
}

int ApresentacaoProjetos::lerOpcao() const {
    std::string linha;
    while (std::getline(std::cin, linha)) {

        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        try {
            return std::stoi(linha);
        } catch (const std::exception&) {
            return -1;
        }
    }
    return 0;
}

std::string ApresentacaoProjetos::lerLinha(const std::string& rotulo) const {
    std::string linha;
    std::cout << rotulo;
    while (std::getline(std::cin, linha)) {
        std::size_t ini = linha.find_first_not_of(" \t\r\n");
        if (ini != std::string::npos) {
            std::size_t fim = linha.find_last_not_of(" \t\r\n");
            return linha.substr(ini, fim - ini + 1);         }
    }
    return "";
}

void ApresentacaoProjetos::executar(const Pessoa& usuarioLogado) {
    validarDependencia();
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

        opcao = lerOpcao();

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
            default: std::cout << "Opcao invalida!\n"; break;
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
    std::cout << "\n--- CRIAR NOVO PROJETO ---\n";
    std::string sId   = lerLinha("ID (ex: PRJ001): ");
    std::string sNome = lerLinha("Nome (ate 15 chars): ");
    std::string sDesc = lerLinha("Descricao (ate 30 chars com ponto final): ");
    std::string sIni  = lerLinha("Inicio (ex: 01-JAN-2026-10:00): ");
    std::string sFim  = lerLinha("Termino (ex: 30-JUN-2026-18:00): ");

    try {
        Projeto novo{Identificador(sId), Nome(sNome), Texto(sDesc), Timestamp(sIni), Timestamp(sFim)};
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

void ApresentacaoControle::validarDependencias() const {
    if (apresentacaoAutenticacao == nullptr) throw std::runtime_error("Modulo de Autenticacao (apresentacao) indisponivel.");
    if (apresentacaoPessoas == nullptr)      throw std::runtime_error("Modulo de Pessoas (apresentacao) indisponivel.");
    if (apresentacaoProjetos == nullptr)     throw std::runtime_error("Modulo de Projetos (apresentacao) indisponivel.");
    if (apresentacaoQuadros == nullptr)      throw std::runtime_error("Modulo de Quadros (apresentacao) indisponivel.");
    if (servicoPessoas == nullptr)           throw std::runtime_error("Servico de Pessoas indisponivel.");
    if (servicoProjetos == nullptr)          throw std::runtime_error("Servico de Projetos indisponivel.");
    if (servicoQuadros == nullptr)           throw std::runtime_error("Servico de Quadros indisponivel.");
}

// Executa uma ação de tela e traduz as exceções em mensagens para o usuário.
void ApresentacaoControle::executarSeguro(const std::function<void()>& acao) const {
    try {
        acao();
    } catch (const std::invalid_argument& e) {
        std::cerr << "\n>>> ERRO DE ENTRADA (Formato Invalido): " << e.what() << "\n";
    } catch (const std::logic_error& e) {
        std::cerr << "\n>>> ERRO DE PERMISSAO: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "\n>>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
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
                        std::cout << ">>> Login nao realizado. E-mail ou senha incorretos. <<<\n";
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
        std::cout << "2. Gerenciar e Abrir Quadros Kanban\n";
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
                    std::cout << ">>> Sessao encerrada: a conta foi excluida. <<<\n";
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
    auto projetos = servicoProjetos->listarTodos();

    if (projetos.empty()) {
        std::cout << "Nenhum projeto cadastrado no sistema.\n";
        return;
    }

    std::cout << "\n--- PROJETOS CADASTRADOS ---\n";
    for (const auto& p : projetos) {
        std::cout << "ID: " << p.getIdentificador().getIdentificador()
                  << " | Nome: " << p.getNome().getNome() << "\n";
    }

    Identificador idProjeto(lerLinha("Digite o ID do projeto (ex: PRJ001) para exibir seus quadros: "));

    Projeto projeto;
    if (!servicoProjetos->ler(idProjeto, projeto)) {
        std::cout << "Projeto nao encontrado.\n";
        return;
    }

    apresentacaoQuadros->gerenciarQuadros(idProjeto, usuarioLogado);
}

void ApresentacaoControle::excluirProjeto(const Pessoa& usuarioLogado) {
    if (usuarioLogado.getPapel().getPapel() != Papel::GESTOR) {
        std::cout << "Apenas gestores podem excluir projetos!\n";
        return;
    }

    Identificador idProjeto(lerLinha("ID do projeto a ser excluido (ex: PRJ001): "));
    std::string confirmacao = lerLinha("Tem certeza que deseja excluir o projeto e TODOS os seus quadros e cartoes (S/N)? ");
    if (confirmacao == "S" || confirmacao == "s") {
        if (servicoProjetos->excluir(idProjeto)) {
            std::cout << ">>> SUCESSO: Projeto e seus quadros excluidos em cascata com sucesso! <<<\n";
        } else {
            std::cout << "Projeto nao encontrado.\n";
        }
    } else {
        std::cout << "Exclusao cancelada.\n";
    }
}

// =============================================================================
// MÓDULO DE QUADROS KANBAN (ApresentacaoQuadros)
// =============================================================================

void ApresentacaoQuadros::validarDependencia() const {
    if (servico == nullptr) {
        throw std::runtime_error("Servico de Quadros indisponivel.");
    }
}

int ApresentacaoQuadros::lerOpcao() const {
    std::string linha;
    while (std::getline(std::cin, linha)) {
        if (linha.empty() || linha.find_first_not_of(" \t\r\n") == std::string::npos) continue;
        try {
            return std::stoi(linha);
        } catch (const std::exception&) {
            return -1;
        }
    }
    return 0;
}

std::string ApresentacaoQuadros::lerLinha(const std::string& rotulo) const {
    std::string linha;
    std::cout << rotulo;
    while (std::getline(std::cin, linha)) {
        std::size_t ini = linha.find_first_not_of(" \t\r\n");
        if (ini != std::string::npos) {
            std::size_t fim = linha.find_last_not_of(" \t\r\n");
            return linha.substr(ini, fim - ini + 1);
        }
    }
    return "";
}

// Menu Principal do Projeto -> Quadros
void ApresentacaoQuadros::gerenciarQuadros(const Identificador& idProjeto, const Pessoa& usuarioLogado) {
    validarDependencia();
    int opcao = -1;
    do {
        auto quadros = servico->listarQuadrosPorProjeto(idProjeto);
        std::cout << "\n========================================\n";
        std::cout << "      GERENCIAMENTO DE QUADROS          \n";
        std::cout << "========================================\n";
        std::cout << " 1. Listar e Abrir Quadro (Gerenciamento de Cartoes)\n";
        if (usuarioLogado.getPapel().getPapel() == Papel::GESTOR) {
            std::cout << " 2. Criar Novo Quadro no Projeto\n";
        }
        std::cout << " 0. Voltar ao Menu Principal\n";
        std::cout << " Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1: {
                if (quadros.empty()) {
                    std::cout << "Nenhum quadro cadastrado neste projeto.\n";
                    break;
                }
                std::cout << "\n--- QUADROS DISPONIVEIS ---\n";
                for (const auto& q : quadros) {
                    std::cout << "ID: " << q.getIdentificador().getIdentificador()
                              << " | Nome: " << q.getNome().getNome() << "\n";
                }
                std::string sIdQuadro = lerLinha("Digite o ID do Quadro que deseja abrir: ");
                exibirQuadro(Identificador(sIdQuadro), usuarioLogado); // <--- ABRE A TELA DO QUADRO E DOS CARTÕES
                break;
            }
            case 2:
                if (usuarioLogado.getPapel().getPapel() == Papel::GESTOR) {
                    criarQuadro(idProjeto);
                }
                break;
            case 0: break;
            default: std::cout << "Opcao invalida!\n"; break;
        }
    } while (opcao != 0);
}

void ApresentacaoQuadros::criarQuadro(const Identificador& idProjeto) {
    std::cout << "\n--- CRIAR NOVO QUADRO KANBAN ---\n";
    std::string sId   = lerLinha("ID do Quadro (ex: QDR001): ");
    std::string sNome = lerLinha("Nome do Quadro (ate 15 chars): ");
    std::string sWip  = lerLinha("Limite de WIP para coluna FAZENDO (1 a 25): ");

    try {
        int nLimite = std::stoi(sWip);
        Quadro novo{Identificador(sId), Nome(sNome), Limite(nLimite)};
        servico->criarQuadro(novo, idProjeto);
        std::cout << ">>> SUCESSO: Quadro criado com sucesso! <<<\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE FORMATO OU LIMITE: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}

// Tela do Quadro Aberto: Exibe Colunas e Submenu de Cartões
void ApresentacaoQuadros::exibirQuadro(const Identificador& idQuadro, const Pessoa& usuarioLogado) {
    validarDependencia();
    Quadro q;
    try {
        if (!servico->lerQuadro(idQuadro, q)) {
            std::cout << "Quadro nao encontrado.\n";
            return;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO: " << e.what() << "\n";
        return;
    }

    int opcao = -1;
    do {
        auto cartoes = servico->listarCartoesPorQuadro(idQuadro);
        int wipAtual = servico->lerWip(idQuadro);
        std::cout << "\n=================================================================\n";
        std::cout << " QUADRO: " << q.getNome().getNome() << " [ID: " << q.getIdentificador().getIdentificador() << "]\n";
        std::cout << " LIMITE DE WIP (FAZENDO): " << wipAtual << " / " << q.getLimite().getLimite() << "\n";
        std::cout << "=================================================================\n";

        std::cout << "\n[ A FAZER ]\n";
        for (const auto& c : cartoes) {
            if (c.getEstado().getEstado() == Estado::A_FAZER) {
                std::cout << " - [" << c.getIdentificador().getIdentificador() << "] "
                          << c.getNome().getNome()
                          << " (Prioridade: " << c.getPrioridade().getPrioridade() << ")\n";
            }
        }

        std::cout << "\n[ FAZENDO ]\n";
        for (const auto& c : cartoes) {
            if (c.getEstado().getEstado() == Estado::FAZENDO) {
                std::cout << " - [" << c.getIdentificador().getIdentificador() << "] "
                          << c.getNome().getNome()
                          << " (Prioridade: " << c.getPrioridade().getPrioridade() << ")\n";
            }
        }

        std::cout << "\n[ FEITO ]\n";
        for (const auto& c : cartoes) {
            if (c.getEstado().getEstado() == Estado::FEITO) {
                std::cout << " - [" << c.getIdentificador().getIdentificador() << "] "
                          << c.getNome().getNome() << "\n";
            }
        }

        std::cout << "\n-----------------------------------------------------------------\n";
        std::cout << " 1. Mover Cartao de Coluna\n";
        if (usuarioLogado.getPapel().getPapel() == Papel::GESTOR) {
            std::cout << " 2. Adicionar Novo Cartao\n";
        }
        std::cout << " 3. Consultar Metricas Kanban (Lead Time e Cycle Time)\n";
        std::cout << " 0. Voltar ao Menu de Quadros\n";
        std::cout << " Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1: moverCartao(idQuadro); break;
            case 2:
                if (usuarioLogado.getPapel().getPapel() == Papel::GESTOR) {
                    adicionarCartao(idQuadro);
                } else {
                    std::cout << "Apenas gestores podem adicionar novos cartoes!\n";
                }
                break;
            case 3:
                try {
                    std::cout << "\n--- METRICAS DE DESEMPENHO KANBAN ---\n";
                    std::cout << "WIP Atual: " << servico->lerWip(idQuadro) << " tarefa(s) em andamento.\n";
                    std::cout << "Tempo de Ciclo Medio: " << servico->lerTempoDeCicloMedio(idQuadro) << " minuto(s).\n";
                    std::cout << "Lead Time Medio: " << servico->lerLeadTimeMedio(idQuadro) << " minuto(s).\n";
                } catch (const std::runtime_error& e) {
                    std::cerr << ">>> ERRO AO LER METRICAS: " << e.what() << "\n";
                }
                break;
            case 0: break;
            default: std::cout << "Opcao invalida!\n"; break;
        }
    } while (opcao != 0);
}

// Função de Adicionar Cartão na coluna A FAZER (Requisito 16)
void ApresentacaoQuadros::adicionarCartao(const Identificador& idQuadro) {
    std::cout << "\n--- CRIAR NOVO CARTAO DE ATIVIDADE ---\n";
    std::string id    = lerLinha("ID do Cartao (ex: ACT001): ");
    std::string nome  = lerLinha("Nome (ate 15 chars): ");
    std::string prio  = lerLinha("Prioridade ('ALTA', 'MEDIA' ou 'BAIXA'): ");
    std::string tam   = lerLinha("Tamanho ('PEQUENO', 'MEDIO' ou 'GRANDE'): ");
    std::string stamp = lerLinha("Timestamp de Criacao (ex: 01-NOV-2024-09:00): ");

    try {
        Texto descPadrao("Descricao inicial.");
        CartaoDeAtividade cartao(Identificador(id), Nome(nome), descPadrao,
                                Prioridade(prio), Estado(Estado::A_FAZER), Tamanho(tam),
                                Timestamp(stamp), Timestamp(stamp), Timestamp(stamp));
        servico->criarCartao(cartao, idQuadro);
        std::cout << ">>> SUCESSO: Cartao inserido na coluna A FAZER! <<<\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE FORMATO: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE NEGOCIO: " << e.what() << "\n";
    }
}

// Função de Mover Cartão entre colunas (Requisito 21)
void ApresentacaoQuadros::moverCartao(const Identificador& idQuadro) {
    std::cout << "\n--- MOVER CARTAO DE COLUNA ---\n";
    std::string sId     = lerLinha("ID do cartao a mover (ex: ACT001): ");
    std::string sEstado = lerLinha("Novo Estado ('FAZENDO' ou 'FEITO'): ");
    std::string sHora   = lerLinha("Timestamp da transicao (ex: 03-NOV-2024-15:00): ");

    try {
        servico->moverCartao(Identificador(sId), Estado(sEstado), Timestamp(sHora));
        std::cout << ">>> SUCESSO: Cartao movimentado com sucesso! <<<\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << ">>> ERRO DE FORMATO: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << ">>> ERRO DE REGRA OPERACIONAL (WIP / Transicao): " << e.what() << "\n";
    }
}

