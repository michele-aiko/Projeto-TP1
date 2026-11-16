#ifndef APRESENTACAO_HPP_INCLUDED 
#define APRESENTACAO_HPP_INCLUDED 
#include "interfaces.hpp" 
#include <string>

///
/// @brief Controladora de Apresentação para o Módulo de Pessoas (Usuários / Contas).
/// @details Responsável pela interação textual no console (`std::cin` / `std::cout`) e pela
///          pré-validação sintática dos dados de entrada (por meio das classes de domínio
///          `Email`, `Nome`, `Senha` e `Papel`) para os serviços de Criar, Ler, Atualizar,
///          Excluir e Listar Pessoas (Requisitos Funcionais 1 a 5). Depende exclusivamente da
///          interface abstrata `IServicoPessoas` e lança `std::runtime_error` caso o serviço
///          esteja indisponível (`nullptr`).
///
class ApresentacaoPessoas : public IApresentacaoPessoas {
    private:
        IServicoPessoas* servico = nullptr; ///< Ponteiro abstrato injetado para a camada de serviço de pessoas.

        /// @brief Verifica se o serviço de pessoas foi injetado antes de processar requisições.
        /// @throw std::runtime_error Se o ponteiro `servico` for `nullptr`.
        void validarDependencia() const;

        /// @brief Lê uma opção numérica de menu no console.
        /// @return Inteiro correspondente à opção escolhida, -1 se inválida ou 0 ao fim da entrada.
        int lerOpcao() const;

        /// @brief Exibe um rótulo e lê uma linha não vazia do console.
        /// @param rotulo Mensagem exibida ao usuário antes da leitura.
        /// @return Objeto `std::string` contendo a linha lida.
        std::string lerLinha(const std::string& rotulo) const;

        /// @brief Exibe os dados cadastrais da conta do usuario autenticado (Requisito 2).
        /// @param usuarioLogado Referencia para o usuario autenticado.
        void lerDadosConta(const Pessoa& usuarioLogado);

        /// @brief Atualiza Nome, Senha e Papel mantendo o Email inalterado (Requisito 3).
        /// @param usuarioLogado Referencia para o usuario autenticado na sessao.
        void atualizarConta(Pessoa& usuarioLogado);

        /// @brief Solicita confirmação e exclui a conta do usuário autenticado (Requisito 4).
        /// @param usuarioLogado Referência constante para o usuário autenticado.
        /// @return `true` se a conta foi excluída com sucesso; `false` caso contrário.
        bool excluirConta(const Pessoa& usuarioLogado);

        /// @brief Lista todas as pessoas cadastradas apresentando o identificador (Email) de cada entidade (Requisitos 5 e 11).
        void listarPessoas();

    public:
        /// @brief Destrutor virtual padrão da classe ApresentacaoPessoas.
        virtual ~ApresentacaoPessoas() = default;

        /// @brief Executa o formulário interativo de criação de nova conta de usuário (Requisito Funcional 1).
        /// @return `true` se a conta foi criada com sucesso; `false` se houve erro de validação ou de negócio.
        /// @throw std::runtime_error Se o serviço `IServicoPessoas` não tiver sido injetado (`nullptr`).
        bool criarConta() override;

        /// @brief Executa o menu interativo de gerenciamento de conta e consulta de usuários (Requisitos Funcionais 2 a 5).
        /// @param usuarioLogado Referência para o objeto `Pessoa` autenticado na sessão.
        /// @throw std::runtime_error Se o serviço `IServicoPessoas` não tiver sido injetado (`nullptr`).
        void gerenciarConta(Pessoa& usuarioLogado) override;

        /// @brief Vincula a controladora de apresentação à interface de serviço de pessoas.
        /// @param servico Ponteiro para uma implementação concreta de `IServicoPessoas`.
        void setServico(IServicoPessoas* servico) override { this->servico = servico; }
};

///
/// @brief Controladora de Apresentação para o Módulo de Projetos. 
/// @details Fornece os menus textuais de console para criar, consultar, listar e editar projetos. 
/// 

