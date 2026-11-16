#include "testeUnit.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void TUTesteServicos::testarCriacaoProjeto() {
    // 1. Cadastra Gestor e Desenvolvedor no container
    Pessoa gestor(Email("gestor@unb.br"), Nome("Gestor Teste"), Senha("A1b2c"), Papel(Papel::GESTOR));
    Pessoa dev(Email("dev@unb.br"), Nome("Dev Teste"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR));
    ContainerDados::getInstancia()->inserirPessoa(gestor);
    ContainerDados::getInstancia()->inserirPessoa(dev);

    Projeto p(Identificador("PRJ999"), Nome("Projeto Teste"), Texto("Descricao valida."),
              Timestamp("01-JAN-2024-10:00"), Timestamp("30-JAN-2024-18:00"));

    // Cenário 1: Gestor cria com sucesso
    assert(servicoProj.criar(p, gestor.getEmail()) == true);

    // Cenário 2 (Exceção de Permissão): Desenvolvedor tenta criar projeto -> Deve lançar std::logic_error
    bool disparouErroPermissao = false;
    try {
        Projeto pInvalido(Identificador("PRJ888"), Nome("Projeto Dev"), Texto("Descricao valida."),
                          Timestamp("01-JAN-2024-10:00"), Timestamp("30-JAN-2024-18:00"));
        servicoProj.criar(pInvalido, dev.getEmail());
    } catch (const std::logic_error&) {
        disparouErroPermissao = true;
    }
    assert(disparouErroPermissao == true);

    // Cenário 3 (Exceção de ID Duplicado): Tentar criar novamente com PRJ999 -> Deve lançar std::runtime_error
    bool disparouErroDuplicado = false;
    try {
        servicoProj.criar(p, gestor.getEmail());
    } catch (const std::runtime_error&) {
        disparouErroDuplicado = true;
    }
    assert(disparouErroDuplicado == true);

    std::cout << "  [OK] Teste Unitario: Criacao de Projeto e Lancamento de Excecoes de Permissao/Unicidade.\n";
}

