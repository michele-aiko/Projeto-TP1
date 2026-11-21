#ifndef APRESENTACAO_HPP_INCLUDED
#define APRESENTACAO_HPP_INCLUDED
#include "interfaces.hpp"
#include "dominios.hpp"
#include "entidades.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

///
/// @brief Controladora da Apresentação para o Módulo de Autenticação de Usuários.
/// @details Recebe o e-mail e a senha do console, constrói os objetos de domínio
///          e chama a camada de serviço para realizar o login no sistema.
///
class ApresentacaoAutenticacao : public IApresentacaoAutenticacao {
private:
    IServicoAutenticacao* servico = nullptr; ///< Ponteiro para a interface de serviço de autenticação.

    /// @brief Garante que o ponteiro de serviço foi criado antes do uso.
    /// @throw std::runtime_error Se o serviço estiver indisponível (ponteiro nulo).
    void validarDependencia() const;

    /// @brief Exibe um rótulo e lê uma linha de texto do teclado.
    /// @param rotulo Texto de instrução para o usuário.
    /// @return String digitada pelo usuário.
    std::string lerLinha(const std::string& rotulo) const;

public:
    virtual ~ApresentacaoAutenticacao() = default;

    /// @brief Executa a tela de login.
    /// @param usuario Instância de Pessoa preenchida com os dados do usuário se o login for bem-sucedido.
    /// @return true se o login for realizado; false caso contrário.
    /// @throw std::runtime_error Se o serviço de autenticação estiver indisponível.
    bool autenticar(Pessoa& usuario) override;

    /// @brief Conecta a tela ao seu serviço correspondente.
    /// @param servico Ponteiro para a implementação da interface IServicoAutenticacao.
    void setServico(IServicoAutenticacao* servico) override { this->servico = servico; }
};

///
/// @brief Controladora de Apresentação para o Módulo de Pessoas (Usuários / Contas).
/// @details Responsável pela interação textual no console (`std::cin` / `std::cout`) e pela
///          pré-validação sintática dos dados de entrada (por meio das classes de domínio
///          `Email`, `Nome`, `Senha` e `Papel`) para os serviços de Criar, Ler, Atualizar,
///          Excluir e Listar Pessoas. Depende exclusivamente da interface abstrata `IServicoPessoas
///          e lança `std::runtime_error` caso o serviço esteja indisponível (`nullptr`).
///
// Requisitos Funcionais 1 a 5
class ApresentacaoPessoas : public IApresentacaoPessoas {
    private:
        IServicoPessoas* servico = nullptr; ///< Ponteiro abstrato injetado para a camada de serviço de pessoas.

        /// @brief Verifica se o serviço de pessoas está disponivel antes de processar requisições.
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
    IServicoProjetos* servico = nullptr; ///< Ponteiro para a interface de serviço de projetos.

    /// @brief Garante que o serviço de projetos está disponível antes de executar requisições.
    /// @throw std::runtime_error Se servico for nullptr.
    void validarDependencia() const;

    /// @brief Lê uma opção numérica de menu no console.
    /// @return Inteiro correspondente à opção escolhida, -1 se inválida ou 0 ao fim da entrada.
    int lerOpcao() const;

    /// @brief Lê uma linha de texto do teclado.
    /// @param rotulo Texto exibido ao usuário.
    /// @return String digitada.
    std::string lerLinha(const std::string& rotulo) const;

    /// @brief Auxiliar para capturar dados e criar um novo projeto (exclusivo para GESTOR).
    /// @param usuarioLogado Dados do usuário autenticado.
    void criarProjeto(const Pessoa& usuarioLogado);

    /// @brief Lista os projetos sob responsabilidade do gestor logado.
    /// @param usuarioLogado Dados do usuário autenticado.
    void listarMeusProjetos(const Pessoa& usuarioLogado);

    /// @brief Lista todos os projetos cadastrados no sistema.
    void listarTodos();

public:
    virtual ~ApresentacaoProjetos() = default;

    /// @brief Executa o menu interativo de gerenciamento de projetos.
    /// @param usuarioLogado Dados do usuário conectado na sessão.
    void executar(const Pessoa& usuarioLogado) override;

    /// @brief Disponibiliza o serviço de projetos.
    /// @param servico Ponteiro para a interface IServicoProjetos.
    void setServico(IServicoProjetos* servico) override { this->servico = servico; }
};

///
/// @brief Controladora do Módulo de Quadros e Cartões Kanban.
/// @details Exibe as colunas Kanban(A FAZER, FAZENDO, FEITO), gerencia quadros e cartões,
///          além de apresentar métricas dos projetos e quadros.
///
class ApresentacaoQuadros : public IApresentacaoQuadros {
private:
    IServicoQuadros* servico = nullptr; ///< Ponteiro para a interface de serviço de quadros.