class ApresentacaoProjetos : public IApresentacaoProjetos { 
    private: 
        IServicoProjetos* servico; ///< Ponteiro abstrato injetado para a camada de serviço de projetos. 
        void criarProjeto(const Pessoa& usuarioLogado); 
        void listarMeusProjetos(const Pessoa& usuarioLogado); 
        void listarTodos(); 
    
    public: 
        virtual ~ApresentacaoProjetos() = default; 
        void executar(const Pessoa& usuarioLogado) override; 
        void setServico(IServicoProjetos* servico) override { this->servico = servico; } 
};


///
/// @brief Controladora de Apresentação para o Módulo de Inicialização e Controle.
/// @details Atua como controladora mestra do sistema Kanban: exibe a tela inicial (criar conta e login),
///          o menu principal do usuário autenticado e despacha para os demais módulos por meio de
///          interfaces abstratas. Antes de executar, verifica se todos os módulos dependentes foram
///          injetados e lança std::runtime_error caso algum esteja indisponível.
///

class ApresentacaoControle : public IApresentacaoControle {
    private:
        IApresentacaoAutenticacao* apresentacaoAutenticacao = nullptr; ///< Tela de login (Módulo 2).
        IApresentacaoPessoas* apresentacaoPessoas = nullptr;           ///< Telas de conta (Módulo 3).
        IApresentacaoProjetos* apresentacaoProjetos = nullptr;         ///< Telas de projetos (Módulo 4).
        IApresentacaoQuadros* apresentacaoQuadros = nullptr;           ///< Telas de quadros e cartões (Módulo 5).
        IServicoPessoas* servicoPessoas = nullptr;                     ///< Serviço usado para conferir a sessão.
        IServicoProjetos* servicoProjetos = nullptr;                   ///< Serviço usado para ler e excluir projetos.
        IServicoQuadros* servicoQuadros = nullptr;                     ///< Serviço usado para listar quadros de um projeto.

        /// @brief Verifica se todos os módulos foram injetados.
        /// @throw std::runtime_error Se algum módulo dependente estiver indisponível.
        void validarDependencias() const;

        /// @brief Lê uma opção numérica de menu. Retorna -1 se a entrada não for número e 0 se a entrada acabar.
        int lerOpcao() const;

        /// @brief Exibe um rótulo e lê uma linha não vazia do teclado.
        std::string lerLinha(const std::string& rotulo) const;

        void menuInicial();                               ///< Tela inicial: criar conta, login e sair.
        void menuPrincipal(Pessoa& usuarioLogado);        ///< Menu do usuário autenticado.
        void abrirQuadro(const Pessoa& usuarioLogado);    ///< Lista os quadros de um projeto e abre um deles.
        void excluirProjeto(const Pessoa& usuarioLogado); ///< Exclui um projeto (apenas GESTOR).

    public:
        virtual ~ApresentacaoControle() = default;

        /// @brief Injeta a controladora de apresentação de Autenticação.
        void setApresentacaoAutenticacao(IApresentacaoAutenticacao* modulo) { apresentacaoAutenticacao = modulo; }
        /// @brief Injeta a controladora de apresentação de Pessoas.
        void setApresentacaoPessoas(IApresentacaoPessoas* modulo) { apresentacaoPessoas = modulo; }
        /// @brief Injeta a controladora de apresentação de Projetos.
        void setApresentacaoProjetos(IApresentacaoProjetos* modulo) { apresentacaoProjetos = modulo; }
        /// @brief Injeta a controladora de apresentação de Quadros.
        void setApresentacaoQuadros(IApresentacaoQuadros* modulo) { apresentacaoQuadros = modulo; }
        /// @brief Injeta o serviço de Pessoas.
        void setServicoPessoas(IServicoPessoas* servico) { servicoPessoas = servico; }
        /// @brief Injeta o serviço de Projetos.
        void setServicoProjetos(IServicoProjetos* servico) { servicoProjetos = servico; }
        /// @brief Injeta o serviço de Quadros.
        void setServicoQuadros(IServicoQuadros* servico) { servicoQuadros = servico; }

        /// @brief Inicia o loop principal do sistema (tela inicial, login e menu principal).
        /// @throw std::runtime_error Se algum módulo dependente não tiver sido injetado.
        void executar() override;
};


#endif // APRESENTACAO_HPP_INCLUDED
