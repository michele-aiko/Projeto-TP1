#include "testeFumaca.hpp"
#include "containerDados.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

// =============================================================================
// FERRAMENTAS COMUNS DOS TESTES (visíveis apenas neste arquivo)
// =============================================================================
namespace {

const std::string SENHA = "A1b2c";

// Teclas digitadas na tela de login (ApresentacaoAutenticacao): e-mail e depois senha.
std::string roteiroLogin(const std::string& email, const std::string& senha) {
    return email + "\n" + senha + "\n";
}

void verificar(bool condicao, const std::string& descricao) {
    if (!condicao) {
        std::cout.rdbuf(std::cout.rdbuf());
        std::cerr.rdbuf(std::cerr.rdbuf());
        std::cerr << "\n [FALHA] " << descricao << "\n";
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

void cadastrarPessoa(const std::string& email, const std::string& nome, const std::string& papel) {
    ContainerPessoas* pessoas = ContainerPessoas::getInstancia();
    pessoas->removerPessoa(Email(email));
    pessoas->inserirPessoa(Pessoa(Email(email), Nome(nome), Senha(SENHA), Papel(papel)));
}

// Apaga o projeto (e seus quadros e cartões), caso um teste anterior tenha deixado sobras.
void apagarProjetoSeExistir(IServicoProjetos& servico, const Identificador& id) {
    Projeto existente;
    if (servico.ler(id, existente)) servico.excluir(id);
}

} // namespace

// =============================================================================
// TESTE FUMAÇA DO MÓDULO DE INICIALIZAÇÃO E CONTROLE
// =============================================================================

void TFumacaControle::executar(ApresentacaoControle& controle) {
    std::cout << "\n============================================\n";
    std::cout << "   EXECUTANDO TESTE FUMACA DO CONTROLE      \n";
    std::cout << "============================================\n";

    ContainerPessoas* pessoas = ContainerPessoas::getInstancia();
    ContainerProjetos* projetos = ContainerProjetos::getInstancia();
    ContainerQuadros* quadros = ContainerQuadros::getInstancia();

    // 1. Massa de dados exclusiva deste teste (identificadores que os outros testes nao usam)
    cadastrarPessoa("fumaca.gestor@unb.br", "Gestor Fumaca", Papel::GESTOR);
    cadastrarPessoa("fumaca.dev@unb.br", "Dev Fumaca", Papel::DESENVOLVEDOR);
    pessoas->removerPessoa(Email("fumaca.novo@unb.br"));
    projetos->removerProjeto(Identificador("PRJ777"));
    quadros->removerQuadro(Identificador("QDR777"));

    Projeto projeto(Identificador("PRJ777"), Nome("Projeto Fumaca"), Texto("Descricao valida."),
                    Timestamp("01-JAN-2026-10:00"), Timestamp("30-JAN-2026-18:00"));
    projetos->inserirProjeto(projeto, Email("fumaca.gestor@unb.br"));
    quadros->inserirQuadro(Quadro(Identificador("QDR777"), Nome("Quadro Fumaca"), Limite(3)), projeto.getIdentificador());

    // 2. Criar conta pela tela inicial e fazer login com a conta nova
    {
        std::string roteiro = "1\nfumaca.novo@unb.br\nNovo Fumaca\nA1b2c\nDESENVOLVEDOR\n"  // criar conta
                            + std::string("2\n") + roteiroLogin("fumaca.novo@unb.br", SENHA) // login
                            + "0\n"                                                          // sair da conta
                            + "0\n";                                                         // sair do sistema
        std::string tela = rodar(controle, roteiro);

        verificar(contem(tela, "Conta criada com sucesso"), "[conta] deve confirmar a criacao da conta");
        verificar(contem(tela, "MENU PRINCIPAL"), "[conta] login da conta nova deve abrir o menu principal");
        verificar(contem(tela, "Novo Fumaca (DESENVOLVEDOR)"), "[conta] menu deve mostrar nome e papel da conta nova");
        std::cout << "  [OK] Fumaca: Criar conta pela tela inicial e entrar com ela.\n";
    }

    // 3. Desenvolvedor: login, lista projetos, tenta excluir (negado) e sai
    {
        std::string roteiro = "2\n" + roteiroLogin("fumaca.dev@unb.br", SENHA)
                            + "1\n1\n0\n"   // menu de projetos -> listar todos -> voltar
                            + "3\n"         // excluir projeto: deve ser negado
                            + "0\n"         // sair da conta
                            + "0\n";        // sair do sistema
        std::string tela = rodar(controle, roteiro);

        verificar(contem(tela, "MENU PRINCIPAL"), "[dev] login deve abrir o menu principal");
        verificar(contem(tela, "(DESENVOLVEDOR)"), "[dev] menu deve mostrar o papel DESENVOLVEDOR");
        verificar(contem(tela, "PRJ777"), "[dev] a listagem deve mostrar o projeto PRJ777");
        verificar(contem(tela, "Apenas gestores podem excluir projetos!"), "[dev] exclusao deve ser negada");

        Projeto lido;
        verificar(projetos->buscarProjeto(projeto.getIdentificador(), lido), "[dev] o projeto deve continuar existindo");
        std::cout << "  [OK] Fumaca: Desenvolvedor faz login, lista projetos e e impedido de excluir.\n";
    }

    // 4. Gestor: login, abre quadros do projeto, exclui o projeto (cascata) e sai
    {
        std::string roteiro = "2\n" + roteiroLogin("fumaca.gestor@unb.br", SENHA)
                            + "2\nPRJ777\n"            // abrir quadros: escolhe o projeto
                            + "1\nQDR777\n0\n"         // listar/abrir quadro QDR777 e voltar do quadro
                            + "0\n"                    // voltar do menu de quadros
                            + "3\nPRJ777\nS\n"         // excluir projeto, confirmando
                            + "0\n"                    // sair da conta
                            + "0\n";                   // sair do sistema
        std::string tela = rodar(controle, roteiro);

        verificar(contem(tela, "(GESTOR)"), "[gestor] menu deve mostrar o papel GESTOR");
        verificar(contem(tela, "PROJETOS CADASTRADOS") && contem(tela, "PRJ777"), "[gestor] deve listar o projeto PRJ777");
        verificar(contem(tela, "QUADROS DISPONIVEIS") && contem(tela, "QDR777"), "[gestor] deve listar o quadro QDR777");
        verificar(contem(tela, "excluidos em cascata"), "[gestor] deve confirmar a exclusao do projeto");

        Projeto lido;
        verificar(!projetos->buscarProjeto(projeto.getIdentificador(), lido), "[gestor] o projeto deve ter sido removido");
        verificar(!quadros->existeQuadro(Identificador("QDR777")), "[gestor] o quadro deve ter sido removido em cascata");
        std::cout << "  [OK] Fumaca: Gestor lista projetos, abre quadro, exclui projeto em cascata e sai.\n";
    }

    // 5. Login recusado: senha errada e e-mail inexistente nao podem abrir o menu principal
    {
        std::string tela = rodar(controle, "2\n" + roteiroLogin("fumaca.dev@unb.br", "Zz9zZ") + "0\n");
        verificar(!contem(tela, "MENU PRINCIPAL"), "[login] senha errada nao pode abrir o menu principal");
        verificar(contem(tela, "Login nao realizado"), "[login] deve avisar que o login falhou");

        tela = rodar(controle, "2\n" + roteiroLogin("naoexiste.fumaca@unb.br", SENHA) + "0\n");
        verificar(!contem(tela, "MENU PRINCIPAL"), "[login] e-mail inexistente nao pode abrir o menu principal");
        std::cout << "  [OK] Fumaca: Login com senha errada ou e-mail inexistente e recusado.\n";
    }

    // 6. Limpeza da massa de dados
    pessoas->removerPessoa(Email("fumaca.gestor@unb.br"));
    pessoas->removerPessoa(Email("fumaca.dev@unb.br"));
    pessoas->removerPessoa(Email("fumaca.novo@unb.br"));

    std::cout << ">>> TESTE FUMACA DO CONTROLE CONCLUIDO COM SUCESSO! <<<\n\n";
}

// =============================================================================
// TESTE FUMAÇA DO MÓDULO DE PESSOAS (REQUISITOS FUNCIONAIS 1 A 5)
// =============================================================================

void TFumacaPessoas::executar(IApresentacaoPessoas& apresentacaoPessoas) {
    std::cout << "\n============================================\n";
    std::cout << "     EXECUTANDO TESTE FUMACA DE PESSOAS     \n";
    std::cout << "============================================\n";

    ContainerPessoas* containerPessoas = ContainerPessoas::getInstancia();
    Email emailTeste("fumaca.pessoa@unb.br");
    containerPessoas->removerPessoa(emailTeste);

    // RF 1: CRIAR PESSOA
    {
        CapturaES captura("fumaca.pessoa@unb.br\nPessoa Fumaca\nA1b2c\nDESENVOLVEDOR\n");
        bool criada = apresentacaoPessoas.criarConta();
        std::string tela = captura.texto();
        verificar(criada == true, "[RF 1 - Criar Pessoa] criarConta() deve retornar true");
        verificar(contem(tela, "SUCESSO"), "[RF 1 - Criar Pessoa] deve confirmar cadastro com sucesso");
        Pessoa persistida;
        verificar(containerPessoas->buscarPessoa(emailTeste, persistida), "[RF 1 - Criar Pessoa] pessoa deve estar no ContainerPessoas");
    }

    std::cout << " [OK] Fumaca (RF 1 - CRIAR PESSOA): Conta criada e persistida com sucesso.\n";

    // RF 2 (LER), RF 3 (ATUALIZAR), RF 5 (LISTAR) e RF 4 (EXCLUIR)
    {
        Pessoa sessao;
        containerPessoas->buscarPessoa(emailTeste, sessao);
        std::string roteiro = "1\n2\nPessoa Editada\nZ9y8x\nGESTOR\n4\n3\nS\n";
        CapturaES captura(roteiro);
        apresentacaoPessoas.gerenciarConta(sessao);
        std::string tela = captura.texto();

        verificar(contem(tela, "DADOS DA PESSOA") && contem(tela, "Pessoa Fumaca"), "[RF 2 - Ler Pessoa] deve exibir os dados");
        verificar(contem(tela, "Dados da conta atualizados com sucesso"), "[RF 3 - Atualizar Pessoa] deve confirmar atualizacao");
        verificar(contem(tela, "PESSOAS CADASTRADAS"), "[RF 5 - Listar Pessoa] deve listar as pessoas");
        verificar(contem(tela, "Conta excluida com sucesso"), "[RF 4 - Excluir Pessoa] deve confirmar exclusao");
    }

    std::cout << " [OK] Fumaca (RF 2 - LER PESSOA): Dados da conta consultados e exibidos com sucesso.\n";
    std::cout << " [OK] Fumaca (RF 3 - ATUALIZAR PESSOA): Nome, Senha e Papel atualizados com sucesso.\n";
    std::cout << " [OK] Fumaca (RF 5 - LISTAR PESSOA): Listagem de pessoas exibida com sucesso.\n";
    std::cout << " [OK] Fumaca (RF 4 - EXCLUIR PESSOA): Conta removida com sucesso.\n";

    std::cout << ">>> TESTE FUMACA DE PESSOAS CONCLUIDO COM SUCESSO! <<<\n\n";
}


// =============================================================================
// TESTE FUMAÇA DO MÓDULO DE PROJETOS (REQUISITOS FUNCIONAIS 6 A 10)
// =============================================================================

void TFumacaProjetos::executar(IApresentacaoProjetos& apresentacao, IServicoProjetos& servico) {
    std::cout << "\n============================================\n";
    std::cout << " EXECUTANDO TESTE FUMACA DE PROJETOS \n";
    std::cout << "============================================\n";

    ContainerPessoas* pessoas = ContainerPessoas::getInstancia();
    cadastrarPessoa("fumaca.gp@unb.br", "Gestor Projeto", Papel::GESTOR);
    cadastrarPessoa("fumaca.dp@unb.br", "Dev Projeto", Papel::DESENVOLVEDOR);

    Pessoa gestor(Email("fumaca.gp@unb.br"), Nome("Gestor Projeto"), Senha(SENHA), Papel(Papel::GESTOR));
    Pessoa dev(Email("fumaca.dp@unb.br"), Nome("Dev Projeto"), Senha(SENHA), Papel(Papel::DESENVOLVEDOR));

    const Identificador id("PRJ701");
    apagarProjetoSeExistir(servico, id);

    // RF 6: CRIAR PROJETO pela tela
    {
        CapturaES captura("3\nPRJ701\nProjeto\nDescricao valida.\n01-JAN-2026-10:00\n30-JUN-2026-18:00\n0\n");
        apresentacao.executar(gestor);
        verificar(contem(captura.texto(), "SUCESSO"), "[RF 6 - Criar Projeto] deve confirmar a criacao");
        Projeto criado;
        verificar(servico.ler(id, criado), "[RF 6 - Criar Projeto] projeto deve existir apos a criacao");
    } // <- Fecha a CapturaES AQUI!

    std::cout << " [OK] Fumaca (RF 6 - CRIAR PROJETO): Projeto criado pelo gestor.\n";

    // RF 10: LISTAR PROJETO pela tela
    {
        CapturaES captura("1\n0\n");
        apresentacao.executar(dev);
        verificar(contem(captura.texto(), "PRJ701"), "[RF 10 - Listar Projeto] deve listar o identificador PRJ701");
    } // <- Fecha a CapturaES AQUI!

    std::cout << " [OK] Fumaca (RF 10 - LISTAR PROJETO): Listagem exibida para o desenvolvedor.\n";

    // RF 7: LER PROJETO (via servico)
    Projeto lido;
    verificar(servico.ler(id, lido), "[RF 7 - Ler Projeto] deve localizar o projeto");
    std::cout << " [OK] Fumaca (RF 7 - LER PROJETO): Dados do projeto lidos (via servico).\n";

    // RF 8: ATUALIZAR PROJETO (via servico)
    lido.setNome(Nome("Projeto Editado"));
    verificar(servico.atualizar(lido), "[RF 8 - Atualizar Projeto] deve retornar true");
    std::cout << " [OK] Fumaca (RF 8 - ATUALIZAR PROJETO): Projeto atualizado (via servico).\n";

    // RF 9: EXCLUIR PROJETO (via servico)
    verificar(servico.excluir(id), "[RF 9 - Excluir Projeto] deve retornar true");
    std::cout << " [OK] Fumaca (RF 9 - EXCLUIR PROJETO): Projeto removido (via servico).\n";

    pessoas->removerPessoa(gestor.getEmail());
    pessoas->removerPessoa(dev.getEmail());

    std::cout << ">>> TESTE FUMACA DE PROJETOS CONCLUIDO COM SUCESSO! <<<\n\n";
}

// =============================================================================
// TESTE FUMAÇA DO MÓDULO DE QUADROS E CARTÕES (REQUISITOS FUNCIONAIS 11 A 26)
// =============================================================================

void TFumacaQuadros::executar(IApresentacaoQuadros& apresentacao, IServicoQuadros& servico, IServicoProjetos& servicoProjetos) {
    std::cout << "\n============================================\n";
    std::cout << "    EXECUTANDO TESTE FUMACA DE QUADROS      \n";
    std::cout << "============================================\n";

    cadastrarPessoa("fumaca.gq@unb.br", "Gestor Quadro", Papel::GESTOR);
    Pessoa gestor(Email("fumaca.gq@unb.br"), Nome("Gestor Quadro"), Senha(SENHA), Papel(Papel::GESTOR));
    const Identificador idProjeto("PRJ702");
    const Identificador idQuadro("QDR702");
    const Identificador cartao1("ACT701");
    const Identificador cartao2("ACT702");
    const Identificador cartao3("ACT703");

    apagarProjetoSeExistir(servicoProjetos, idProjeto);
    servicoProjetos.criar(Projeto(idProjeto, Nome("Projeto Quadros"), Texto("Descricao valida."),
                                  Timestamp("01-NOV-2026-08:00"), Timestamp("30-NOV-2026-18:00")), gestor.getEmail());

    // RF 11: CRIAR QUADRO pela tela (limite de WIP = 1 para testar o bloqueio depois)
    {
        CapturaES captura("2\nQDR702\nSprint\n1\n0\n");
        apresentacao.gerenciarQuadros(idProjeto, gestor);
        verificar(contem(captura.texto(), "SUCESSO"), "[RF 11 - Criar Quadro] deve confirmar a criacao");
        std::cout << "  [OK] Fumaca (RF 11 - CRIAR QUADRO): Quadro criado com limite de WIP = 1.\n";
    }

    // RF 15: LISTAR QUADRO pela tela (que tambem abre o quadro, mostrando seus dados: RF 12)
    {
        CapturaES captura("1\nQDR702\n0\n0\n");
        apresentacao.gerenciarQuadros(idProjeto, gestor);
        std::string tela = captura.texto();
        verificar(contem(tela, "QDR702") && contem(tela, "Sprint"), "[RF 15 - Listar Quadro] deve listar o identificador e o nome");
        verificar(contem(tela, "0 / 1"), "[RF 12 - Ler Quadro] deve exibir WIP e limite do quadro");
        verificar(servico.listarQuadrosPorProjeto(idProjeto).size() == 1, "[RF 15 - Listar Quadro] servico deve listar 1 quadro");
        std::cout << "  [OK] Fumaca (RF 15 - LISTAR QUADRO): Quadro listado e aberto pela tela.\n";
    }

    // RF 12: LER QUADRO (via servico)
    {
        Quadro lido;
        verificar(servico.lerQuadro(idQuadro, lido), "[RF 12 - Ler Quadro] deve localizar o quadro");
        verificar(lido.getNome().getNome() == "Sprint" && lido.getLimite().getLimite() == 1, "[RF 12 - Ler Quadro] dados devem corresponder");
        std::cout << "  [OK] Fumaca (RF 12 - LER QUADRO): Dados do quadro lidos (via servico).\n";
    }

    // RF 16: CRIAR CARTAO DE ATIVIDADE pela tela (gestor)
    {
        CapturaES captura("2\nACT701\nTarefa\nALTA\nPEQUENO\n01-NOV-2026-09:00\n"
                          "2\nACT702\nOutra\nMEDIA\nGRANDE\n01-NOV-2026-09:00\n"
                          "0\n");
        apresentacao.exibirQuadro(idQuadro, gestor);

        CartaoDeAtividade lido;
        verificar(servico.lerCartao(cartao1, lido) && servico.lerCartao(cartao2, lido), "[RF 16 - Criar Cartao] os 2 cartoes devem existir");
        verificar(lido.getEstado().getEstado() == Estado::A_FAZER, "[RF 16 - Criar Cartao] cartao novo deve estar em A FAZER");
        std::cout << "  [OK] Fumaca (RF 16 - CRIAR CARTAO): Dois cartoes criados em A FAZER.\n";
    }

    // RF 21 (MOVER) e RF 26 (WIP): o segundo cartao deve ser bloqueado pelo limite de WIP
    {
        CapturaES captura("1\nACT701\nFAZENDO\n01-NOV-2026-10:00\n"
                          "1\nACT702\nFAZENDO\n01-NOV-2026-11:00\n"
                          "0\n");
        apresentacao.exibirQuadro(idQuadro, gestor);
        std::string tela = captura.texto();

        verificar(contem(tela, "SUCESSO"), "[RF 21 - Mover Cartao] o primeiro movimento deve funcionar");
        verificar(contem(tela, "BLOQUEIO"), "[RF 21 - Mover Cartao] o segundo movimento deve ser bloqueado pelo limite de WIP");
        verificar(servico.lerWip(idQuadro) == 1, "[RF 26 - Ler WIP] WIP deve ser 1 (limite do quadro)");
        std::cout << "  [OK] Fumaca (RF 21 - MOVER CARTAO): A FAZER -> FAZENDO funciona e o limite de WIP bloqueia o excedente.\n";
        std::cout << "  [OK] Fumaca (RF 26 - LER WIP): WIP = 1 conferido pelo servico e pela tela.\n";
    }

    // Conclui o primeiro cartao, executa o segundo ate o fim e consulta as metricas pela tela (RF 23 e RF 25)
    {
        CapturaES captura("1\nACT701\nFEITO\n01-NOV-2026-12:30\n"
                          "1\nACT702\nFAZENDO\n02-NOV-2026-09:00\n"
                          "1\nACT702\nFEITO\n02-NOV-2026-10:00\n"
                          "3\n0\n");
        apresentacao.exibirQuadro(idQuadro, gestor);
        std::string tela = captura.texto();

        verificar(servico.lerWip(idQuadro) == 0, "[RF 26 - Ler WIP] WIP deve ser 0 com tudo concluido");
        verificar(contem(tela, "ACT701") && contem(tela, "ACT702"), "[RF 20 - Listar Cartao] tela deve listar os cartoes do quadro");
        verificar(contem(tela, "Tempo de Ciclo Medio: 105"), "[RF 23 - Tempo de Ciclo Medio] tela deve exibir 105 minutos");
        verificar(contem(tela, "Lead Time Medio: 855"), "[RF 25 - Lead Time Medio] tela deve exibir 855 minutos");
        std::cout << "  [OK] Fumaca (RF 20 - LISTAR CARTAO): Cartoes exibidos nas colunas do quadro.\n";
        std::cout << "  [OK] Fumaca (RF 23 - TEMPO DE CICLO MEDIO): 105 minutos exibidos pela tela.\n";
        std::cout << "  [OK] Fumaca (RF 25 - LEAD TIME MEDIO): 855 minutos exibidos pela tela.\n";
    }

    // RF 22 e RF 24: tempo de ciclo e lead time por cartao (via servico)
    {
        verificar(servico.lerTempoDeCiclo(cartao1) == 150 && servico.lerTempoDeCiclo(cartao2) == 60,
                  "[RF 22 - Tempo de Ciclo] deve ser 150 e 60 minutos");
        verificar(servico.lerLeadTime(cartao1) == 210 && servico.lerLeadTime(cartao2) == 1500,
                  "[RF 24 - Lead Time] deve ser 210 e 1500 minutos");
        verificar(servico.lerTempoDeCicloMedio(idQuadro) == 105.0 && servico.lerLeadTimeMedio(idQuadro) == 855.0,
                  "[RF 23/25] medias do servico devem ser 105 e 855");
        std::cout << "  [OK] Fumaca (RF 22 - TEMPO DE CICLO): 150 e 60 minutos (via servico).\n";
        std::cout << "  [OK] Fumaca (RF 24 - LEAD TIME): 210 e 1500 minutos (via servico).\n";
    }

    // RF 17: LER CARTAO, RF 18: ATUALIZAR CARTAO e RF 19: EXCLUIR CARTAO (via servico)
    {
        CartaoDeAtividade lido;
        verificar(servico.lerCartao(cartao1, lido), "[RF 17 - Ler Cartao] deve localizar o cartao");
        verificar(lido.getNome().getNome() == "Tarefa" && lido.getEstado().getEstado() == Estado::FEITO, "[RF 17 - Ler Cartao] dados devem corresponder");
        std::cout << "  [OK] Fumaca (RF 17 - LER CARTAO): Dados do cartao lidos (via servico).\n";

        lido.setNome(Nome("Tarefa Editada"));
        lido.setPrioridade(Prioridade(Prioridade::BAIXA));
        verificar(servico.atualizarCartao(lido), "[RF 18 - Atualizar Cartao] deve retornar true");
        CartaoDeAtividade conferido;
        servico.lerCartao(cartao1, conferido);
        verificar(conferido.getNome().getNome() == "Tarefa Editada" && conferido.getPrioridade().getPrioridade() == "BAIXA",
                  "[RF 18 - Atualizar Cartao] nome e prioridade devem ter sido alterados");
        verificar(conferido.getEstado().getEstado() == Estado::FEITO, "[RF 18 - Atualizar Cartao] estado nao pode mudar");
        std::cout << "  [OK] Fumaca (RF 18 - ATUALIZAR CARTAO): Cartao atualizado sem alterar o estado (via servico).\n";

        const Timestamp t("03-NOV-2026-09:00");
        servico.criarCartao(CartaoDeAtividade(cartao3, Nome("Descartavel"), Texto("Descricao valida."), Prioridade(Prioridade::BAIXA),
                                              Estado(Estado::A_FAZER), Tamanho(Tamanho::PEQUENO), t, t, t), idQuadro);
        verificar(servico.excluirCartao(cartao3), "[RF 19 - Excluir Cartao] deve retornar true");
        verificar(!servico.lerCartao(cartao3, lido), "[RF 19 - Excluir Cartao] cartao nao deve mais existir");
        std::cout << "  [OK] Fumaca (RF 19 - EXCLUIR CARTAO): Cartao removido (via servico).\n";
    }

    // RF 13: ATUALIZAR QUADRO e RF 14: EXCLUIR QUADRO (via servico; exclusao remove os cartoes)
    {
        verificar(servico.atualizarQuadro(Quadro(idQuadro, Nome("Sprint Final"), Limite(2))), "[RF 13 - Atualizar Quadro] deve retornar true");
        Quadro conferido;
        servico.lerQuadro(idQuadro, conferido);
        verificar(conferido.getNome().getNome() == "Sprint Final" && conferido.getLimite().getLimite() == 2,
                  "[RF 13 - Atualizar Quadro] nome e limite devem ter sido alterados");
        std::cout << "  [OK] Fumaca (RF 13 - ATUALIZAR QUADRO): Quadro atualizado (via servico).\n";

        verificar(servico.excluirQuadro(idQuadro), "[RF 14 - Excluir Quadro] deve retornar true");
        verificar(!servico.lerQuadro(idQuadro, conferido), "[RF 14 - Excluir Quadro] quadro nao deve mais existir");
        CartaoDeAtividade orfao;
        verificar(!servico.lerCartao(cartao1, orfao), "[RF 14 - Excluir Quadro] cartoes do quadro devem ser removidos");
        std::cout << "  [OK] Fumaca (RF 14 - EXCLUIR QUADRO): Quadro e cartoes removidos (via servico).\n";
    }

    servicoProjetos.excluir(idProjeto);
    ContainerPessoas::getInstancia()->removerPessoa(gestor.getEmail());
    std::cout << ">>> TESTE FUMACA DE QUADROS CONCLUIDO COM SUCESSO! <<<\n\n";
}