void TUTesteServicos::executar() {
    std::cout << "\n============================================\n";
    std::cout << "   EXECUTANDO TESTES UNITARIOS DE SERVICOS  \n";
    std::cout << "============================================\n";
    testarCriacaoProjeto();
    std::cout << ">>> TODOS OS TESTES PASSARAM COM SUCESSO! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DO MÓDULO DE INICIALIZAÇÃO E CONTROLE 
// =============================================================================

namespace {

// Verificação que também funciona em Release (assert some quando NDEBUG está definido).
void verificar(bool condicao, const std::string& descricao) {
    if (!condicao) {
        std::cerr << "  [FALHA] " << descricao << "\n";
        std::exit(1);
    }
}

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}

int contar(const std::string& texto, const std::string& trecho) {
    int n = 0;
    for (std::size_t pos = texto.find(trecho); pos != std::string::npos; pos = texto.find(trecho, pos + trecho.size())) ++n;
    return n;
}

// Substitui cin, cout e cerr por buffers em memória enquanto o objeto existir.
class CapturaES {
private:
    std::istringstream entrada;
    std::ostringstream saida;
    std::streambuf* bufIn;
    std::streambuf* bufOut;
    std::streambuf* bufErr;

public:
    explicit CapturaES(const std::string& roteiro) : entrada(roteiro) {
        bufIn = std::cin.rdbuf(entrada.rdbuf());
        bufOut = std::cout.rdbuf(saida.rdbuf());
        bufErr = std::cerr.rdbuf(saida.rdbuf());
    }
    ~CapturaES() {
        std::cin.rdbuf(bufIn);
        std::cout.rdbuf(bufOut);
        std::cerr.rdbuf(bufErr);
        std::cin.clear();
        std::cout.clear();
        std::cerr.clear();
    }
    std::string texto() const { return saida.str(); }
};

// --- Módulos falsos (mocks) das interfaces abstratas ---

class MockApresentacaoAutenticacao : public IApresentacaoAutenticacao {
public:
    bool sucesso = true;
    Pessoa usuario;
    int chamadas = 0;
    bool autenticar(Pessoa& pessoa) override { ++chamadas; if (sucesso) pessoa = usuario; return sucesso; }
    void setServico(IServicoAutenticacao*) override {}
};

class MockApresentacaoPessoas : public IApresentacaoPessoas {
public:
    int criarContaChamadas = 0;
    int gerenciarChamadas = 0;
    bool criarConta() override { ++criarContaChamadas; return true; }
    void gerenciarConta(Pessoa&) override { ++gerenciarChamadas; }
    void setServico(IServicoPessoas*) override {}
};

class MockApresentacaoProjetos : public IApresentacaoProjetos {
public:
    int chamadas = 0;
    void executar(const Pessoa&) override { ++chamadas; }
    void setServico(IServicoProjetos*) override {}
};

class MockApresentacaoQuadros : public IApresentacaoQuadros {
public:
    int chamadas = 0;
    std::string ultimoId;
    void exibirQuadro(const Identificador& idQuadro, const Pessoa&) override {
        ++chamadas;
        ultimoId = idQuadro.getIdentificador();
    }
    void setServico(IServicoQuadros*) override {}
};

class MockServicoPessoas : public IServicoPessoas {
public:
    bool existe = true;
    Pessoa pessoaAtual;
    bool criar(const Pessoa&) override { return true; }
    bool ler(const Email&, Pessoa& pessoa) override { if (existe) pessoa = pessoaAtual; return existe; }
    bool atualizar(const Pessoa&) override { return true; }
    bool excluir(const Email&) override { return true; }
    std::vector<Pessoa> listar() override { return {}; }
};

class MockServicoProjetos : public IServicoProjetos {
public:
    bool existe = true;
    int exclusoes = 0;
    bool criar(const Projeto&, const Email&) override { return true; }
    bool ler(const Identificador&, Projeto& projeto) override {
        if (existe) {
            projeto = Projeto(Identificador("PRJ001"), Nome("Projeto Mock"), Texto("Descricao valida."),
                              Timestamp("01-JAN-2024-10:00"), Timestamp("30-JAN-2024-18:00"));
        }
        return existe;
    }
    bool atualizar(const Projeto&) override { return true; }
    bool excluir(const Identificador&) override { ++exclusoes; return existe; }
    std::vector<Projeto> listarPorGestor(const Email&) override { return {}; }
    std::vector<Projeto> listarTodos() override { return {}; }
};

class MockServicoQuadros : public IServicoQuadros {
public:
    std::vector<Quadro> quadros;
    bool criarQuadro(const Quadro&, const Identificador&) override { return true; }
    bool lerQuadro(const Identificador&, Quadro&) override { return true; }
    bool atualizarQuadro(const Quadro&) override { return true; }
    bool excluirQuadro(const Identificador&) override { return true; }
    std::vector<Quadro> listarQuadrosPorProjeto(const Identificador&) override { return quadros; }
    bool criarCartao(const CartaoDeAtividade&, const Identificador&) override { return true; }
    bool lerCartao(const Identificador&, CartaoDeAtividade&) override { return true; }
    bool atualizarCartao(const CartaoDeAtividade&) override { return true; }
    bool excluirCartao(const Identificador&) override { return true; }
    std::vector<CartaoDeAtividade> listarCartoesPorQuadro(const Identificador&) override { return {}; }
    bool moverCartao(const Identificador&, const Estado&, const Timestamp&) override { return true; }
    int lerWip(const Identificador&) override { return 0; }
    int lerTempoDeCiclo(const Identificador&) override { return 0; }
    double lerTempoDeCicloMedio(const Identificador&) override { return 0; }
    int lerLeadTime(const Identificador&) override { return 0; }
    double lerLeadTimeMedio(const Identificador&) override { return 0; }
};

// Monta a controladora ligada aos mocks, com um usuário (GESTOR por padrão) que faz login com sucesso.
struct Cenario {
    MockApresentacaoAutenticacao autenticacao;
    MockApresentacaoPessoas pessoas;
    MockApresentacaoProjetos projetos;
    MockApresentacaoQuadros quadrosTela;
    MockServicoPessoas servicoPessoas;
    MockServicoProjetos servicoProjetos;
    MockServicoQuadros servicoQuadros;
    ApresentacaoControle controle;

    explicit Cenario(const std::string& papel = Papel::GESTOR) {
        Pessoa usuario(Email("usuario@unb.br"), Nome("Usuario Teste"), Senha("A1b2c"), Papel(papel));
        autenticacao.usuario = usuario;
        servicoPessoas.pessoaAtual = usuario;
        controle.setApresentacaoAutenticacao(&autenticacao);
        controle.setApresentacaoPessoas(&pessoas);
        controle.setApresentacaoProjetos(&projetos);
        controle.setApresentacaoQuadros(&quadrosTela);
        controle.setServicoPessoas(&servicoPessoas);
        controle.setServicoProjetos(&servicoProjetos);
        controle.setServicoQuadros(&servicoQuadros);
    }

    // Executa a controladora com o roteiro de teclas informado e devolve tudo o que foi escrito na tela.
    std::string rodar(const std::string& roteiro) {
        CapturaES captura(roteiro);
        controle.executar();
        return captura.texto();
    }
};

} // namespace

