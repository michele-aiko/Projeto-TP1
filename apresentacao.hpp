#ifndef APRESENTACAO_HPP_INCLUDED 
#define APRESENTACAO_HPP_INCLUDED 
#include "interfaces.hpp" 
#include <string>

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
