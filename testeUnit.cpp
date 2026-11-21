#include "testeUnit.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// =============================================================================
// FERRAMENTAS COMUNS DOS TESTES (visíveis apenas neste arquivo)
// =============================================================================

namespace {

static std::streambuf* g_realCerrBuf = std::cerr.rdbuf();

void verificar(bool condicao, const std::string& descricao) {
    if (!condicao) {
        std::cerr.rdbuf(g_realCerrBuf);
        std::cerr << "\n [FALHA NO TESTE UNITARIO]: " << descricao << "\n";
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

// Retorna true somente se a ação lançar exatamente a exceção esperada (ou uma derivada dela).
template <typename Excecao, typename Acao>
bool lanca(Acao acao) {
    try {
        acao();
    } catch (const Excecao&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

// Retorna true se o domínio aceitar o valor; false se lançar std::invalid_argument.
template <typename Dominio, typename Valor>
bool aceita(const Valor& valor) {
    try {
        Dominio dominio(valor);
        (void)dominio;
    } catch (const std::invalid_argument&) {
        return false;
    }
    return true;
}

template <typename Dominio, typename Valor>
bool rejeita(const Valor& valor) {
    return !aceita<Dominio>(valor);
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

// --- Dados de apoio ---------------------------------------------------------

const std::string SENHA_TU = "A1b2c";
const std::string EMAIL_GESTOR_TU = "gestor.serv.tu@unb.br";
const std::string EMAIL_DEV_TU = "dev.serv.tu@unb.br";

Pessoa pessoaMock(const std::string& papel) {
    return Pessoa(Email("usuario@unb.br"), Nome("Usuario Teste"), Senha(SENHA_TU), Papel(papel));
}

Projeto projetoMock(const std::string& id) {
    return Projeto(Identificador(id), Nome("Projeto Mock"), Texto("Descricao valida."),
                   Timestamp("01-JAN-2026-10:00"), Timestamp("30-JAN-2026-18:00"));
}

Quadro quadroMock(const std::string& id, int limite) {
    return Quadro(Identificador(id), Nome("Quadro Teste"), Limite(limite));
}

CartaoDeAtividade cartaoMock(const std::string& id, const std::string& estado) {
    const Timestamp t("01-NOV-2026-09:00");
    return CartaoDeAtividade(Identificador(id), Nome("Tarefa Mock"), Texto("Descricao valida."),
                             Prioridade(Prioridade::MEDIA), Estado(estado), Tamanho(Tamanho::MEDIO), t, t, t);
}

// Cadastra (ou recadastra) uma pessoa direto no contêiner.
void garantirPessoa(const std::string& email, const std::string& papel) {
    ContainerPessoas* c = ContainerPessoas::getInstancia();
    c->removerPessoa(Email(email));
    c->inserirPessoa(Pessoa(Email(email), Nome("Pessoa Teste"), Senha(SENHA_TU), Papel(papel)));
}

// Remove o projeto (com quadros e cartões) se ele existir, para o teste começar limpo.
void removerProjetoCompleto(const std::string& id) {
    Identificador ident(id);
    if (ContainerProjetos::getInstancia()->existeProjeto(ident)) {
        ServicoProjetos servico;
        servico.excluir(ident);
    }
}

// Deixa pronto um gestor e um projeto novo de propriedade dele.
void prepararProjeto(const std::string& idProjeto) {
    garantirPessoa(EMAIL_GESTOR_TU, Papel::GESTOR);
    removerProjetoCompleto(idProjeto);
    ServicoProjetos servico;
    servico.criar(projetoMock(idProjeto), Email(EMAIL_GESTOR_TU));
}

void limparProjeto(const std::string& idProjeto) {
    removerProjetoCompleto(idProjeto);
    ContainerPessoas::getInstancia()->removerPessoa(Email(EMAIL_GESTOR_TU));
}

// --- Módulos falsos (mocks) das interfaces abstratas ---------------------------

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
    int chamadasGerenciar = 0;
    int chamadasExibir = 0;
    std::string ultimoIdProjeto;
    void exibirQuadro(const Identificador&, const Pessoa&) override { ++chamadasExibir; }
    void gerenciarQuadros(const Identificador& idProjeto, const Pessoa&) override {
        ++chamadasGerenciar;
        ultimoIdProjeto = idProjeto.getIdentificador();
    }
    void setServico(IServicoQuadros*) override {}
};

class MockServicoAutenticacao : public IServicoAutenticacao {
public:
    bool aceitar = true;
    int chamadas = 0;
    Pessoa pessoaAutenticada;
    bool autenticar(const Email&, const Senha&, Pessoa& pessoa) override {
        ++chamadas;
        if (aceitar) pessoa = pessoaAutenticada;
        return aceitar;
    }
};

// Serviço de pessoas usado nos testes da controladora (sessão).
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

// Serviço de pessoas usado nos testes da tela de Pessoas (conta chamadas e simula erros).
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

class MockServicoProjetos : public IServicoProjetos {
public:
    bool existe = true;
    std::string erroCriacao;           // "", "negocio" ou "permissao"
    int criacoes = 0;
    int exclusoes = 0;
    int listagensTodos = 0;
    int listagensGestor = 0;
    std::vector<Projeto> projetos;

    bool criar(const Projeto&, const Email&) override {
        ++criacoes;
        if (erroCriacao == "negocio") throw std::runtime_error("Projeto duplicado.");
        if (erroCriacao == "permissao") throw std::logic_error("Permissao negada.");
        return true;
    }
    bool ler(const Identificador& id, Projeto& projeto) override {
        if (existe) projeto = projetoMock(id.getIdentificador());
        return existe;
    }
    bool atualizar(const Projeto&) override { return true; }
    bool excluir(const Identificador&) override { ++exclusoes; return existe; }
    std::vector<Projeto> listarPorGestor(const Email&) override { ++listagensGestor; return projetos; }
    std::vector<Projeto> listarTodos() override { ++listagensTodos; return projetos; }
};

class MockServicoQuadros : public IServicoQuadros {
public:
    std::vector<Quadro> quadros;
    std::vector<CartaoDeAtividade> cartoes;
    bool quadroExiste = true;
    bool falharNegocio = false;       // criarQuadro/criarCartao lançam runtime_error
    bool bloquearMovimento = false;   // moverCartao lança runtime_error (simula limite de WIP)
    int criacoesQuadro = 0;
    int criacoesCartao = 0;
    int movimentos = 0;
    std::string ultimoEstado;

    bool criarQuadro(const Quadro&, const Identificador&) override {
        ++criacoesQuadro;
        if (falharNegocio) throw std::runtime_error("Quadro duplicado.");
        return true;
    }
    bool lerQuadro(const Identificador&, Quadro& quadro) override {
        if (quadroExiste) quadro = Quadro(Identificador("QDR001"), Nome("Sprint Um"), Limite(3));
        return quadroExiste;
    }
    bool atualizarQuadro(const Quadro&) override { return true; }
    bool excluirQuadro(const Identificador&) override { return true; }
    std::vector<Quadro> listarQuadrosPorProjeto(const Identificador&) override { return quadros; }
    bool criarCartao(const CartaoDeAtividade&, const Identificador&) override {
        ++criacoesCartao;
        if (falharNegocio) throw std::runtime_error("Cartao duplicado.");
        return true;
    }
    bool lerCartao(const Identificador&, CartaoDeAtividade&) override { return true; }
    bool atualizarCartao(const CartaoDeAtividade&) override { return true; }
    bool excluirCartao(const Identificador&) override { return true; }
    std::vector<CartaoDeAtividade> listarCartoesPorQuadro(const Identificador&) override { return cartoes; }
    bool moverCartao(const Identificador&, const Estado& novoEstado, const Timestamp&) override {
        ++movimentos;
        ultimoEstado = novoEstado.getEstado();
        if (bloquearMovimento) throw std::runtime_error("Limite de WIP atingido.");
        return true;
    }
    int lerWip(const Identificador&) override { return 2; }
    int lerTempoDeCiclo(const Identificador&) override { return 150; }
    double lerTempoDeCicloMedio(const Identificador&) override { return 105.0; }
    int lerLeadTime(const Identificador&) override { return 210; }
    double lerLeadTimeMedio(const Identificador&) override { return 855.0; }
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
        Pessoa usuario(Email("usuario@unb.br"), Nome("Usuario Teste"), Senha(SENHA_TU), Papel(papel));
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

// =============================================================================
// TESTES UNITÁRIOS DOS DOMÍNIOS
// =============================================================================

void TUDominios::testarEmail() {
    verificar(aceita<Email>("usuario.teste@unb.br"), "Email: formato valido deve ser aceito");
    verificar(aceita<Email>(std::string(64, 'a') + "@unb.br"), "Email: parte local com 64 caracteres e valida");
    verificar(rejeita<Email>("usuario.unb.br"), "Email: sem '@' deve ser rejeitado");
    verificar(rejeita<Email>("@unb.br"), "Email: parte local vazia deve ser rejeitada");
    verificar(rejeita<Email>("a@"), "Email: dominio vazio deve ser rejeitado");
    verificar(rejeita<Email>("-a@unb.br"), "Email: nao pode iniciar com hifen");
    verificar(rejeita<Email>("a.@unb.br"), "Email: nao pode terminar com ponto");
    verificar(rejeita<Email>("a..b@unb.br"), "Email: ponto deve ser seguido por letra ou digito");
    verificar(rejeita<Email>("a b@unb.br"), "Email: espaco nao e permitido");
    verificar(rejeita<Email>(std::string(65, 'a') + "@unb.br"), "Email: parte local com 65 caracteres e invalida");

    Email email("a@b.com");
    verificar(lanca<std::invalid_argument>([&] { email.setEmail("invalido"); }), "Email: setEmail invalido deve lancar invalid_argument");
    verificar(email.getEmail() == "a@b.com", "Email: valor antigo deve ser mantido apos set invalido");
    email.setEmail("novo@b.com");
    verificar(email.getEmail() == "novo@b.com", "Email: set valido deve alterar o valor");
}

void TUDominios::testarEstado() {
    verificar(aceita<Estado>("A FAZER") && aceita<Estado>("FAZENDO") && aceita<Estado>("FEITO"), "Estado: tres valores validos");
    verificar(rejeita<Estado>("DOING"), "Estado: valor fora da lista deve ser rejeitado");
    verificar(rejeita<Estado>("fazendo"), "Estado: minusculas devem ser rejeitadas");
    Estado estado;
    verificar(estado.getEstado() == Estado::A_FAZER, "Estado: valor padrao e A FAZER");
    estado.setEstado(Estado::FEITO);
    verificar(estado.getEstado() == "FEITO", "Estado: set valido deve alterar o valor");
    verificar(Estado::getOpcoes().size() == 3, "Estado: deve haver 3 opcoes");
}

void TUDominios::testarIdentificador() {
    verificar(aceita<Identificador>("ABC123"), "Identificador: 3 letras + 3 digitos e valido");
    verificar(aceita<Identificador>("abc000"), "Identificador: letras minusculas sao validas");
    verificar(rejeita<Identificador>("AB1234"), "Identificador: apenas 2 letras iniciais e invalido");
    verificar(rejeita<Identificador>("ABCDEF"), "Identificador: sem digitos e invalido");
    verificar(rejeita<Identificador>("ABC12"), "Identificador: 5 caracteres e invalido");
    verificar(rejeita<Identificador>("ABC1234"), "Identificador: 7 caracteres e invalido");
    Identificador id("QDR001");
    verificar(lanca<std::invalid_argument>([&] { id.setIdentificador("X"); }), "Identificador: set invalido deve lancar");
    verificar(id.getIdentificador() == "QDR001", "Identificador: valor antigo mantido");
}

void TUDominios::testarLimite() {
    verificar(aceita<Limite>(1) && aceita<Limite>(25) && aceita<Limite>(10), "Limite: 1, 10 e 25 sao validos");
    verificar(rejeita<Limite>(0) && rejeita<Limite>(26) && rejeita<Limite>(-3), "Limite: 0, 26 e -3 sao invalidos");
    Limite limite;
    verificar(limite.getLimite() == 5, "Limite: valor padrao e 5");
    verificar(lanca<std::invalid_argument>([&] { limite.setLimite(30); }), "Limite: set invalido deve lancar");
    verificar(limite.getLimite() == 5, "Limite: valor antigo mantido");
}

void TUDominios::testarNome() {
    verificar(aceita<Nome>("Maria Silva"), "Nome: letras e espaco interno sao validos");
    verificar(aceita<Nome>("Abcdefghijklmno"), "Nome: 15 caracteres e valido");
    verificar(rejeita<Nome>("Abcdefghijklmnop"), "Nome: 16 caracteres e invalido");
    verificar(rejeita<Nome>(""), "Nome: vazio e invalido");
    verificar(rejeita<Nome>(" Maria"), "Nome: nao pode iniciar com espaco");
    verificar(rejeita<Nome>("Maria "), "Nome: nao pode terminar com espaco");
    verificar(rejeita<Nome>("Maria  Silva"), "Nome: espaco deve ser seguido por letra");
    verificar(rejeita<Nome>("Maria2"), "Nome: digito nao e permitido");
    Nome nome("Ana");
    verificar(lanca<std::invalid_argument>([&] { nome.setNome("Ana!"); }), "Nome: set invalido deve lancar");
    verificar(nome.getNome() == "Ana", "Nome: valor antigo mantido");
}

void TUDominios::testarPapel() {
    verificar(aceita<Papel>("GESTOR") && aceita<Papel>("DESENVOLVEDOR"), "Papel: GESTOR e DESENVOLVEDOR sao validos");
    verificar(rejeita<Papel>("ADMIN"), "Papel: valor fora da lista e invalido");
    Papel papel;
    verificar(papel.getPapel() == Papel::GESTOR, "Papel: valor padrao e GESTOR");
    papel.setPapel(Papel::DESENVOLVEDOR);
    verificar(papel.getPapel() == "DESENVOLVEDOR", "Papel: set valido altera o valor");
    verificar(Papel::getOpcoes().size() == 2, "Papel: deve haver 2 opcoes");
}

void TUDominios::testarPrioridade() {
    verificar(aceita<Prioridade>("ALTA") && aceita<Prioridade>("MEDIA") && aceita<Prioridade>("BAIXA"), "Prioridade: tres valores validos");
    verificar(rejeita<Prioridade>("URGENTE"), "Prioridade: valor fora da lista e invalido");
    Prioridade prioridade;
    verificar(prioridade.getPrioridade() == Prioridade::BAIXA, "Prioridade: valor padrao e BAIXA");
    prioridade.setPrioridade(Prioridade::ALTA);
    verificar(prioridade.getPrioridade() == "ALTA", "Prioridade: set valido altera o valor");
    verificar(Prioridade::getOpcoes().size() == 3, "Prioridade: deve haver 3 opcoes");
}

void TUDominios::testarSenha() {
    verificar(aceita<Senha>("A1b2c") && aceita<Senha>("1234a"), "Senha: 5 caracteres com letra e digito sao validos");
    verificar(rejeita<Senha>("abcde"), "Senha: sem digito e invalida");
    verificar(rejeita<Senha>("12345"), "Senha: sem letra e invalida");
    verificar(rejeita<Senha>("A1b2"), "Senha: 4 caracteres e invalida");
    verificar(rejeita<Senha>("A1b2c3"), "Senha: 6 caracteres e invalida");
    verificar(rejeita<Senha>("A1b2!"), "Senha: caractere especial e invalido");
    Senha senha("A1b2c");
    verificar(lanca<std::invalid_argument>([&] { senha.setSenha("abcde"); }), "Senha: set invalido deve lancar");
    verificar(senha.getSenha() == "A1b2c", "Senha: valor antigo mantido");
}

void TUDominios::testarTamanho() {
    verificar(aceita<Tamanho>("PEQUENO") && aceita<Tamanho>("MEDIO") && aceita<Tamanho>("GRANDE"), "Tamanho: tres valores validos");
    verificar(rejeita<Tamanho>("GIGANTE"), "Tamanho: valor fora da lista e invalido");
    Tamanho tamanho;
    verificar(tamanho.getTamanho() == Tamanho::PEQUENO, "Tamanho: valor padrao e PEQUENO");
    tamanho.setTamanho(Tamanho::GRANDE);
    verificar(tamanho.getTamanho() == "GRANDE", "Tamanho: set valido altera o valor");
    verificar(Tamanho::getOpcoes().size() == 3, "Tamanho: deve haver 3 opcoes");
}

void TUDominios::testarTexto() {
    verificar(aceita<Texto>("Descricao valida."), "Texto: maiuscula inicial e ponto final sao validos");
    verificar(aceita<Texto>("Ola, mundo: teste."), "Texto: pontuacao separada por letras e valida");
    verificar(aceita<Texto>("A" + std::string(28, 'b') + "."), "Texto: 30 caracteres e valido");
    verificar(rejeita<Texto>(std::string(30, 'A') + "."), "Texto: 31 caracteres e invalido");
    verificar(rejeita<Texto>("descricao."), "Texto: primeira letra deve ser maiuscula");
    verificar(rejeita<Texto>("Sem ponto final"), "Texto: deve terminar com ponto");
    verificar(rejeita<Texto>("Dois pontos.."), "Texto: pontuacao nao pode ser seguida de pontuacao");
    verificar(rejeita<Texto>("Com @ aqui."), "Texto: caractere especial e invalido");
    Texto texto("Texto um.");
    verificar(lanca<std::invalid_argument>([&] { texto.setTexto("invalido"); }), "Texto: set invalido deve lancar");
    verificar(texto.getTexto() == "Texto um.", "Texto: valor antigo mantido");
}

void TUDominios::testarTimestamp() {
    verificar(aceita<Timestamp>("15-JAN-2024-14:30"), "Timestamp: data com dia de 2 digitos e valida");
    verificar(aceita<Timestamp>("1-JAN-2024-00:00"), "Timestamp: dia de 1 digito e valido");
    verificar(aceita<Timestamp>("29-FEV-2024-23:59"), "Timestamp: 29/FEV em ano bissexto e valido");
    verificar(aceita<Timestamp>("31-DEZ-2099-23:59"), "Timestamp: limite superior do ano e valido");
    verificar(rejeita<Timestamp>("29-FEV-2023-10:00"), "Timestamp: 29/FEV em ano comum e invalido");
    verificar(rejeita<Timestamp>("31-ABR-2024-10:00"), "Timestamp: 31/ABR e invalido");
    verificar(rejeita<Timestamp>("32-JAN-2024-10:00"), "Timestamp: dia 32 e invalido");
    verificar(rejeita<Timestamp>("10-XXX-2024-10:00"), "Timestamp: mes desconhecido e invalido");
    verificar(rejeita<Timestamp>("10-JAN-1999-10:00"), "Timestamp: ano menor que 2000 e invalido");
    verificar(rejeita<Timestamp>("10-JAN-2100-10:00"), "Timestamp: ano maior que 2099 e invalido");
    verificar(rejeita<Timestamp>("10-JAN-2024-24:00"), "Timestamp: hora 24 e invalida");
    verificar(rejeita<Timestamp>("10-JAN-2024-10:60"), "Timestamp: minuto 60 e invalido");
    verificar(rejeita<Timestamp>("10-JAN-2024-1000"), "Timestamp: horario sem ':' e invalido");
    Timestamp ts;
    verificar(ts.getTimeStamp() == "01-JAN-2024-12:00", "Timestamp: valor padrao deve ser valido");
    verificar(lanca<std::invalid_argument>([&] { ts.setTimeStamp("lixo"); }), "Timestamp: set invalido deve lancar");
    verificar(ts.getTimeStamp() == "01-JAN-2024-12:00", "Timestamp: valor antigo mantido");
}

void TUDominios::executar() {
    std::cout << "\n============================================\n";
    std::cout << "    EXECUTANDO TESTES UNITARIOS DE DOMINIOS \n";
    std::cout << "============================================\n";
    testarEmail();        std::cout << "  [OK] Teste Unitario: dominio Email.\n";
    testarEstado();       std::cout << "  [OK] Teste Unitario: dominio Estado.\n";
    testarIdentificador(); std::cout << "  [OK] Teste Unitario: dominio Identificador.\n";
    testarLimite();       std::cout << "  [OK] Teste Unitario: dominio Limite.\n";
    testarNome();         std::cout << "  [OK] Teste Unitario: dominio Nome.\n";
    testarPapel();        std::cout << "  [OK] Teste Unitario: dominio Papel.\n";
    testarPrioridade();   std::cout << "  [OK] Teste Unitario: dominio Prioridade.\n";
    testarSenha();        std::cout << "  [OK] Teste Unitario: dominio Senha.\n";
    testarTamanho();      std::cout << "  [OK] Teste Unitario: dominio Tamanho.\n";
    testarTexto();        std::cout << "  [OK] Teste Unitario: dominio Texto.\n";
    testarTimestamp();    std::cout << "  [OK] Teste Unitario: dominio Timestamp.\n";
    std::cout << ">>> TODOS OS TESTES DE DOMINIOS PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DAS ENTIDADES
// =============================================================================

void TUEntidades::testarPessoa() {
    Pessoa padrao; // construtor padrao deve usar os valores padrao dos dominios
    verificar(padrao.getPapel().getPapel() == Papel::GESTOR, "Pessoa: papel padrao e GESTOR");

    Pessoa p(Email("ent@unb.br"), Nome("Ana Souza"), Senha("A1b2c"), Papel(Papel::GESTOR));
    verificar(p.getEmail().getEmail() == "ent@unb.br", "Pessoa: get Email");
    verificar(p.getNome().getNome() == "Ana Souza", "Pessoa: get Nome");
    verificar(p.getSenha().getSenha() == "A1b2c", "Pessoa: get Senha");
    verificar(p.getPapel().getPapel() == "GESTOR", "Pessoa: get Papel");

    p.setEmail(Email("novo@unb.br"));
    p.setNome(Nome("Bia Lima"));
    p.setSenha(Senha("Z9y8x"));
    p.setPapel(Papel(Papel::DESENVOLVEDOR));
    verificar(p.getEmail().getEmail() == "novo@unb.br" && p.getNome().getNome() == "Bia Lima" &&
              p.getSenha().getSenha() == "Z9y8x" && p.getPapel().getPapel() == "DESENVOLVEDOR",
              "Pessoa: set deve alterar todos os atributos");
}

void TUEntidades::testarProjeto() {
    Projeto padrao; // exige que Timestamp padrao seja valido
    verificar(padrao.getIdentificador().getIdentificador() == "ABC123", "Projeto: identificador padrao");

    Projeto p(Identificador("PRJ001"), Nome("Projeto Um"), Texto("Descricao valida."),
              Timestamp("01-JAN-2026-10:00"), Timestamp("30-JUN-2026-18:00"));
    verificar(p.getIdentificador().getIdentificador() == "PRJ001", "Projeto: get Identificador");
    verificar(p.getNome().getNome() == "Projeto Um", "Projeto: get Nome");
    verificar(p.getDescricao().getTexto() == "Descricao valida.", "Projeto: get Descricao");
    verificar(p.getInicio().getTimeStamp() == "01-JAN-2026-10:00", "Projeto: get Inicio");
    verificar(p.getTermino().getTimeStamp() == "30-JUN-2026-18:00", "Projeto: get Termino");

    p.setIdentificador(Identificador("PRJ002"));
    p.setNome(Nome("Projeto Dois"));
    p.setDescricao(Texto("Outra descricao."));
    p.setInicio(Timestamp("02-FEV-2026-08:00"));
    p.setTermino(Timestamp("03-DEZ-2026-17:30"));
    verificar(p.getIdentificador().getIdentificador() == "PRJ002" && p.getNome().getNome() == "Projeto Dois" &&
              p.getDescricao().getTexto() == "Outra descricao." && p.getInicio().getTimeStamp() == "02-FEV-2026-08:00" &&
              p.getTermino().getTimeStamp() == "03-DEZ-2026-17:30", "Projeto: set deve alterar todos os atributos");
}

void TUEntidades::testarQuadro() {
    Quadro padrao;
    verificar(padrao.getLimite().getLimite() == 5, "Quadro: limite padrao e 5");

    Quadro q(Identificador("QDR001"), Nome("Sprint Um"), Limite(3));
    verificar(q.getIdentificador().getIdentificador() == "QDR001", "Quadro: get Identificador");
    verificar(q.getNome().getNome() == "Sprint Um", "Quadro: get Nome");
    verificar(q.getLimite().getLimite() == 3, "Quadro: get Limite");

    q.setIdentificador(Identificador("QDR002"));
    q.setNome(Nome("Sprint Dois"));
    q.setLimite(Limite(25));
    verificar(q.getIdentificador().getIdentificador() == "QDR002" && q.getNome().getNome() == "Sprint Dois" &&
              q.getLimite().getLimite() == 25, "Quadro: set deve alterar todos os atributos");
}

void TUEntidades::testarCartao() {
    CartaoDeAtividade padrao;
    verificar(padrao.getEstado().getEstado() == Estado::A_FAZER, "Cartao: estado padrao e A FAZER");

    CartaoDeAtividade c(Identificador("ACT001"), Nome("Tarefa Um"), Texto("Descricao valida."),
                        Prioridade(Prioridade::ALTA), Estado(Estado::FAZENDO), Tamanho(Tamanho::GRANDE),
                        Timestamp("01-NOV-2026-09:00"), Timestamp("01-NOV-2026-10:00"), Timestamp("01-NOV-2026-12:00"));
    verificar(c.getIdentificador().getIdentificador() == "ACT001", "Cartao: get Identificador");
    verificar(c.getNome().getNome() == "Tarefa Um", "Cartao: get Nome");
    verificar(c.getDescricao().getTexto() == "Descricao valida.", "Cartao: get Descricao");
    verificar(c.getPrioridade().getPrioridade() == "ALTA", "Cartao: get Prioridade");
    verificar(c.getEstado().getEstado() == "FAZENDO", "Cartao: get Estado");
    verificar(c.getTamanho().getTamanho() == "GRANDE", "Cartao: get Tamanho");
    verificar(c.getEntrada().getTimeStamp() == "01-NOV-2026-09:00", "Cartao: get Entrada");
    verificar(c.getInicio().getTimeStamp() == "01-NOV-2026-10:00", "Cartao: get Inicio");
    verificar(c.getTermino().getTimeStamp() == "01-NOV-2026-12:00", "Cartao: get Termino");

    c.setIdentificador(Identificador("ACT002"));
    c.setNome(Nome("Tarefa Dois"));
    c.setDescricao(Texto("Nova descricao."));
    c.setPrioridade(Prioridade(Prioridade::BAIXA));
    c.setEstado(Estado(Estado::FEITO));
    c.setTamanho(Tamanho(Tamanho::PEQUENO));
    c.setEntrada(Timestamp("02-NOV-2026-09:00"));
    c.setInicio(Timestamp("02-NOV-2026-10:00"));
    c.setTermino(Timestamp("02-NOV-2026-11:00"));
    verificar(c.getIdentificador().getIdentificador() == "ACT002" && c.getNome().getNome() == "Tarefa Dois" &&
              c.getDescricao().getTexto() == "Nova descricao." && c.getPrioridade().getPrioridade() == "BAIXA" &&
              c.getEstado().getEstado() == "FEITO" && c.getTamanho().getTamanho() == "PEQUENO" &&
              c.getEntrada().getTimeStamp() == "02-NOV-2026-09:00" && c.getInicio().getTimeStamp() == "02-NOV-2026-10:00" &&
              c.getTermino().getTimeStamp() == "02-NOV-2026-11:00", "Cartao: set deve alterar todos os atributos");
}

void TUEntidades::executar() {
    std::cout << "\n============================================\n";
    std::cout << "    EXECUTANDO TESTES UNITARIOS DE ENTIDADES\n";
    std::cout << "============================================\n";
    testarPessoa();  std::cout << "  [OK] Teste Unitario: entidade Pessoa.\n";
    testarProjeto(); std::cout << "  [OK] Teste Unitario: entidade Projeto.\n";
    testarQuadro();  std::cout << "  [OK] Teste Unitario: entidade Quadro.\n";
    testarCartao();  std::cout << "  [OK] Teste Unitario: entidade CartaoDeAtividade.\n";
    std::cout << ">>> TODOS OS TESTES DE ENTIDADES PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DO SERVIÇO DE AUTENTICAÇÃO
// =============================================================================

void TUServicoAutenticacao::testarLoginValido() {
    garantirPessoa("auth.tu@unb.br", Papel::DESENVOLVEDOR);

    Pessoa logada;
    verificar(servico.autenticar(Email("auth.tu@unb.br"), Senha(SENHA_TU), logada) == true, "credenciais corretas devem retornar true");
    verificar(logada.getEmail().getEmail() == "auth.tu@unb.br", "pessoa deve ser preenchida com a conta autenticada");
    verificar(logada.getPapel().getPapel() == Papel::DESENVOLVEDOR, "papel da conta deve ser preservado");

    ContainerPessoas::getInstancia()->removerPessoa(Email("auth.tu@unb.br"));
    std::cout << "  [OK] Teste Unitario: ServicoAutenticacao::autenticar com credenciais validas.\n";
}

void TUServicoAutenticacao::testarLoginInvalido() {
    garantirPessoa("auth.tu@unb.br", Papel::GESTOR);

    Pessoa intacta;
    bool lancou = false;
    bool resultado = true;
    try {
        resultado = servico.autenticar(Email("auth.tu@unb.br"), Senha("Z9y8x"), intacta);
    } catch (...) {
        lancou = true;
    }
    verificar(!lancou && resultado == false, "senha errada deve retornar false sem lancar excecao");
    verificar(intacta.getEmail().getEmail() == "parte-local@dominio", "pessoa nao pode ser alterada em login recusado");

    Pessoa outra;
    verificar(servico.autenticar(Email("naoexiste.tu@unb.br"), Senha(SENHA_TU), outra) == false, "e-mail inexistente deve retornar false");

    ContainerPessoas::getInstancia()->removerPessoa(Email("auth.tu@unb.br"));
    std::cout << "  [OK] Teste Unitario: ServicoAutenticacao::autenticar com senha errada e e-mail inexistente.\n";
}

void TUServicoAutenticacao::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE AUTENTICACAO\n";
    std::cout << "             (CAMADA DE SERVICO)            \n";
    std::cout << "============================================\n";
    testarLoginValido();
    testarLoginInvalido();
    std::cout << ">>> TODOS OS TESTES DE SERVICO DE AUTENTICACAO PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DO SERVIÇO DE PESSOAS
// =============================================================================

void TUServicoPessoas::testarCriarELerPessoa() {
    Email email("pessoa.tu@unb.br");
    ContainerPessoas::getInstancia()->removerPessoa(email);

    Pessoa nova(email, Nome("Maria Silva"), Senha("A1b2c"), Papel(Papel::GESTOR));
    verificar(servicoPessoas.criar(nova) == true, "criar pessoa inedita deve retornar true");
    verificar(ContainerPessoas::getInstancia()->existePessoa(email) == true, "existePessoa deve retornar true");
    verificar(lanca<std::runtime_error>([&] { servicoPessoas.criar(nova); }), "criar pessoa com e-mail duplicado deve lancar std::runtime_error");

    Pessoa lida;
    verificar(servicoPessoas.ler(email, lida) == true, "ler pessoa existente deve retornar true");
    verificar(lida.getNome().getNome() == "Maria Silva", "nome lido deve corresponder ao cadastrado");
    verificar(lida.getPapel().getPapel() == Papel::GESTOR, "papel lido deve corresponder ao cadastrado");

    Pessoa inexistente;
    verificar(servicoPessoas.ler(Email("inexistente.tu@unb.br"), inexistente) == false, "ler pessoa inexistente deve retornar false");

    ContainerPessoas::getInstancia()->removerPessoa(email);
    std::cout << "  [OK] Teste Unitario: ServicoPessoas::criar e ServicoPessoas::ler (sucesso e contrato de retorno).\n";
}

void TUServicoPessoas::testarAtualizarPessoa() {
    Email email("atualizar.tu@unb.br");
    ContainerPessoas::getInstancia()->removerPessoa(email);

    servicoPessoas.criar(Pessoa(email, Nome("Joao Original"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR)));

    Pessoa alterada(email, Nome("Joao Novo"), Senha("Z9y8x"), Papel(Papel::GESTOR));
    verificar(servicoPessoas.atualizar(alterada) == true, "atualizar pessoa existente deve retornar true");

    Pessoa conferida;
    servicoPessoas.ler(email, conferida);
    verificar(conferida.getNome().getNome() == "Joao Novo", "nome deve ter sido atualizado");
    verificar(conferida.getSenha().getSenha() == "Z9y8x", "senha deve ter sido atualizada");
    verificar(conferida.getPapel().getPapel() == Papel::GESTOR, "papel deve ter sido atualizado");

    verificar(lanca<std::runtime_error>([&] {
        servicoPessoas.atualizar(Pessoa(Email("fantasma.tu@unb.br"), Nome("Fantasma"), Senha("A1b2c"), Papel(Papel::GESTOR)));
    }), "atualizar pessoa inexistente deve lancar std::runtime_error");

    // Gestor com projeto vinculado nao pode virar DESENVOLVEDOR (Requisito 15)
    Email emailGestor("gestor.projetos.tu@unb.br");
    Identificador idProj("PRJ789");
    ContainerProjetos::getInstancia()->removerProjeto(idProj);
    ContainerPessoas::getInstancia()->removerPessoa(emailGestor);

    servicoPessoas.criar(Pessoa(emailGestor, Nome("Gestor ComProj"), Senha("A1b2c"), Papel(Papel::GESTOR)));
    ContainerProjetos::getInstancia()->inserirProjeto(projetoMock("PRJ789"), emailGestor);

    verificar(lanca<std::runtime_error>([&] {
        servicoPessoas.atualizar(Pessoa(emailGestor, Nome("Gestor ComProj"), Senha("A1b2c"), Papel(Papel::DESENVOLVEDOR)));
    }), "alterar papel de GESTOR para DESENVOLVEDOR com projetos ativos deve lancar std::runtime_error");

    ContainerProjetos::getInstancia()->removerProjeto(idProj);
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

    bool achou1 = false;
    bool achou2 = false;
    for (const auto& p : servicoPessoas.listar()) {
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

    servicoPessoas.criar(Pessoa(emailGestor, Nome("Gestor Excluir"), Senha("A1b2c"), Papel(Papel::GESTOR)));
    ContainerProjetos::getInstancia()->inserirProjeto(projetoMock("PRJ654"), emailGestor);

    verificar(lanca<std::runtime_error>([&] { servicoPessoas.excluir(emailGestor); }),
              "excluir pessoa com projeto vinculado deve lancar std::runtime_error");

    ContainerProjetos::getInstancia()->removerProjeto(idProj);
    verificar(servicoPessoas.excluir(emailGestor) == true, "excluir pessoa sem vinculos deve retornar true");
    verificar(ContainerPessoas::getInstancia()->existePessoa(emailGestor) == false, "existePessoa deve retornar false apos exclusao");
    verificar(lanca<std::runtime_error>([&] { servicoPessoas.excluir(emailGestor); }),
              "excluir pessoa inexistente deve lancar std::runtime_error");

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

// =============================================================================
// TESTES UNITÁRIOS DO SERVIÇO DE PROJETOS
// =============================================================================

void TUServicoProjetos::testarCriarProjeto() {
    removerProjetoCompleto("PRJ901");
    removerProjetoCompleto("PRJ902");
    garantirPessoa(EMAIL_GESTOR_TU, Papel::GESTOR);
    garantirPessoa(EMAIL_DEV_TU, Papel::DESENVOLVEDOR);

    // Cenario de sucesso
    verificar(servico.criar(projetoMock("PRJ901"), Email(EMAIL_GESTOR_TU)) == true, "gestor deve criar projeto com sucesso");
    verificar(ContainerProjetos::getInstancia()->existeProjeto(Identificador("PRJ901")), "projeto criado deve estar no contêiner");

    // Excecao de permissao
    verificar(lanca<std::logic_error>([&] { servico.criar(projetoMock("PRJ902"), Email(EMAIL_DEV_TU)); }),
              "desenvolvedor criando projeto deve lancar std::logic_error");
    verificar(!ContainerProjetos::getInstancia()->existeProjeto(Identificador("PRJ902")), "projeto negado nao pode ser gravado");

    // Excecao de ID duplicado
    verificar(lanca<std::runtime_error>([&] { servico.criar(projetoMock("PRJ901"), Email(EMAIL_GESTOR_TU)); }),
              "identificador duplicado deve lancar std::runtime_error");

    // Excecao de gestor inexistente
    verificar(lanca<std::runtime_error>([&] { servico.criar(projetoMock("PRJ902"), Email("fantasma.proj.tu@unb.br")); }),
              "gestor inexistente deve lancar std::runtime_error");

    removerProjetoCompleto("PRJ901");
    ContainerPessoas::getInstancia()->removerPessoa(Email(EMAIL_GESTOR_TU));
    ContainerPessoas::getInstancia()->removerPessoa(Email(EMAIL_DEV_TU));
    std::cout << "  [OK] Teste Unitario: ServicoProjetos::criar (sucesso, permissao, duplicidade e gestor inexistente).\n";
}

void TUServicoProjetos::testarLerEAtualizarProjeto() {
    prepararProjeto("PRJ903");

    Projeto lido;
    verificar(servico.ler(Identificador("PRJ903"), lido) == true, "ler projeto existente deve retornar true");
    verificar(lido.getNome().getNome() == "Projeto Mock", "nome lido deve corresponder ao cadastrado");
    verificar(servico.ler(Identificador("PRJ999"), lido) == false, "ler projeto inexistente deve retornar false");

    Projeto alterado = lido;
    alterado.setNome(Nome("Projeto Novo"));
    alterado.setDescricao(Texto("Nova descricao."));
    alterado.setTermino(Timestamp("31-DEZ-2026-18:00"));
    verificar(servico.atualizar(alterado) == true, "atualizar projeto existente deve retornar true");

    Projeto conferido;
    servico.ler(Identificador("PRJ903"), conferido);
    verificar(conferido.getNome().getNome() == "Projeto Novo", "nome deve ter sido atualizado");
    verificar(conferido.getDescricao().getTexto() == "Nova descricao.", "descricao deve ter sido atualizada");
    verificar(conferido.getTermino().getTimeStamp() == "31-DEZ-2026-18:00", "termino deve ter sido atualizado");
    verificar(conferido.getIdentificador().getIdentificador() == "PRJ903", "identificador (chave) nao muda");

    verificar(lanca<std::runtime_error>([&] { servico.atualizar(projetoMock("PRJ999")); }),
              "atualizar projeto inexistente deve lancar std::runtime_error");

    limparProjeto("PRJ903");
    std::cout << "  [OK] Teste Unitario: ServicoProjetos::ler e ServicoProjetos::atualizar.\n";
}

void TUServicoProjetos::testarListarProjetos() {
    prepararProjeto("PRJ904");
    servico.criar(projetoMock("PRJ905"), Email(EMAIL_GESTOR_TU));
    garantirPessoa(EMAIL_DEV_TU, Papel::DESENVOLVEDOR);

    verificar(servico.listarPorGestor(Email(EMAIL_GESTOR_TU)).size() == 2, "listarPorGestor deve retornar os 2 projetos do gestor");
    verificar(servico.listarPorGestor(Email(EMAIL_DEV_TU)).empty(), "listarPorGestor de quem nao gere projetos deve retornar vazio");

    bool achou4 = false;
    bool achou5 = false;
    for (const auto& p : servico.listarTodos()) {
        if (p.getIdentificador().getIdentificador() == "PRJ904") achou4 = true;
        if (p.getIdentificador().getIdentificador() == "PRJ905") achou5 = true;
    }
    verificar(achou4 && achou5, "listarTodos deve incluir os projetos criados");

    removerProjetoCompleto("PRJ905");
    limparProjeto("PRJ904");
    ContainerPessoas::getInstancia()->removerPessoa(Email(EMAIL_DEV_TU));
    std::cout << "  [OK] Teste Unitario: ServicoProjetos::listarPorGestor e ServicoProjetos::listarTodos.\n";
}

void TUServicoProjetos::testarExcluirProjetoEmCascata() {
    prepararProjeto("PRJ906");
    ServicoQuadros servicoQuadros;
    servicoQuadros.criarQuadro(quadroMock("QDR906", 3), Identificador("PRJ906"));
    servicoQuadros.criarCartao(cartaoMock("ACT906", Estado::A_FAZER), Identificador("QDR906"));

    verificar(servico.excluir(Identificador("PRJ906")) == true, "excluir projeto existente deve retornar true");
    verificar(!ContainerProjetos::getInstancia()->existeProjeto(Identificador("PRJ906")), "projeto deve ter sido removido");
    verificar(!ContainerQuadros::getInstancia()->existeQuadro(Identificador("QDR906")), "quadro do projeto deve ter sido removido em cascata");
    verificar(!ContainerCartoes::getInstancia()->existeCartao(Identificador("ACT906")), "cartao do quadro deve ter sido removido em cascata");
    verificar(lanca<std::runtime_error>([&] { servico.excluir(Identificador("PRJ906")); }),
              "excluir projeto inexistente deve lancar std::runtime_error");

    ContainerPessoas::getInstancia()->removerPessoa(Email(EMAIL_GESTOR_TU));
    std::cout << "  [OK] Teste Unitario: ServicoProjetos::excluir com cascata de quadros e cartoes.\n";
}

void TUServicoProjetos::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE PROJETOS   \n";
    std::cout << "             (CAMADA DE SERVICO)            \n";
    std::cout << "============================================\n";
    testarCriarProjeto();
    testarLerEAtualizarProjeto();
    testarListarProjetos();
    testarExcluirProjetoEmCascata();
    std::cout << ">>> TODOS OS TESTES DE SERVICO DE PROJETOS PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DO SERVIÇO DE QUADROS E CARTÕES
// =============================================================================

void TUServicoQuadros::testarQuadros() {
    prepararProjeto("PRJ910");
    const Identificador projeto("PRJ910");
    const Identificador idQuadro("QDR910");

    verificar(servico.criarQuadro(quadroMock("QDR910", 3), projeto) == true, "criar quadro valido deve retornar true");
    verificar(lanca<std::runtime_error>([&] { servico.criarQuadro(quadroMock("QDR910", 3), projeto); }),
              "quadro duplicado deve lancar std::runtime_error");
    verificar(lanca<std::runtime_error>([&] { servico.criarQuadro(quadroMock("QDR911", 3), Identificador("PRJ999")); }),
              "quadro em projeto inexistente deve lancar std::runtime_error");

    Quadro lido;
    verificar(servico.lerQuadro(idQuadro, lido) == true && lido.getLimite().getLimite() == 3, "ler quadro existente deve retornar true");
    verificar(servico.lerQuadro(Identificador("QDR999"), lido) == false, "ler quadro inexistente deve retornar false");

    Quadro alterado(idQuadro, Nome("Sprint Novo"), Limite(7));
    verificar(servico.atualizarQuadro(alterado) == true, "atualizar quadro deve retornar true");
    servico.lerQuadro(idQuadro, lido);
    verificar(lido.getNome().getNome() == "Sprint Novo" && lido.getLimite().getLimite() == 7, "nome e limite devem ter sido atualizados");
    verificar(lanca<std::runtime_error>([&] { servico.atualizarQuadro(quadroMock("QDR999", 3)); }),
              "atualizar quadro inexistente deve lancar std::runtime_error");

    verificar(servico.listarQuadrosPorProjeto(projeto).size() == 1, "listar deve retornar o quadro do projeto");
    verificar(servico.listarQuadrosPorProjeto(Identificador("PRJ999")).empty(), "projeto sem quadros deve retornar vazio");

    limparProjeto("PRJ910");
    std::cout << "  [OK] Teste Unitario: ServicoQuadros - criar, ler, atualizar e listar quadros.\n";
}

void TUServicoQuadros::testarCartoes() {
    prepararProjeto("PRJ912");
    servico.criarQuadro(quadroMock("QDR912", 3), Identificador("PRJ912"));
    const Identificador quadro("QDR912");
    const Identificador cartao("ACT912");

    // Mesmo informando FEITO, o cartao novo deve nascer em A FAZER
    verificar(servico.criarCartao(cartaoMock("ACT912", Estado::FEITO), quadro) == true, "criar cartao valido deve retornar true");
    CartaoDeAtividade lido;
    verificar(servico.lerCartao(cartao, lido) == true, "ler cartao existente deve retornar true");
    verificar(lido.getEstado().getEstado() == Estado::A_FAZER, "todo cartao novo deve nascer em A FAZER");
    verificar(servico.lerCartao(Identificador("ACT999"), lido) == false, "ler cartao inexistente deve retornar false");

    verificar(lanca<std::runtime_error>([&] { servico.criarCartao(cartaoMock("ACT912", Estado::A_FAZER), quadro); }),
              "cartao duplicado deve lancar std::runtime_error");
    verificar(lanca<std::runtime_error>([&] { servico.criarCartao(cartaoMock("ACT913", Estado::A_FAZER), Identificador("QDR999")); }),
              "cartao em quadro inexistente deve lancar std::runtime_error");

    // atualizarCartao nao pode mudar o estado (so moverCartao muda)
    CartaoDeAtividade alterado = lido;
    alterado.setNome(Nome("Tarefa Editada"));
    alterado.setPrioridade(Prioridade(Prioridade::ALTA));
    alterado.setEstado(Estado(Estado::FEITO));
    verificar(servico.atualizarCartao(alterado) == true, "atualizar cartao deve retornar true");
    servico.lerCartao(cartao, lido);
    verificar(lido.getNome().getNome() == "Tarefa Editada" && lido.getPrioridade().getPrioridade() == "ALTA", "nome e prioridade devem ser atualizados");
    verificar(lido.getEstado().getEstado() == Estado::A_FAZER, "atualizarCartao nao pode alterar o estado");
    verificar(lanca<std::runtime_error>([&] { servico.atualizarCartao(cartaoMock("ACT999", Estado::A_FAZER)); }),
              "atualizar cartao inexistente deve lancar std::runtime_error");

    verificar(servico.listarCartoesPorQuadro(quadro).size() == 1, "listar deve retornar o cartao do quadro");
    verificar(servico.excluirCartao(cartao) == true, "excluir cartao existente deve retornar true");
    verificar(servico.listarCartoesPorQuadro(quadro).empty(), "quadro deve ficar sem cartoes");
    verificar(lanca<std::runtime_error>([&] { servico.excluirCartao(cartao); }), "excluir cartao inexistente deve lancar std::runtime_error");

    limparProjeto("PRJ912");
    std::cout << "  [OK] Teste Unitario: ServicoQuadros - criar, ler, atualizar, listar e excluir cartoes.\n";
}

void TUServicoQuadros::testarMoverCartaoEWip() {
    prepararProjeto("PRJ914");
    servico.criarQuadro(quadroMock("QDR914", 1), Identificador("PRJ914")); // limite de WIP = 1
    servico.criarCartao(cartaoMock("ACT914", Estado::A_FAZER), Identificador("QDR914"));
    servico.criarCartao(cartaoMock("ACT915", Estado::A_FAZER), Identificador("QDR914"));
    const Identificador c1("ACT914");
    const Identificador c2("ACT915");
    const Identificador quadro("QDR914");

    verificar(lanca<std::runtime_error>([&] { servico.moverCartao(c1, Estado(Estado::FEITO), Timestamp("01-NOV-2026-10:00")); }),
              "A FAZER -> FEITO e transicao invalida e deve lancar std::runtime_error");
    verificar(servico.moverCartao(c1, Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-10:00")) == true, "A FAZER -> FAZENDO deve funcionar");
    verificar(servico.lerWip(quadro) == 1, "WIP deve ser 1 apos iniciar um cartao");

    verificar(lanca<std::runtime_error>([&] { servico.moverCartao(c2, Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-10:30")); }),
              "mover alem do limite de WIP deve lancar std::runtime_error");
    verificar(servico.lerWip(quadro) == 1, "WIP nao pode passar do limite");

    verificar(lanca<std::runtime_error>([&] { servico.moverCartao(c1, Estado(Estado::FEITO), Timestamp("01-NOV-2026-08:00")); }),
              "horario anterior ao inicio do cartao deve lancar std::runtime_error");
    verificar(lanca<std::runtime_error>([&] { servico.moverCartao(c1, Estado(Estado::A_FAZER), Timestamp("01-NOV-2026-11:00")); }),
              "FAZENDO -> A FAZER e transicao invalida");

    verificar(servico.moverCartao(c1, Estado(Estado::FEITO), Timestamp("01-NOV-2026-12:30")) == true, "FAZENDO -> FEITO deve funcionar");
    verificar(servico.lerWip(quadro) == 0, "WIP deve voltar a 0 apos concluir");
    verificar(servico.moverCartao(c2, Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-13:00")) == true, "com WIP livre o outro cartao pode iniciar");
    verificar(lanca<std::runtime_error>([&] { servico.moverCartao(Identificador("ACT999"), Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-13:00")); }),
              "mover cartao inexistente deve lancar std::runtime_error");

    // Novo limite menor que o WIP atual deve ser recusado
    servico.criarQuadro(quadroMock("QDR916", 2), Identificador("PRJ914"));
    servico.criarCartao(cartaoMock("ACT916", Estado::A_FAZER), Identificador("QDR916"));
    servico.criarCartao(cartaoMock("ACT917", Estado::A_FAZER), Identificador("QDR916"));
    servico.moverCartao(Identificador("ACT916"), Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-10:00"));
    servico.moverCartao(Identificador("ACT917"), Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-10:00"));
    verificar(lanca<std::runtime_error>([&] { servico.atualizarQuadro(quadroMock("QDR916", 1)); }),
              "limite menor que o WIP atual deve lancar std::runtime_error");

    limparProjeto("PRJ914");
    std::cout << "  [OK] Teste Unitario: ServicoQuadros::moverCartao e ServicoQuadros::lerWip (transicoes, horario e limite de WIP).\n";
}

void TUServicoQuadros::testarMetricas() {
    prepararProjeto("PRJ918");
    servico.criarQuadro(quadroMock("QDR918", 5), Identificador("PRJ918"));
    const Identificador quadro("QDR918");
    servico.criarCartao(cartaoMock("ACT918", Estado::A_FAZER), quadro); // entrada 01-NOV-2026-09:00
    servico.criarCartao(cartaoMock("ACT919", Estado::A_FAZER), quadro);
    servico.criarCartao(cartaoMock("ACT920", Estado::A_FAZER), quadro); // nunca sera concluido

    verificar(servico.lerTempoDeCicloMedio(quadro) == 0.0 && servico.lerLeadTimeMedio(quadro) == 0.0,
              "sem cartoes concluidos as medias devem ser 0");
    verificar(lanca<std::runtime_error>([&] { servico.lerTempoDeCiclo(Identificador("ACT918")); }),
              "tempo de ciclo de cartao nao concluido deve lancar std::runtime_error");

    servico.moverCartao(Identificador("ACT918"), Estado(Estado::FAZENDO), Timestamp("01-NOV-2026-10:00"));
    servico.moverCartao(Identificador("ACT918"), Estado(Estado::FEITO), Timestamp("01-NOV-2026-12:30"));
    servico.moverCartao(Identificador("ACT919"), Estado(Estado::FAZENDO), Timestamp("02-NOV-2026-09:00"));
    servico.moverCartao(Identificador("ACT919"), Estado(Estado::FEITO), Timestamp("02-NOV-2026-10:00"));

    verificar(servico.lerTempoDeCiclo(Identificador("ACT918")) == 150, "tempo de ciclo ACT918 = 150 min");
    verificar(servico.lerLeadTime(Identificador("ACT918")) == 210, "lead time ACT918 = 210 min");
    verificar(servico.lerTempoDeCiclo(Identificador("ACT919")) == 60, "tempo de ciclo ACT919 = 60 min");
    verificar(servico.lerLeadTime(Identificador("ACT919")) == 1500, "lead time ACT919 = 1500 min");
    verificar(servico.lerTempoDeCicloMedio(quadro) == 105.0, "tempo de ciclo medio = (150 + 60) / 2");
    verificar(servico.lerLeadTimeMedio(quadro) == 855.0, "lead time medio = (210 + 1500) / 2");
    verificar(servico.lerWip(quadro) == 0, "WIP = 0 (nenhum cartao em FAZENDO)");

    verificar(lanca<std::runtime_error>([&] { servico.lerLeadTime(Identificador("ACT999")); }), "lead time de cartao inexistente deve lancar");
    verificar(lanca<std::runtime_error>([&] { servico.lerWip(Identificador("QDR999")); }), "WIP de quadro inexistente deve lancar");
    verificar(lanca<std::runtime_error>([&] { servico.lerTempoDeCicloMedio(Identificador("QDR999")); }), "ciclo medio de quadro inexistente deve lancar");
    verificar(lanca<std::runtime_error>([&] { servico.lerLeadTimeMedio(Identificador("QDR999")); }), "lead time medio de quadro inexistente deve lancar");

    limparProjeto("PRJ918");
    std::cout << "  [OK] Teste Unitario: ServicoQuadros - tempo de ciclo, lead time e medias.\n";
}

void TUServicoQuadros::testarExclusaoEmCascata() {
    prepararProjeto("PRJ921");
    servico.criarQuadro(quadroMock("QDR921", 3), Identificador("PRJ921"));
    servico.criarCartao(cartaoMock("ACT921", Estado::A_FAZER), Identificador("QDR921"));

    verificar(servico.excluirQuadro(Identificador("QDR921")) == true, "excluir quadro existente deve retornar true");
    verificar(!ContainerQuadros::getInstancia()->existeQuadro(Identificador("QDR921")), "quadro deve ter sido removido");
    verificar(!ContainerCartoes::getInstancia()->existeCartao(Identificador("ACT921")), "cartoes do quadro devem ser removidos");
    verificar(lanca<std::runtime_error>([&] { servico.excluirQuadro(Identificador("QDR921")); }),
              "excluir quadro inexistente deve lancar std::runtime_error");

    limparProjeto("PRJ921");
    std::cout << "  [OK] Teste Unitario: ServicoQuadros::excluirQuadro remove os cartoes em cascata.\n";
}

void TUServicoQuadros::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE QUADROS    \n";
    std::cout << "             (CAMADA DE SERVICO)            \n";
    std::cout << "============================================\n";
    testarQuadros();
    testarCartoes();
    testarMoverCartaoEWip();
    testarMetricas();
    testarExclusaoEmCascata();
    std::cout << ">>> TODOS OS TESTES DE SERVICO DE QUADROS PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DA APRESENTAÇÃO DE AUTENTICAÇÃO
// =============================================================================

void TUApresentacaoAutenticacao::testarServicoIndisponivel() {
    ApresentacaoAutenticacao apresentacao; // setServico nao foi chamado (nullptr)
    Pessoa usuario;
    verificar(lanca<std::runtime_error>([&] { apresentacao.autenticar(usuario); }),
              "autenticar() sem servico disponível deve lancar std::runtime_error");
    std::cout << "  [OK] Teste Unitario: ApresentacaoAutenticacao lanca runtime_error se o servico estiver indisponivel.\n";
}

void TUApresentacaoAutenticacao::testarLoginValidoEInvalido() {
    MockServicoAutenticacao mock;
    ApresentacaoAutenticacao apresentacao;
    apresentacao.setServico(&mock);

    // 1. Login com sucesso (Garante a pessoa com o papel testado)
    {
        mock.aceitar = true;
        mock.pessoaAutenticada = pessoaMock(Papel::DESENVOLVEDOR);
        CapturaES cap("usuario@unb.br\nA1b2c\n");
        Pessoa usuario;
        bool ok = apresentacao.autenticar(usuario);
        verificar(ok == true && mock.chamadas == 1, "login com sucesso deve chamar o servico uma vez");
        verificar(usuario.getPapel().getPapel() == Papel::DESENVOLVEDOR, "usuario deve ser preenchido pelo servico");
    }

    // 2. Servico recusa as credenciais (retorna false para a controladora)
    {
        mock.aceitar = false;
        CapturaES cap("usuario@unb.br\nA1b2c\n");
        Pessoa usuario;
        bool ok = apresentacao.autenticar(usuario);
        verificar(ok == false && mock.chamadas == 2, "login recusado deve retornar false");
    }

    // 3. Formato invalido nas credenciais (excecao invalid_argument capturada pela tela)
    {
        mock.aceitar = true;
        CapturaES cap("usuario@unb.br\n123\n");
        Pessoa usuario;
        bool ok = apresentacao.autenticar(usuario);
        std::string tela = cap.texto();
        verificar(ok == false && mock.chamadas == 2, "senha invalida nao pode chegar ao servico");
        verificar(contem(tela, "ERRO") || contem(tela, "Erro"), "tela deve exibir o erro de formato");
    }

    std::cout << " [OK] Teste Unitario: ApresentacaoAutenticacao::autenticar (sucesso, recusa e erro de formato).\n";
}


void TUApresentacaoAutenticacao::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE AUTENTICACAO\n";
    std::cout << "          (CAMADA DE APRESENTACAO)          \n";
    std::cout << "============================================\n";
    testarServicoIndisponivel();
    testarLoginValidoEInvalido();
    std::cout << ">>> TODOS OS TESTES DE APRESENTACAO DE AUTENTICACAO PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DA APRESENTAÇÃO DE PESSOAS
// =============================================================================

void TUApresentacaoPessoas::testarServicoIndisponivel() {
    ApresentacaoPessoas apresentacao; // setServico nao foi chamado (nullptr)
    Pessoa u = pessoaMock(Papel::GESTOR);

    verificar(lanca<std::runtime_error>([&] { apresentacao.criarConta(); }), "criarConta() sem servico injetado deve lancar std::runtime_error");
    verificar(lanca<std::runtime_error>([&] { apresentacao.gerenciarConta(u); }), "gerenciarConta() sem servico injetado deve lancar std::runtime_error");
    std::cout << "  [OK] Teste Unitario: ApresentacaoPessoas lanca runtime_error se o servico estiver indisponivel.\n";
}

void TUApresentacaoPessoas::testarCriarContaValidaEInvalida() {
    MockServicoPessoasCompleto mock;
    ApresentacaoPessoas apresentacao;
    apresentacao.setServico(&mock);

    {   // Entrada valida
        CapturaES cap("novo.usuario@unb.br\nAna Clara\nA1b2c\nGESTOR\n");
        bool ok = apresentacao.criarConta();
        std::string tela = cap.texto();
        verificar(ok == true, "criarConta() com dados validos deve retornar true");
        verificar(mock.chamadasCriar == 1, "servico->criar deve ser chamado uma vez");
        verificar(contem(tela, "SUCESSO"), "tela deve exibir mensagem de sucesso");
    }
    {   // Senha sem letras: erro de formato
        CapturaES cap("novo.usuario@unb.br\nAna Clara\n12345\nGESTOR\n");
        bool ok = apresentacao.criarConta();
        std::string tela = cap.texto();
        verificar(ok == false, "criarConta() com senha sem letras deve retornar false");
        verificar(mock.chamadasCriar == 1, "servico->criar nao deve ser chamado quando dominio falha");
        verificar(contem(tela, "ERRO DE ENTRADA"), "tela deve informar erro de formato invalido");
    }
    {   // Erro de negocio (e-mail duplicado)
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

    // 1 (Ler) -> 2 (Atualizar para "User Novo", "B2c3d", "GESTOR") -> 4 (Listar) -> 0 (Voltar)
    CapturaES cap("1\n2\nUser Novo\nB2c3d\nGESTOR\n4\n0\n");
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
    Pessoa logado = pessoaMock(Papel::GESTOR);

    // Cancela (N) e depois confirma (S); o menu encerra sozinho apos excluir
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

// =============================================================================
// TESTES UNITÁRIOS DA APRESENTAÇÃO DE PROJETOS
// =============================================================================

void TUApresentacaoProjetos::testarServicoIndisponivel() {
    ApresentacaoProjetos apresentacao; // setServico nao foi chamado (nullptr)
    Pessoa gestor = pessoaMock(Papel::GESTOR);
    verificar(lanca<std::runtime_error>([&] { apresentacao.executar(gestor); }),
              "executar() sem servico injetado deve lancar std::runtime_error");
    std::cout << "  [OK] Teste Unitario: ApresentacaoProjetos lanca runtime_error se o servico estiver indisponivel.\n";
}

void TUApresentacaoProjetos::testarListagens() {
    MockServicoProjetos mock;
    mock.projetos.push_back(projetoMock("PRJ321"));
    ApresentacaoProjetos apresentacao;
    apresentacao.setServico(&mock);

    CapturaES cap("1\n2\n0\n"); // listar todos, listar meus, voltar
    apresentacao.executar(pessoaMock(Papel::GESTOR));
    std::string tela = cap.texto();

    verificar(mock.listagensTodos == 1, "opcao 1 deve chamar servico->listarTodos");
    verificar(mock.listagensGestor == 1, "opcao 2 deve chamar servico->listarPorGestor");
    verificar(contem(tela, "PRJ321"), "listagem deve exibir o identificador do projeto");
    std::cout << "  [OK] Teste Unitario: ApresentacaoProjetos - listar todos os projetos e listar meus projetos.\n";
}

void TUApresentacaoProjetos::testarCriarProjeto() {
    const std::string dados = "PRJ123\nProjeto\nDescricao valida.\n01-JAN-2026-10:00\n30-JUN-2026-18:00\n";
    MockServicoProjetos mock;
    ApresentacaoProjetos apresentacao;
    apresentacao.setServico(&mock);
    Pessoa gestor = pessoaMock(Papel::GESTOR);

    {   // Sucesso
        CapturaES cap("3\n" + dados + "0\n");
        apresentacao.executar(gestor);
        verificar(mock.criacoes == 1, "dados validos devem chamar servico->criar uma vez");
        verificar(contem(cap.texto(), "SUCESSO"), "tela deve exibir mensagem de sucesso");
    }
    {   // Identificador invalido: erro de formato, servico nao e chamado
        CapturaES cap("3\nXX\nProjeto\nDescricao valida.\n01-JAN-2026-10:00\n30-JUN-2026-18:00\n0\n");
        apresentacao.executar(gestor);
        verificar(mock.criacoes == 1, "dados invalidos nao podem chegar ao servico");
        verificar(contem(cap.texto(), "ERRO DE ENTRADA"), "tela deve informar erro de formato");
    }
    {   // Erro de negocio
        mock.erroCriacao = "negocio";
        CapturaES cap("3\n" + dados + "0\n");
        apresentacao.executar(gestor);
        verificar(contem(cap.texto(), "ERRO DE NEGOCIO"), "runtime_error do servico deve virar ERRO DE NEGOCIO");
    }
    {   // Erro de permissao
        mock.erroCriacao = "permissao";
        CapturaES cap("3\n" + dados + "0\n");
        apresentacao.executar(gestor);
        verificar(contem(cap.texto(), "ERRO DE PERMISSAO"), "logic_error do servico deve virar ERRO DE PERMISSAO");
    }
    std::cout << "  [OK] Teste Unitario: ApresentacaoProjetos - criar projeto (sucesso, formato, negocio e permissao).\n";
}

void TUApresentacaoProjetos::testarPermissaoDesenvolvedor() {
    MockServicoProjetos mock;
    ApresentacaoProjetos apresentacao;
    apresentacao.setServico(&mock);

    CapturaES cap("3\n0\n");
    apresentacao.executar(pessoaMock(Papel::DESENVOLVEDOR));
    std::string tela = cap.texto();

    verificar(!contem(tela, "3. Criar Novo Projeto"), "desenvolvedor nao deve ver a opcao de criar projeto");
    verificar(contem(tela, "Apenas gestores podem criar projetos!"), "opcao 3 deve ser negada ao desenvolvedor");
    verificar(mock.criacoes == 0, "nenhuma criacao pode chegar ao servico");
    std::cout << "  [OK] Teste Unitario: ApresentacaoProjetos - desenvolvedor nao pode criar projetos.\n";
}

void TUApresentacaoProjetos::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE PROJETOS   \n";
    std::cout << "          (CAMADA DE APRESENTACAO)          \n";
    std::cout << "============================================\n";
    testarServicoIndisponivel();
    testarListagens();
    testarCriarProjeto();
    testarPermissaoDesenvolvedor();
    std::cout << ">>> TODOS OS TESTES DE APRESENTACAO DE PROJETOS PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DA APRESENTAÇÃO DE QUADROS
// =============================================================================

void TUApresentacaoQuadros::testarServicoIndisponivel() {
    ApresentacaoQuadros apresentacao; // setServico nao foi chamado (nullptr)
    Pessoa gestor = pessoaMock(Papel::GESTOR);
    verificar(lanca<std::runtime_error>([&] { apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor); }),
              "gerenciarQuadros() sem servico injetado deve lancar std::runtime_error");
    verificar(lanca<std::runtime_error>([&] { apresentacao.exibirQuadro(Identificador("QDR001"), gestor); }),
              "exibirQuadro() sem servico injetado deve lancar std::runtime_error");
    std::cout << "  [OK] Teste Unitario: ApresentacaoQuadros lanca runtime_error se o servico estiver indisponivel.\n";
}

void TUApresentacaoQuadros::testarGerenciarQuadros() {
    MockServicoQuadros mock;
    ApresentacaoQuadros apresentacao;
    apresentacao.setServico(&mock);
    Pessoa gestor = pessoaMock(Papel::GESTOR);

    {   // Projeto com quadro: lista, abre o quadro e mostra as 3 colunas
        mock.quadros.push_back(Quadro(Identificador("QDR001"), Nome("Sprint Um"), Limite(3)));
        mock.cartoes = { cartaoMock("ACT001", Estado::A_FAZER), cartaoMock("ACT002", Estado::FAZENDO), cartaoMock("ACT003", Estado::FEITO) };
        CapturaES cap("1\nQDR001\n0\n0\n"); // listar/abrir, ID do quadro, voltar do quadro, voltar do menu
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor);
        std::string tela = cap.texto();
        verificar(contem(tela, "QUADROS DISPONIVEIS") && contem(tela, "QDR001"), "deve listar os quadros do projeto");
        verificar(contem(tela, "[ A FAZER ]") && contem(tela, "[ FAZENDO ]") && contem(tela, "[ FEITO ]"), "deve exibir as tres colunas");
        verificar(contem(tela, "ACT001") && contem(tela, "ACT002") && contem(tela, "ACT003"), "deve exibir os cartoes nas colunas");
        verificar(contem(tela, "2 / 3"), "deve exibir WIP atual e limite");
    }
    {   // Projeto sem quadros
        mock.quadros.clear();
        CapturaES cap("1\n0\n");
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor);
        verificar(contem(cap.texto(), "Nenhum quadro cadastrado"), "deve avisar que nao ha quadros");
    }
    {   // Quadro inexistente
        mock.quadros.push_back(Quadro(Identificador("QDR001"), Nome("Sprint Um"), Limite(3)));
        mock.quadroExiste = false;
        CapturaES cap("1\nQDR001\n0\n0\n");
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor);
        verificar(contem(cap.texto(), "Quadro nao encontrado."), "deve avisar quando o quadro nao existe");
    }
    std::cout << "  [OK] Teste Unitario: ApresentacaoQuadros::gerenciarQuadros e exibirQuadro (listar, abrir, vazio e inexistente).\n";
}

void TUApresentacaoQuadros::testarCriarQuadro() {
    MockServicoQuadros mock;
    ApresentacaoQuadros apresentacao;
    apresentacao.setServico(&mock);
    Pessoa gestor = pessoaMock(Papel::GESTOR);

    {   // Sucesso
        CapturaES cap("2\nQDR002\nSprint\n5\n0\n");
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor);
        verificar(mock.criacoesQuadro == 1, "dados validos devem chamar servico->criarQuadro");
        verificar(contem(cap.texto(), "SUCESSO"), "tela deve exibir mensagem de sucesso");
    }
    {   // Limite fora de 1..25
        CapturaES cap("2\nQDR002\nSprint\n30\n0\n");
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor);
        verificar(mock.criacoesQuadro == 1, "limite invalido nao pode chegar ao servico");
        verificar(contem(cap.texto(), "ERRO DE FORMATO"), "tela deve informar erro de formato");
    }
    {   // Erro de negocio
        mock.falharNegocio = true;
        CapturaES cap("2\nQDR002\nSprint\n5\n0\n");
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), gestor);
        verificar(contem(cap.texto(), "ERRO DE NEGOCIO"), "runtime_error do servico deve virar ERRO DE NEGOCIO");
    }
    {   // Desenvolvedor nao cria quadro
        mock.falharNegocio = false;
        int antes = mock.criacoesQuadro;
        CapturaES cap("2\n0\n");
        apresentacao.gerenciarQuadros(Identificador("PRJ001"), pessoaMock(Papel::DESENVOLVEDOR));
        verificar(mock.criacoesQuadro == antes, "desenvolvedor nao pode criar quadro");
        verificar(!contem(cap.texto(), "2. Criar Novo Quadro"), "desenvolvedor nao deve ver a opcao de criar quadro");
    }
    std::cout << "  [OK] Teste Unitario: ApresentacaoQuadros - criar quadro (sucesso, formato, negocio e permissao).\n";
}

void TUApresentacaoQuadros::testarExibirQuadroCartoes() {
    MockServicoQuadros mock;
    ApresentacaoQuadros apresentacao;
    apresentacao.setServico(&mock);
    const Identificador quadro("QDR001");

    {   // Gestor adiciona cartao
        CapturaES cap("2\nACT002\nTarefa\nALTA\nPEQUENO\n01-NOV-2026-09:00\n0\n");
        apresentacao.exibirQuadro(quadro, pessoaMock(Papel::GESTOR));
        verificar(mock.criacoesCartao == 1, "dados validos devem chamar servico->criarCartao");
        verificar(contem(cap.texto(), "SUCESSO"), "tela deve exibir mensagem de sucesso");
    }
    {   // Prioridade invalida
        CapturaES cap("2\nACT002\nTarefa\nURGENTE\nPEQUENO\n01-NOV-2026-09:00\n0\n");
        apresentacao.exibirQuadro(quadro, pessoaMock(Papel::GESTOR));
        verificar(mock.criacoesCartao == 1, "prioridade invalida nao pode chegar ao servico");
        verificar(contem(cap.texto(), "ERRO DE FORMATO"), "tela deve informar erro de formato");
    }
    {   // Desenvolvedor nao adiciona cartao
        CapturaES cap("2\n0\n");
        apresentacao.exibirQuadro(quadro, pessoaMock(Papel::DESENVOLVEDOR));
        verificar(mock.criacoesCartao == 1, "desenvolvedor nao pode criar cartao");
        verificar(!contem(cap.texto(), "2. Adicionar Novo Cartao"), "desenvolvedor nao deve ver a opcao de adicionar cartao");
    }
    std::cout << "  [OK] Teste Unitario: ApresentacaoQuadros::exibirQuadro - adicionar cartao (sucesso, formato e permissao).\n";
}

void TUApresentacaoQuadros::testarMoverCartao() {
    MockServicoQuadros mock;
    ApresentacaoQuadros apresentacao;
    apresentacao.setServico(&mock);
    const Identificador quadro("QDR001");
    Pessoa dev = pessoaMock(Papel::DESENVOLVEDOR); // desenvolvedor tambem pode mover

    {   // Sucesso
        CapturaES cap("1\nACT001\nFAZENDO\n01-NOV-2026-10:00\n0\n");
        apresentacao.exibirQuadro(quadro, dev);
        verificar(mock.movimentos == 1 && mock.ultimoEstado == "FAZENDO", "deve chamar servico->moverCartao com o estado informado");
        verificar(contem(cap.texto(), "SUCESSO"), "tela deve exibir mensagem de sucesso");
    }
    {   // Bloqueio de regra (WIP / transicao)
        mock.bloquearMovimento = true;
        CapturaES cap("1\nACT001\nFAZENDO\n01-NOV-2026-10:00\n0\n");
        apresentacao.exibirQuadro(quadro, dev);
        verificar(contem(cap.texto(), "BLOQUEIO"), "runtime_error do servico deve virar mensagem de bloqueio");
    }
    {   // Estado invalido
        mock.bloquearMovimento = false;
        int antes = mock.movimentos;
        CapturaES cap("1\nACT001\nABC\n01-NOV-2026-10:00\n0\n");
        apresentacao.exibirQuadro(quadro, dev);
        verificar(mock.movimentos == antes, "estado invalido nao pode chegar ao servico");
        verificar(contem(cap.texto(), "ERRO DE FORMATO"), "tela deve informar erro de formato");
    }
    std::cout << "  [OK] Teste Unitario: ApresentacaoQuadros::exibirQuadro - mover cartao (sucesso, bloqueio e formato).\n";
}

void TUApresentacaoQuadros::testarMetricas() {
    MockServicoQuadros mock;
    ApresentacaoQuadros apresentacao;
    apresentacao.setServico(&mock);

    CapturaES cap("3\n0\n");
    apresentacao.exibirQuadro(Identificador("QDR001"), pessoaMock(Papel::GESTOR));
    std::string tela = cap.texto();

    verificar(contem(tela, "WIP Atual: 2"), "deve exibir o WIP");
    verificar(contem(tela, "Tempo de Ciclo Medio: 105"), "deve exibir o tempo de ciclo medio");
    verificar(contem(tela, "Lead Time Medio: 855"), "deve exibir o lead time medio");
    std::cout << "  [OK] Teste Unitario: ApresentacaoQuadros::exibirQuadro - metricas Kanban.\n";
}

void TUApresentacaoQuadros::executar() {
    std::cout << "\n============================================\n";
    std::cout << "  EXECUTANDO TESTES UNITARIOS DE QUADROS    \n";
    std::cout << "          (CAMADA DE APRESENTACAO)          \n";
    std::cout << "============================================\n";
    testarServicoIndisponivel();
    testarGerenciarQuadros();
    testarCriarQuadro();
    testarExibirQuadroCartoes();
    testarMoverCartao();
    testarMetricas();
    std::cout << ">>> TODOS OS TESTES DE APRESENTACAO DE QUADROS PASSARAM! <<<\n\n";
}

// =============================================================================
// TESTES UNITÁRIOS DO MÓDULO DE INICIALIZAÇÃO E CONTROLE
// =============================================================================

void TUApresentacaoControle::testarModulosIndisponiveis() {
    const char* esperado[7] = {
        "Autenticacao", "Pessoas (apresentacao)", "Projetos (apresentacao)", "Quadros (apresentacao)",
        "Servico de Pessoas", "Servico de Projetos", "Servico de Quadros"
    };

    for (int faltando = 0; faltando < 7; ++faltando) {
        Cenario c;
        // Remove da controladora exatamente um modulo por vez.
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
    // Numero fora do menu e texto nao numerico: ambos sao recusados sem derrubar o programa.
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
    {   // Gestor confirma a exclusao
        Cenario c;
        std::string tela = c.rodar("2\n3\nPRJ001\nS\n0\n0\n");
        verificar(c.servicoProjetos.exclusoes == 1, "confirmacao S deve chamar o servico de exclusao uma vez");
        verificar(contem(tela, "excluidos em cascata"), "deve avisar que o projeto e seus quadros foram excluidos");
    }
    {   // Gestor cancela
        Cenario c;
        std::string tela = c.rodar("2\n3\nPRJ001\nN\n0\n0\n");
        verificar(c.servicoProjetos.exclusoes == 0, "confirmacao N nao pode chamar o servico");
        verificar(contem(tela, "Exclusao cancelada."), "deve avisar que a exclusao foi cancelada");
    }
    {   // Projeto inexistente
        Cenario c;
        c.servicoProjetos.existe = false;
        std::string tela = c.rodar("2\n3\nPRJ001\nS\n0\n0\n");
        verificar(contem(tela, "Projeto nao encontrado."), "deve avisar quando o projeto nao existe");
    }
    std::cout << "  [OK] Teste Unitario: Exclusao de projeto (confirmada, cancelada e inexistente).\n";
}

void TUApresentacaoControle::testarAbrirQuadro() {
    {   // Projeto existente: a controladora delega ao modulo de Quadros
        Cenario c;
        c.servicoProjetos.projetos.push_back(projetoMock("PRJ001"));
        std::string tela = c.rodar("2\n2\nPRJ001\n0\n0\n");
        verificar(contem(tela, "PROJETOS CADASTRADOS"), "deve listar os projetos para escolha");
        verificar(c.quadrosTela.chamadasGerenciar == 1 && c.quadrosTela.ultimoIdProjeto == "PRJ001",
                  "deve abrir o gerenciamento de quadros do projeto PRJ001");
    }
    {   // Nenhum projeto cadastrado
        Cenario c;
        std::string tela = c.rodar("2\n2\n0\n0\n");
        verificar(contem(tela, "Nenhum projeto cadastrado"), "deve avisar quando nao ha projetos");
        verificar(c.quadrosTela.chamadasGerenciar == 0, "nao pode abrir quadros sem projetos");
    }
    {   // Projeto digitado nao existe
        Cenario c;
        c.servicoProjetos.projetos.push_back(projetoMock("PRJ001"));
        c.servicoProjetos.existe = false;
        std::string tela = c.rodar("2\n2\nPRJ001\n0\n0\n");
        verificar(contem(tela, "Projeto nao encontrado."), "deve avisar quando o projeto nao existe");
        verificar(c.quadrosTela.chamadasGerenciar == 0, "nao pode abrir quadros de projeto inexistente");
    }
    std::cout << "  [OK] Teste Unitario: Abrir quadros (projeto existente, sem projetos e projeto inexistente).\n";
}

void TUApresentacaoControle::testarIdentificadorInvalido() {
    Cenario c;
    c.servicoProjetos.projetos.push_back(projetoMock("PRJ001"));
    // "XX" nao e um Identificador valido (3 letras + 3 digitos): o erro deve ser tratado na tela.
    std::string tela = c.rodar("2\n2\nXX\n0\n0\n");

    verificar(contem(tela, "ERRO DE ENTRADA"), "identificador invalido deve gerar mensagem de erro de entrada");
    verificar(c.quadrosTela.chamadasGerenciar == 0, "identificador invalido nao pode abrir quadros");
    std::cout << "  [OK] Teste Unitario: Identificador invalido e tratado sem encerrar o programa.\n";
}

void TUApresentacaoControle::testarSessaoAposGerenciarConta() {
    {   // Conta alterada: o menu deve refletir o novo nome
        Cenario c;
        c.servicoPessoas.pessoaAtual = Pessoa(Email("usuario@unb.br"), Nome("Novo Nome"), Senha(SENHA_TU), Papel(Papel::GESTOR));
        std::string tela = c.rodar("2\n4\n0\n0\n");
        verificar(c.pessoas.gerenciarChamadas == 1, "opcao 4 deve acionar o modulo de Pessoas (gerenciar conta)");
        verificar(contem(tela, "Novo Nome (GESTOR)"), "sessao deve refletir o nome atualizado");
    }
    {   // Conta excluida: a sessao e encerrada e volta para a tela inicial
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