void TUApresentacaoControle::testarModulosIndisponiveis() {
    const char* esperado[7] = {
        "Autenticacao", "Pessoas (apresentacao)", "Projetos (apresentacao)", "Quadros (apresentacao)",
        "Servico de Pessoas", "Servico de Projetos", "Servico de Quadros"
    };

    for (int faltando = 0; faltando < 7; ++faltando) {
        Cenario c;
        // Remove da controladora exatamente um módulo por vez.
        switch (faltando) {
            case 0: c.controle.setApresentacaoAutenticacao(nullptr); break;
            case 1: c.controle.setApresentacaoPessoas(nullptr); break;
            case 2: c.controle.setApresentacaoProjetos(nullptr); break;
            case 3: c.controle.setApresentacaoQuadros(nullptr); break;
            case 4: c.controle.setServicoPessoas(nullptr); break;
            case 5: c.controle.setServicoProjetos(nullptr); break;
            case 6: c.controle.setServicoQuadros(nullptr); break;
        }

        bool lancou = false;
        std::string mensagem;
        try {
            c.rodar("0\n");
        } catch (const std::runtime_error& e) {
            lancou = true;
            mensagem = e.what();
        }
        verificar(lancou, std::string("executar() deve lancar runtime_error sem o modulo: ") + esperado[faltando]);
        verificar(contem(mensagem, esperado[faltando]), std::string("mensagem deve citar: ") + esperado[faltando]);
    }
    std::cout << "  [OK] Teste Unitario: Controle lanca runtime_error para cada um dos 7 modulos indisponiveis.\n";
}

void TUApresentacaoControle::testarCriarContaELogin() {
    Cenario c;
    // 1 = criar conta; 2 = login (entra no menu principal); 0 = sair da conta; 0 = sair do sistema
    std::string tela = c.rodar("1\n2\n0\n0\n");

    verificar(contem(tela, "SISTEMA KANBAN - PROJETOS E QUADROS"), "tela inicial deve exibir o cabecalho do sistema");
    verificar(c.pessoas.criarContaChamadas == 1, "opcao 1 deve acionar o modulo de Pessoas (criar conta)");
    verificar(contem(tela, "Conta criada com sucesso"), "deve avisar que a conta foi criada");
    verificar(c.autenticacao.chamadas == 1, "opcao 2 deve acionar o modulo de Autenticacao");
    verificar(contem(tela, "MENU PRINCIPAL"), "login com sucesso deve abrir o menu principal");
    verificar(contem(tela, "Usuario Teste (GESTOR)"), "menu principal deve mostrar nome e papel do usuario");
    std::cout << "  [OK] Teste Unitario: Criar conta e login com sucesso abrem o menu principal.\n";
}

void TUApresentacaoControle::testarLoginFalho() {
    Cenario c;
    c.autenticacao.sucesso = false;
    std::string tela = c.rodar("2\n0\n");

    verificar(contem(tela, "Login nao realizado"), "login falho deve avisar o usuario");
    verificar(!contem(tela, "MENU PRINCIPAL"), "login falho nao pode abrir o menu principal");
    std::cout << "  [OK] Teste Unitario: Login recusado nao abre o menu principal.\n";
}

void TUApresentacaoControle::testarOpcaoInvalida() {
    Cenario c;
    // Número fora do menu e texto não numérico: ambos são recusados sem derrubar o programa.
    std::string tela = c.rodar("9\nabc\n0\n");

    verificar(contar(tela, "Opcao invalida!") == 2, "devem ser recusadas as duas entradas invalidas");
    std::cout << "  [OK] Teste Unitario: Opcao numerica invalida e entrada nao numerica sao recusadas.\n";
}

