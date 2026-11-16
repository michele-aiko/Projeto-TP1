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

// =============================================================================
// TESTE FUMAÇA DO MÓDULO DE PESSOAS (REQUISITOS FUNCIONAIS 1 A 5)
// =============================================================================

void TFumacaPessoas::executar(ApresentacaoPessoas& apresentacaoPessoas) {
    std::cout << "\n============================================\n";
    std::cout << "    EXECUTANDO TESTE FUMACA DE PESSOAS      \n";
    std::cout << "============================================\n";

    ContainerPessoas* containerPessoas = ContainerPessoas::getInstancia();
    Email emailTeste("fumaca.pessoa@unb.br");
    containerPessoas->removerPessoa(emailTeste);

    // Caso de Sucesso RF 1: CRIAR PESSOA via ApresentacaoPessoas -> ServicoPessoas -> ContainerPessoas
    {
        CapturaES captura("fumaca.pessoa@unb.br\nPessoa Fumaca\nA1b2c\nDESENVOLVEDOR\n");
        bool criada = apresentacaoPessoas.criarConta();
        std::string tela = captura.texto();

        verificar(criada == true, "[RF 1 - Criar Pessoa] criarConta() deve retornar true");
        verificar(contem(tela, "SUCESSO"), "[RF 1 - Criar Pessoa] deve confirmar cadastro com sucesso");

        Pessoa persistida;
        verificar(containerPessoas->buscarPessoa(emailTeste, persistida),
                  "[RF 1 - Criar Pessoa] pessoa deve estar armazenada no ContainerPessoas");
        std::cout << "  [OK] Fumaca (RF 1 - CRIAR PESSOA): Conta criada e persistida com sucesso.\n";
    }

    // Casos de Sucesso RF 2 (LER), RF 3 (ATUALIZAR), RF 5 (LISTAR) e RF 4 (EXCLUIR)
    {
        Pessoa sessao;
        containerPessoas->buscarPessoa(emailTeste, sessao);

        // Roteiro no menu gerenciarConta:
        // 1 -> Ler Dados da Conta (RF 2)
        // 2 -> Atualizar Dados da Conta para "Pessoa Editada", "Z9y8x", "GESTOR" (RF 3)
        // 4 -> Listar Pessoas Cadastradas (RF 5)
        // 3 -> Excluir Minha Conta confirmando com "S" (RF 4)
        std::string roteiro = "1\n"
                              "2\nPessoa Editada\nZ9y8x\nGESTOR\n"
                              "4\n"
                              "3\nS\n";
        CapturaES captura(roteiro);
        apresentacaoPessoas.gerenciarConta(sessao);
        std::string tela = captura.texto();

        // Verificação RF 2 (LER PESSOA)
        verificar(contem(tela, "DADOS DA PESSOA") && contem(tela, "Pessoa Fumaca"),
                  "[RF 2 - Ler Pessoa] deve exibir os dados cadastrais da pessoa");
        std::cout << "  [OK] Fumaca (RF 2 - LER PESSOA): Dados da conta consultados e exibidos com sucesso.\n";

        // Verificação RF 3 (ATUALIZAR PESSOA)
        verificar(contem(tela, "Dados da conta atualizados com sucesso"),
                  "[RF 3 - Atualizar Pessoa] deve confirmar atualizacao cadastral");
        verificar(sessao.getNome().getNome() == "Pessoa Editada" && sessao.getPapel().getPapel() == Papel::GESTOR,
                  "[RF 3 - Atualizar Pessoa] sessao deve refletir o novo Nome e Papel");
        std::cout << "  [OK] Fumaca (RF 3 - ATUALIZAR PESSOA): Nome, Senha e Papel atualizados com sucesso.\n";

        // Verificação RF 5 (LISTAR PESSOA)
        verificar(contem(tela, "PESSOAS CADASTRADAS") && contem(tela, "fumaca.pessoa@unb.br"),
                  "[RF 5 - Listar Pessoa] deve listar o identificador (e-mail) das pessoas cadastradas");
        std::cout << "  [OK] Fumaca (RF 5 - LISTAR PESSOA): Listagem de pessoas exibida com sucesso.\n";

        // Verificação RF 4 (EXCLUIR PESSOA)
        verificar(contem(tela, "Conta excluida com sucesso"),
                  "[RF 4 - Excluir Pessoa] deve confirmar a exclusao da conta");
        verificar(!containerPessoas->existePessoa(emailTeste),
                  "[RF 4 - Excluir Pessoa] pessoa nao deve mais existir no ContainerPessoas");
        std::cout << "  [OK] Fumaca (RF 4 - EXCLUIR PESSOA): Conta removida com sucesso.\n";
    }

    std::cout << ">>> TESTE FUMACA DE PESSOAS CONCLUIDO COM SUCESSO! <<<\n\n";
}
