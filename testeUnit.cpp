#include "testeUnit.hpp"
#include <cassert>
#include <iostream>

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