void TUApresentacaoControle::testarPermissaoDesenvolvedor() {
    Cenario c(Papel::DESENVOLVEDOR);
    // login; tenta excluir projeto (3); sai da conta; sai do sistema
    std::string tela = c.rodar("2\n3\n0\n0\n");

    verificar(contem(tela, "(DESENVOLVEDOR)"), "menu deve mostrar o papel DESENVOLVEDOR");
    verificar(!contem(tela, "3. Excluir Projeto"), "desenvolvedor nao deve ver a opcao de excluir projeto");
    verificar(contem(tela, "Apenas gestores podem excluir projetos!"), "opcao 3 deve ser negada ao desenvolvedor");
    verificar(c.servicoProjetos.exclusoes == 0, "nenhuma exclusao pode chegar ao servico");
    std::cout << "  [OK] Teste Unitario: Desenvolvedor nao pode excluir projetos.\n";
}

void TUApresentacaoControle::testarExclusaoDeProjeto() {
    // Gestor confirma a exclusao
    {
        Cenario c;
        std::string tela = c.rodar("2\n3\nPRJ001\nS\n0\n0\n");
        verificar(c.servicoProjetos.exclusoes == 1, "confirmacao S deve chamar o servico de exclusao uma vez");
        verificar(contem(tela, "Projeto excluido com sucesso"), "deve avisar que o projeto foi excluido");
    }
    // Gestor cancela
    {
        Cenario c;
        std::string tela = c.rodar("2\n3\nPRJ001\nN\n0\n0\n");
        verificar(c.servicoProjetos.exclusoes == 0, "confirmacao N nao pode chamar o servico");
        verificar(contem(tela, "Exclusao cancelada."), "deve avisar que a exclusao foi cancelada");
    }
    // Projeto inexistente
    {
        Cenario c;
        c.servicoProjetos.existe = false;
        std::string tela = c.rodar("2\n3\nPRJ001\nS\n0\n0\n");
        verificar(contem(tela, "Projeto nao encontrado."), "deve avisar quando o projeto nao existe");
    }
    std::cout << "  [OK] Teste Unitario: Exclusao de projeto (confirmada, cancelada e inexistente).\n";
}

void TUApresentacaoControle::testarAbrirQuadro() {
    // Projeto com quadro: abre a tela do quadro escolhido
    {
        Cenario c;
        c.servicoQuadros.quadros.push_back(Quadro(Identificador("QDR001"), Nome("Sprint Um"), Limite(3)));
        std::string tela = c.rodar("2\n2\nPRJ001\nQDR001\n0\n0\n");
        verificar(contem(tela, "QUADROS DO PROJETO PRJ001"), "deve listar os quadros do projeto");
        verificar(contem(tela, "Limite WIP: 3"), "deve mostrar o limite de WIP do quadro");
        verificar(c.quadrosTela.chamadas == 1 && c.quadrosTela.ultimoId == "QDR001", "deve abrir o quadro QDR001");
    }
    // Projeto inexistente: não abre nada
    {
        Cenario c;
        c.servicoProjetos.existe = false;
        std::string tela = c.rodar("2\n2\nPRJ001\n0\n0\n");
        verificar(contem(tela, "Projeto nao encontrado."), "deve avisar quando o projeto nao existe");
        verificar(c.quadrosTela.chamadas == 0, "nao pode abrir quadro de projeto inexistente");
    }
    // Projeto sem quadros: não pede ID de quadro
    {
        Cenario c;
        std::string tela = c.rodar("2\n2\nPRJ001\n0\n0\n");
        verificar(contem(tela, "ainda nao possui quadros"), "deve avisar que nao ha quadros");
        verificar(c.quadrosTela.chamadas == 0, "nao pode abrir quadro quando nao ha quadros");
    }
    std::cout << "  [OK] Teste Unitario: Abrir quadro (com quadros, projeto inexistente e sem quadros).\n";
}

void TUApresentacaoControle::testarIdentificadorInvalido() {
    Cenario c;
    // "XX" nao e um Identificador valido (3 letras + 3 digitos): o erro deve ser tratado na tela.
    std::string tela = c.rodar("2\n2\nXX\n0\n0\n");

    verificar(contem(tela, "ERRO DE ENTRADA"), "identificador invalido deve gerar mensagem de erro de entrada");
    verificar(c.quadrosTela.chamadas == 0, "identificador invalido nao pode abrir quadro");
    std::cout << "  [OK] Teste Unitario: Identificador invalido e tratado sem encerrar o programa.\n";
}

