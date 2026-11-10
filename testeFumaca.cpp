#include "testeFumaca.hpp"
#include "containerDados.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

// =============================================================================
// TESTE FUMAÇA DO MÓDULO DE INICIALIZAÇÃO E CONTROLE 
// =============================================================================

namespace {

// ---------------------------------------------------------------------------
// Verificar dependencia de outros modulos no final do projeto 
// ---------------------------------------------------------------------------
const char* const SAIR_DO_QUADRO = "0\n";   // tecla que sai da tela do quadro (ApresentacaoQuadros)
const char* const SAIR_DE_PROJETOS = "0\n"; // tecla que volta do menu de projetos (ApresentacaoProjetos)

// Teclas digitadas na tela de login (ApresentacaoAutenticacao): e-mail e depois senha.
std::string roteiroLogin(const std::string& email, const std::string& senha) {
    return email + "\n" + senha + "\n";
}

void verificar(bool condicao, const std::string& descricao) {
    if (!condicao) {
        std::cerr << "  [FALHA] " << descricao << "\n";
        std::exit(1);
    }
}

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
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

std::string rodar(ApresentacaoControle& controle, const std::string& roteiro) {
    CapturaES captura(roteiro);
    controle.executar();
    return captura.texto();
}

} // namespace

void TFumacaControle::executar(ApresentacaoControle& controle) {
    std::cout << "\n============================================\n";
    std::cout << "   EXECUTANDO TESTE FUMACA DO CONTROLE      \n";
    std::cout << "============================================\n";

    ContainerDados* container = ContainerDados::getInstancia();

    // 1. Massa de dados exclusiva do teste fumaça (identificadores que os outros testes não usam)
    const std::string senha = "A1b2c";
    Pessoa gestor(Email("fumaca.gestor@unb.br"), Nome("Gestor Fumaca"), Senha(senha), Papel(Papel::GESTOR));
    Pessoa dev(Email("fumaca.dev@unb.br"), Nome("Dev Fumaca"), Senha(senha), Papel(Papel::DESENVOLVEDOR));
    Projeto projeto(Identificador("PRJ777"), Nome("Projeto Fumaca"), Texto("Descricao valida."),
                    Timestamp("01-JAN-2024-10:00"), Timestamp("30-JAN-2024-18:00"));
    Quadro quadro(Identificador("QDR777"), Nome("Quadro Fumaca"), Limite(3));

    container->inserirPessoa(gestor);
    container->inserirPessoa(dev);
    container->inserirProjeto(projeto, gestor.getEmail());
    container->inserirQuadro(quadro, projeto.getIdentificador());

    // 2. Desenvolvedor: faz login, consulta projetos, tenta excluir (negado) e sai
    {
        std::string roteiro = "2\n" + roteiroLogin("fumaca.dev@unb.br", senha)
                            + "1\n" + SAIR_DE_PROJETOS   // menu de projetos
                            + "3\n"                      // excluir projeto: deve ser negado
                            + "0\n"                      // sair da conta
                            + "0\n";                     // sair do sistema
        std::string tela = rodar(controle, roteiro);

        verificar(contem(tela, "MENU PRINCIPAL"), "[dev] login deve abrir o menu principal");
        verificar(contem(tela, "(DESENVOLVEDOR)"), "[dev] menu deve mostrar o papel DESENVOLVEDOR");
        verificar(contem(tela, "Apenas gestores podem excluir projetos!"), "[dev] exclusao deve ser negada");

        Projeto lido;
        verificar(container->buscarProjeto(projeto.getIdentificador(), lido), "[dev] o projeto deve continuar existindo");
        std::cout << "  [OK] Fumaca: Desenvolvedor faz login, consulta projetos e e impedido de excluir.\n";
    }

    // 3. Gestor: login, lista projetos, abre quadro, exclui projeto, logout
    {
        std::string roteiro = "2\n" + roteiroLogin("fumaca.gestor@unb.br", senha)
                            + "1\n" + SAIR_DE_PROJETOS   // menu de projetos
                            + "2\nPRJ777\nQDR777\n" + SAIR_DO_QUADRO // abrir quadro Kanban
                            + "3\nPRJ777\nS\n"           // excluir projeto, confirmando
                            + "0\n"                      // sair da conta
                            + "0\n";                     // sair do sistema
        std::string tela = rodar(controle, roteiro);

        verificar(contem(tela, "(GESTOR)"), "[gestor] menu deve mostrar o papel GESTOR");
        verificar(contem(tela, "PRJ777"), "[gestor] a listagem deve mostrar o projeto PRJ777");
        verificar(contem(tela, "QUADROS DO PROJETO PRJ777"), "[gestor] deve listar os quadros do projeto");
        verificar(contem(tela, "QDR777"), "[gestor] deve listar o quadro QDR777");
        verificar(contem(tela, "Projeto excluido com sucesso"), "[gestor] deve confirmar a exclusao do projeto");

        Projeto lido;
        verificar(!container->buscarProjeto(projeto.getIdentificador(), lido), "[gestor] o projeto deve ter sido removido do container");
        std::cout << "  [OK] Fumaca: Gestor faz login, lista projetos, abre quadro, exclui projeto e sai.\n";
    }

    // 4. Login recusado: senha errada não pode abrir o menu principal
    {
        std::string roteiro = "2\n" + roteiroLogin("fumaca.dev@unb.br", "Zz9zZ") + "0\n";
        std::string tela = rodar(controle, roteiro);
        verificar(!contem(tela, "MENU PRINCIPAL"), "[login] senha errada nao pode abrir o menu principal");
        std::cout << "  [OK] Fumaca: Login com senha errada e recusado.\n";
    }

    // 5. Limpeza da massa de dados
    container->removerPessoa(gestor.getEmail());
    container->removerPessoa(dev.getEmail());
    container->removerProjeto(projeto.getIdentificador());

    std::cout << ">>> TESTE FUMACA DO CONTROLE CONCLUIDO COM SUCESSO! <<<\n\n";
}