    /// @brief Garante que o serviço de quadros está disponível antes de executar requisições.
    /// @throw std::runtime_error Se servico for nullptr.
    void validarDependencia() const;

    /// @brief Lê uma opção numérica de menu no console.
    /// @return Inteiro correspondente à opção escolhida, -1 se inválida ou 0 ao fim da entrada.
    int lerOpcao() const;

    /// @brief Lê uma linha de texto do teclado.
    /// @param rotulo Texto exibido ao usuário.
    /// @return String digitada.
    std::string lerLinha(const std::string& rotulo) const;

    /// @brief Formata e adiciona um novo cartão na coluna A FAZER do quadro.
    /// @param idQuadro Identificador do quadro pai.
    void adicionarCartao(const Identificador& idQuadro);

    /// @brief Captura dados para movimentar um cartão entre as colunas Kanban.
    /// @param idQuadro Identificador do quadro ativo.
    void moverCartao(const Identificador& idQuadro);

    /// @brief Exibe o WIP atual, Lead Time médio e Cycle Time médio do quadro.
    /// @param idQuadro Identificador do quadro.
    void exibirMetricas(const Identificador& idQuadro);

    /// @brief Cria um novo quadro associado ao projeto (exclusivo para GESTOR).
    /// @param idProjeto Identificador do projeto pai.
    void criarQuadro(const Identificador& idProjeto);

public:
    virtual ~ApresentacaoQuadros() = default;

    /// @brief Apresenta as colunas e cartões de um quadro Kanban.
    /// @param idQuadro Identificador do quadro a ser exibido.
    /// @param usuarioLogado Dados do usuário autenticado.
    void exibirQuadro(const Identificador& idQuadro, const Pessoa& usuarioLogado) override;

    /// @brief Gerencia e lista todos os quadros de um projeto.
    /// @param idProjeto Identificador do projeto pai.
    /// @param usuarioLogado Dados do usuário autenticado.
    void gerenciarQuadros(const Identificador& idProjeto, const Pessoa& usuarioLogado) override;

    /// @brief Disponibiliza o serviço de quadros.
    /// @param servico Ponteiro para a interface IServicoQuadros.
    void setServico(IServicoQuadros* servico) override { this->servico = servico; }
};

///
/// @brief Controladora de Apresentação para o Módulo de Inicialização e Controle do sistema.
/// @details Atua como controladora mestra do sistema Kanban: exibe a tela inicial (criar conta e login),
///          o menu principal do usuário autenticado e despacha para os demais módulos por meio de
///          interfaces abstratas. Antes de executar, verifica se todos os módulos dependentes estão disponíveis
///          e lança std::runtime_error caso contrário.
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

        /// @brief Verifica se todos os módulos estão disponíveis antes de iniciar o sistema.
        /// @throw std::runtime_error Se algum módulo dependente estiver indisponível (se qualquer ponteiro for nulo).
        void validarDependencias() const;

        /// @brief Executa uma ação de menu tratando exceções capturadas para evitar interrupções no programa.
        /// @param acao Função lambda contendo o fluxo a ser executado com tratamento de erros.
        void executarSeguro(const std::function<void()>& acao) const;

        /// @brief Lê uma opção numérica de menu. Retorna -1 se a entrada não for número e 0 se a entrada acabar.
        /// @return Número inteiro da opção selecionada.
        int lerOpcao() const;

        /// @brief Lê uma linha de texto não vazia do teclado.
        /// @param rotulo Texto de instrução para o usuário.
        /// @return String digitada.
        std::string lerLinha(const std::string& rotulo) const;

        /// @brief Exibe o menu inicial de boas-vindas (Criar conta, Entrar, Sair).
        void menuInicial();

        /// @brief Exibe o menu principal do sistema para o usuário autenticado.
        /// @param usuarioLogado Referência dos dados do usuário logado na sessão.
        void menuPrincipal(Pessoa& usuarioLogado);

        /// @brief Permite escolher um projeto e abrir o seu quadro Kanban.
        /// @param usuarioLogado Dados do usuário autenticado.
        void abrirQuadro(const Pessoa& usuarioLogado);

        /// @brief Solicita confirmação e exclui um projeto do sistema (exclusivo para GESTOR).
        /// @param usuarioLogado Dados do usuário autenticado.
        void excluirProjeto(const Pessoa& usuarioLogado);

    public:
        virtual ~ApresentacaoControle() = default;

        // Métodos de injeção de dependências para os módulos de apresentação e serviço
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
        /// @throw std::runtime_error Se algum módulo dependente não estiver disponível.
        void executar() override;
};


#endif // APRESENTACAO_HPP_INCLUDED

