#ifndef APRESENTACAO_HPP_INCLUDED 
#define APRESENTACAO_HPP_INCLUDED 
#include "interfaces.hpp" 

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

#endif // APRESENTACAO_HPP_INCLUDED
