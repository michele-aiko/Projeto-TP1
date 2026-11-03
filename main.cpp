#include "apresentacao.hpp" 
#include "servicos.hpp" 
#include "testeUnit.hpp" 
#include <iostream>

//Código abaixo apenas para TESTE. O processos descritos em 1, 2 e 3 devem ser implementados pelo módulo Inicialização e Controle

int main() { 
    // 1. Executa a bateria de Testes Unitários dos Serviços 
    TUTesteServicos suiteTestes; 
    suiteTestes.executar();
    // 2. Instanciação das Camadas Concretas (Serviços e Apresentação) 
    ServicoProjetos servicoProjetos;
    ApresentacaoProjetos apresentacaoProjetos;
    // 3. Injeção de Dependências (Ligação pelas Interfaces Abstratas)
    apresentacaoProjetos.setServico(&servicoProjetos); 
    
    // 4. Criação do usuário de demonstração para a sessão 
    Pessoa gestorSessao(Email("gestor@unb.br"), Nome("Joyce Gestor"), Senha("A1b2c"), Papel(Papel::GESTOR));
    
    int acaoTeste = 0; 
    std::cout << "========================================\n"; 
    std::cout << " SISTEMA KANBAN - PROJETOS E QUADROS \n"; 
    std::cout << "========================================\n"; 
    std::cout << "1. Abrir Menu de Projetos\n"; 
    std::cout << "2. Abrir Quadro Kanban de Exemplo (QDR001)\n"; 
    std::cout << "0. Sair\n"; 
    std::cout << "Escolha uma opcao: "; 
    std::cin >> acaoTeste;
    
    if (acaoTeste == 1) { 
        apresentacaoProjetos.executar(gestorSessao); 
    } else if (acaoTeste == 2) { 
        apresentacaoQuadros.exibirQuadro(Identificador("QDR001"), gestorSessao); 
    }
    
    std::cout << "\nEncerrando o programa com sucesso.\n"; 
    return 0;

}

