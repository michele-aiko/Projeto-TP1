#include "apresentacao.hpp"
#include "servicos.hpp"
#include <iostream>
#include <stdexcept>

// A tela inicial com os menus de opcoes ficam em ApresentacaoControle.
// Ainda sem rodar porque depende da criacao de outros objetos da apresentacao

int main() {
    // 1. Instanciação das Camadas Concretas (Serviços e Apresentação)
    ServicoAutenticacao servicoAutenticacao;
    ServicoPessoas servicoPessoas;
    ServicoProjetos servicoProjetos;
    ServicoQuadros servicoQuadros;

    ApresentacaoAutenticacao apresentacaoAutenticacao;
    ApresentacaoPessoas apresentacaoPessoas;
    ApresentacaoProjetos apresentacaoProjetos;
    ApresentacaoQuadros apresentacaoQuadros;
    ApresentacaoControle apresentacaoControle;

    // 2. Injeção de Dependências (Ligação pelas Interfaces Abstratas)
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

    // 3. Execução do sistema (tela inicial, login e menu principal)
    try {
        apresentacaoControle.executar();
    } catch (const std::runtime_error& e) {
        std::cerr << "ERRO FATAL: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\nEncerrando o programa com sucesso.\n";
    return 0;

}