void TUApresentacaoControle::testarSessaoAposGerenciarConta() {
    // Conta alterada: o menu deve refletir o novo nome
    {
        Cenario c;
        c.servicoPessoas.pessoaAtual = Pessoa(Email("usuario@unb.br"), Nome("Novo Nome"), Senha("A1b2c"), Papel(Papel::GESTOR));
        std::string tela = c.rodar("2\n4\n0\n0\n");
        verificar(c.pessoas.gerenciarChamadas == 1, "opcao 4 deve acionar o modulo de Pessoas (gerenciar conta)");
        verificar(contem(tela, "Novo Nome (GESTOR)"), "sessao deve refletir o nome atualizado");
    }
    // Conta excluida: a sessao e encerrada e volta para a tela inicial
    {
        Cenario c;
        c.servicoPessoas.existe = false;
        std::string tela = c.rodar("2\n4\n0\n");
        verificar(contem(tela, "Sessao encerrada"), "conta excluida deve encerrar a sessao");
    }
    std::cout << "  [OK] Teste Unitario: Sessao acompanha alteracao e exclusao da conta.\n";
}

void TUApresentacaoControle::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DO CONTROLE   \n";
    std::cout << "============================================\n";
    testarModulosIndisponiveis();
    testarCriarContaELogin();
    testarLoginFalho();
    testarOpcaoInvalida();
    testarPermissaoDesenvolvedor();
    testarExclusaoDeProjeto();
    testarAbrirQuadro();
    testarIdentificadorInvalido();
    testarSessaoAposGerenciarConta();
    std::cout << ">>> TODOS OS TESTES DO CONTROLE PASSARAM COM SUCESSO! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DO MÓDULO DE PESSOAS
// =============================================================================

void TUServicoPessoas::testarCriarELerPessoa() {
    Email email("pessoa.tu@unb.br");
    ContainerPessoas::getInstancia()->removerPessoa(email);

    Pessoa nova(email, Nome("Maria Silva"), Senha("A1b2c"), Papel(Papel::GESTOR));
    verificar(servicoPessoas.criar(nova) == true, "criar pessoa inedita deve retornar true");
    verificar(ContainerPessoas::getInstancia()->existePessoa(email) == true, "existePessoa deve retornar true");

    // Tentativa de criar com o mesmo e-mail (chave primária duplicada) -> std::runtime_error
    bool lancouDuplicado = false;
    try {
        servicoPessoas.criar(nova);
    } catch (const std::runtime_error&) {
        lancouDuplicado = true;
    }
    verificar(lancouDuplicado, "criar pessoa com e-mail duplicado deve lancar std::runtime_error");

    // Leitura com sucesso
    Pessoa lida;
    verificar(servicoPessoas.ler(email, lida) == true, "ler pessoa existente deve retornar true");
    verificar(lida.getNome().getNome() == "Maria Silva", "nome lido deve corresponder ao cadastrado");
    verificar(lida.getPapel().getPapel() == Papel::GESTOR, "papel lido deve corresponder ao cadastrado");

    // Leitura de e-mail inexistente -> deve retornar false (sem lancar excecao)
    Pessoa inexistente;
    verificar(servicoPessoas.ler(Email("inexistente.tu@unb.br"), inexistente) == false,
              "ler pessoa inexistente deve retornar false");

    ContainerPessoas::getInstancia()->removerPessoa(email);
    std::cout << "  [OK] Teste Unitario: ServicoPessoas::criar e ServicoPessoas::ler (sucesso e contrato de retorno).\n";
}

void TUServicoPessoas::testarAtualizarPessoa() {
    Email email("atualizar.tu@unb.br");
    ContainerPessoas::getInstancia()->removerPessoa(email);

    Pessoa original(email, Nome("Joao Original"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR));
    servicoPessoas.criar(original);

    Pessoa alterada(email, Nome("Joao Novo"), Senha("Z9y8x"), Papel(Papel::GESTOR));
    verificar(servicoPessoas.atualizar(alterada) == true, "atualizar pessoa existente deve retornar true");

    Pessoa conferida;
    servicoPessoas.ler(email, conferida);
    verificar(conferida.getNome().getNome() == "Joao Novo", "nome deve ter sido atualizado");
    verificar(conferida.getSenha().getSenha() == "Z9y8x", "senha deve ter sido atualizada");
    verificar(conferida.getPapel().getPapel() == Papel::GESTOR, "papel deve ter sido atualizado");

    // Atualizar pessoa inexistente -> std::runtime_error
    bool lancouInexistente = false;
    try {
        Pessoa fantasma(Email("fantasma.tu@unb.br"), Nome("Fantasma"), Senha("A1b2c"), Papel(Papel::GESTOR));
        servicoPessoas.atualizar(fantasma);
    } catch (const std::runtime_error&) {
        lancouInexistente = true;
    }
    verificar(lancouInexistente, "atualizar pessoa inexistente deve lancar std::runtime_error");

    // Gestor com projetos vinculados tenta virar DESENVOLVEDOR
    Email emailGestor("gestor.projetos.tu@unb.br");
    Identificador idProjVinculado("PRJ789");
    ContainerProjetos::getInstancia()->removerProjeto(idProjVinculado);
    ContainerPessoas::getInstancia()->removerPessoa(emailGestor);

    Pessoa gestorProj(emailGestor, Nome("Gestor ComProj"), Senha("A1b2c"), Papel(Papel::GESTOR));
    servicoPessoas.criar(gestorProj);

    Projeto projVinculado(idProjVinculado, Nome("Proj Gestao"), Texto("Descricao valida."),
                          Timestamp("01-JAN-2024-10:00"), Timestamp("30-JAN-2024-18:00"));
    ContainerProjetos::getInstancia()->inserirProjeto(projVinculado, emailGestor);

    bool bloqueouRebaixamento = false;
    try {
        Pessoa gestorRebaixado(emailGestor, Nome("Gestor ComProj"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR));
        servicoPessoas.atualizar(gestorRebaixado);
    } catch (const std::runtime_error&) {
        bloqueouRebaixamento = true;
    }
    verificar(bloqueouRebaixamento, "alterar papel de GESTOR para DESENVOLVEDOR com projetos ativos deve lancar std::runtime_error");

    // Limpeza
    ContainerProjetos::getInstancia()->removerProjeto(idProjVinculado);
    ContainerPessoas::getInstancia()->removerPessoa(emailGestor);
    ContainerPessoas::getInstancia()->removerPessoa(email);

    std::cout << "  [OK] Teste Unitario: ServicoPessoas::atualizar (sucesso, excecao inexistente e integridade gestor->dev).\n";
}

void TUServicoPessoas::testarListarPessoas() {
    Email e1("lista1.tu@unb.br");
    Email e2("lista2.tu@unb.br");
    ContainerPessoas::getInstancia()->removerPessoa(e1);
    ContainerPessoas::getInstancia()->removerPessoa(e2);

    servicoPessoas.criar(Pessoa(e1, Nome("Pessoa Um"), Senha("A1b2c"), Papel(Papel::GESTOR)));
    servicoPessoas.criar(Pessoa(e2, Nome("Pessoa Dois"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR)));

    std::vector<Pessoa> lista = servicoPessoas.listar();
    bool achou1 = false;
    bool achou2 = false;
    for (const auto& p : lista) {
        if (p.getEmail().getEmail() == e1.getEmail()) achou1 = true;
        if (p.getEmail().getEmail() == e2.getEmail()) achou2 = true;
    }
    verificar(achou1 && achou2, "listar() deve retornar todas as pessoas cadastradas");

    ContainerPessoas::getInstancia()->removerPessoa(e1);
    ContainerPessoas::getInstancia()->removerPessoa(e2);
    std::cout << "  [OK] Teste Unitario: ServicoPessoas::listar retorna as entidades cadastradas.\n";
}

void TUServicoPessoas::testarExcluirPessoaEIntegridade() {
    Email emailGestor("excluir.tu@unb.br");
    Identificador idProj("PRJ654");
    ContainerProjetos::getInstancia()->removerProjeto(idProj);
    ContainerPessoas::getInstancia()->removerPessoa(emailGestor);

    Pessoa gestor(emailGestor, Nome("Gestor Excluir"), Senha("A1b2c"), Papel(Papel::GESTOR));
    servicoPessoas.criar(gestor);

    // Vincula um projeto ao gestor em ContainerProjetos -> exclusão deve ser bloqueada na camada de serviço (Requisito 15)
    Projeto proj(idProj, Nome("Proj Vinculo"), Texto("Projeto ativo."),
                 Timestamp("01-JAN-2024-10:00"), Timestamp("30-JAN-2024-18:00"));
    ContainerProjetos::getInstancia()->inserirProjeto(proj, emailGestor);

    bool bloqueouPorProjeto = false;
    try {
        servicoPessoas.excluir(emailGestor);
    } catch (const std::runtime_error&) {
        bloqueouPorProjeto = true;
    }
    verificar(bloqueouPorProjeto, "excluir pessoa com projeto vinculado deve lancar std::runtime_error");

    // Remove o projeto vinculado e agora exclui a pessoa com sucesso
    ContainerProjetos::getInstancia()->removerProjeto(idProj);
    verificar(servicoPessoas.excluir(emailGestor) == true, "excluir pessoa sem vinculos deve retornar true");
    verificar(ContainerPessoas::getInstancia()->existePessoa(emailGestor) == false, "existePessoa deve retornar false apos exclusao");

    // Tentar excluir novamente -> std::runtime_error
    bool lancouInexistente = false;
    try {
        servicoPessoas.excluir(emailGestor);
    } catch (const std::runtime_error&) {
        lancouInexistente = true;
    }
    verificar(lancouInexistente, "excluir pessoa inexistente deve lancar std::runtime_error");

    std::cout << "  [OK] Teste Unitario: ServicoPessoas::excluir e bloqueio de integridade referencial.\n";
}

void TUServicoPessoas::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE PESSOAS    \n";
    std::cout << "             (CAMADA DE SERVICO)            \n";
    std::cout << "============================================\n";
    testarCriarELerPessoa();
    testarAtualizarPessoa();
    testarListarPessoas();
    testarExcluirPessoaEIntegridade();
    std::cout << ">>> TODOS OS TESTES DE SERVICO DE PESSOAS PASSARAM! <<<\n\n";
}

namespace {

class MockServicoPessoasCompleto : public IServicoPessoas {
public:
    bool falharNegocio = false;
    int chamadasCriar = 0;
    int chamadasLer = 0;
    int chamadasAtualizar = 0;
    int chamadasExcluir = 0;
    int chamadasListar = 0;
    Pessoa ultimaPessoa;
    std::vector<Pessoa> lista;

    bool criar(const Pessoa& pessoa) override {
        ++chamadasCriar;
        if (falharNegocio) throw std::runtime_error("E-mail ja cadastrado.");
        ultimaPessoa = pessoa;
        return true;
    }
    bool ler(const Email&, Pessoa& pessoa) override {
        ++chamadasLer;
        if (falharNegocio) return false;
        pessoa = ultimaPessoa;
        return true;
    }
    bool atualizar(const Pessoa& pessoa) override {
        ++chamadasAtualizar;
        if (falharNegocio) throw std::runtime_error("Falha ao atualizar pessoa.");
        ultimaPessoa = pessoa;
        return true;
    }
    bool excluir(const Email&) override {
        ++chamadasExcluir;
        if (falharNegocio) throw std::runtime_error("Pessoa possui vinculos ativos.");
        return true;
    }
    std::vector<Pessoa> listar() override {
        ++chamadasListar;
        return lista;
    }
};

} // namespace

void TUApresentacaoPessoas::testarServicoIndisponivel() {
    ApresentacaoPessoas apresentacao; // setServico não foi chamado (nullptr)

    bool lancouCriar = false;
    try {
        apresentacao.criarConta();
    } catch (const std::runtime_error&) {
        lancouCriar = true;
    }
    verificar(lancouCriar, "criarConta() sem servico injetado deve lancar std::runtime_error");

    bool lancouGerenciar = false;
    try {
        Pessoa u(Email("teste@unb.br"), Nome("Teste"), Senha("A1b2c"), Papel(Papel::GESTOR));
        apresentacao.gerenciarConta(u);
    } catch (const std::runtime_error&) {
        lancouGerenciar = true;
    }
    verificar(lancouGerenciar, "gerenciarConta() sem servico injetado deve lancar std::runtime_error");

    std::cout << "  [OK] Teste Unitario: ApresentacaoPessoas lanca runtime_error se o servico estiver indisponivel.\n";
}

void TUApresentacaoPessoas::testarCriarContaValidaEInvalida() {
    MockServicoPessoasCompleto mock;
    ApresentacaoPessoas apresentacao;
    apresentacao.setServico(&mock);

    // 1. Entrada válida -> cria conta e retorna true
    {
        CapturaES cap("novo.usuario@unb.br\nAna Clara\nA1b2c\nGESTOR\n");
        bool ok = apresentacao.criarConta();
        std::string tela = cap.texto();
        verificar(ok == true, "criarConta() com dados validos deve retornar true");
        verificar(mock.chamadasCriar == 1, "servico->criar deve ser chamado uma vez");
        verificar(contem(tela, "SUCESSO"), "tela deve exibir mensagem de sucesso");
    }

    // 2. Entrada com formato inválido (senha inválida) -> captura invalid_argument e retorna false
    {
        CapturaES cap("novo.usuario@unb.br\nAna Clara\n12345\nGESTOR\n");
        bool ok = apresentacao.criarConta();
        std::string tela = cap.texto();
        verificar(ok == false, "criarConta() com senha sem letras deve retornar false");
        verificar(mock.chamadasCriar == 1, "servico->criar nao deve ser chamado quando dominio falha");
        verificar(contem(tela, "ERRO DE ENTRADA"), "tela deve informar erro de formato invalido");
    }

    // 3. Erro de negócio no serviço (e-mail duplicado) -> captura runtime_error e retorna false
    {
        mock.falharNegocio = true;
        CapturaES cap("novo.usuario@unb.br\nAna Clara\nA1b2c\nGESTOR\n");
        bool ok = apresentacao.criarConta();
        std::string tela = cap.texto();
        verificar(ok == false, "criarConta() quando servico lanca runtime_error deve retornar false");
        verificar(contem(tela, "ERRO DE NEGOCIO"), "tela deve informar erro de negocio");
    }

    std::cout << "  [OK] Teste Unitario: ApresentacaoPessoas::criarConta (sucesso, erro de formato e erro de negocio).\n";
}

void TUApresentacaoPessoas::testarGerenciarContaLerAtualizarListar() {
    MockServicoPessoasCompleto mock;
    ApresentacaoPessoas apresentacao;
    apresentacao.setServico(&mock);

    Pessoa logado(Email("user@unb.br"), Nome("User Antigo"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR));
    mock.ultimaPessoa = logado;
    mock.lista = { logado };

    // Roteiro: 1 (Ler) -> 2 (Atualizar para "User Novo", "B2c3d", "GESTOR") -> 4 (Listar) -> 0 (Voltar)
    std::string roteiro = "1\n2\nUser Novo\nB2c3d\nGESTOR\n4\n0\n";
    CapturaES cap(roteiro);
    apresentacao.gerenciarConta(logado);
    std::string tela = cap.texto();

    verificar(mock.chamadasLer == 1, "opcao 1 deve chamar servico->ler");
    verificar(mock.chamadasAtualizar == 1, "opcao 2 deve chamar servico->atualizar");
    verificar(mock.chamadasListar == 1, "opcao 4 deve chamar servico->listar");
    verificar(logado.getNome().getNome() == "User Novo", "objeto usuarioLogado deve ser atualizado em sessao");
    verificar(logado.getPapel().getPapel() == Papel::GESTOR, "papel do usuarioLogado deve ser atualizado");
    verificar(contem(tela, "PESSOAS CADASTRADAS (1)"), "listagem deve exibir o total de pessoas");
    verificar(contem(tela, "user@unb.br"), "listagem deve exibir o identificador (Email) da pessoa");

    std::cout << "  [OK] Teste Unitario: ApresentacaoPessoas::gerenciarConta (Ler, Atualizar e Listar).\n";
}

void TUApresentacaoPessoas::testarGerenciarContaExcluir() {
    MockServicoPessoasCompleto mock;
    ApresentacaoPessoas apresentacao;
    apresentacao.setServico(&mock);

    Pessoa logado(Email("user@unb.br"), Nome("User Teste"), Senha("A1b2c"), Papel(Papel::GESTOR));

    // Cancela exclusão (N) e depois confirma (S) -> encerra o menu automaticamente após excluir
    CapturaES cap("3\nN\n3\nS\n");
    apresentacao.gerenciarConta(logado);
    std::string tela = cap.texto();

    verificar(contem(tela, "Exclusao cancelada."), "confirmacao N deve cancelar a exclusao");
    verificar(mock.chamadasExcluir == 1, "confirmacao S deve acionar servico->excluir uma vez");
    verificar(contem(tela, "Conta excluida com sucesso"), "deve informar sucesso na exclusao da conta");

    std::cout << "  [OK] Teste Unitario: ApresentacaoPessoas::gerenciarConta (Excluir cancelado e confirmado).\n";
}

void TUApresentacaoPessoas::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE PESSOAS    \n";
    std::cout << "          (CAMADA DE APRESENTACAO)          \n";
    std::cout << "============================================\n";
    testarServicoIndisponivel();
    testarCriarContaValidaEInvalida();
    testarGerenciarContaLerAtualizarListar();
    testarGerenciarContaExcluir();
    std::cout << ">>> TODOS OS TESTES DE APRESENTACAO DE PESSOAS PASSARAM! <<<\n\n";
}