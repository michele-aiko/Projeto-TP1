#ifndef INTERFACES_HPP_INCLUDED
#define INTERFACES_HPP_INCLUDED

#include "dominios.hpp"
#include "entidades.hpp"
#include <vector>

// Declaração prévia das interfaces de serviço para criação de dependência nas controladoras de apresentação
class IServicoAutenticacao;
class IServicoPessoas;
class IServicoProjetos;
class IServicoQuadros;

// =============================================================================
// MÓDULO 1: INICIALIZAÇÃO E CONTROLE
// =============================================================================

///
/// @brief Interface da Camada de Apresentação para o Módulo de Inicialização e Controle.
/// @details Atua como a controladora mestra da aplicação. É responsável pela inicialização do sistema,
///          exibição do menu principal (boas-vindas, login, cadastro) e despacho para os demais módulos.
///
class IApresentacaoControle {
public:
    virtual ~IApresentacaoControle() = default;

    ///
    /// @brief Inicia o loop de execução principal do sistema de gestão Kanban.
    /// @details Mantém o fluxo interativo do console até que o usuário escolha encerrar o programa.
    ///
    virtual void executar() = 0;
};

// =============================================================================
// MÓDULO 2: AUTENTICAÇÃO
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Autenticação.
/// @details Declara os serviços de negócio para validação e autenticação de credenciais de acesso.
///
class IServicoAutenticacao {
public:
    virtual ~IServicoAutenticacao() = default;

    ///
    /// @brief Autentica um usuário no sistema com base em seu e-mail e senha.
    /// @param email Objeto da classe Email.
    /// @param senha Objeto da classe Senha.
    /// @param pessoa Referência para o objeto Pessoa a ser preenchido caso a autenticação tenha sucesso.
    /// @return true se as credenciais forem válidas e autenticadas; false caso contrário.
    ///
    virtual bool autenticar(const Email& email, const Senha& senha, Pessoa& pessoa) = 0;
};

///
/// @brief Interface da Camada de Apresentação para o Módulo de Autenticação.
/// @details Responsável por solicitar, capturar e pré-validar as credenciais do usuário na tela de login.
///
class IApresentacaoAutenticacao {
public:
    virtual ~IApresentacaoAutenticacao() = default;

    ///
    /// @brief Executa o fluxo de captura de login e senha no console.
    /// @param pessoa Referência para o objeto Pessoa preenchido em caso de sucesso.
    /// @return true se o usuário efetuou o login com sucesso; false se cancelou ou falhou.
    ///
    virtual bool autenticar(Pessoa& pessoa) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço de negócio.
    /// @param servico Ponteiro para uma implementação concreta de IServicoAutenticacao.
    ///
    virtual void setServico(IServicoAutenticacao* servico) = 0;
};

// =============================================================================
// MÓDULO 3: PESSOAS (USUÁRIOS / CONTAS)
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Pessoas.
/// @details Declara as operações de ciclo de vida (CRUD) dos usuários e controle cadastral.
///
class IServicoPessoas {
public:
    virtual ~IServicoPessoas() = default;

    ///
    /// @brief Cadastra uma nova pessoa no repositório do sistema.
    /// @param pessoa Objeto Pessoa devidamente preenchido e validado.
    /// @return true se cadastrado com sucesso; false caso o e-mail já exista.
    ///
    virtual bool criar(const Pessoa& pessoa) = 0;

    ///
    /// @brief Consulta os dados cadastrais de uma pessoa por meio de seu e-mail (chave primária).
    /// @param email Objeto Email da conta desejada.
    /// @param pessoa Referência do objeto onde os dados encontrados serão gravados.
    /// @return true se a pessoa foi localizada; false caso contrário.
    ///
    virtual bool ler(const Email& email, Pessoa& pessoa) = 0;

    ///
    /// @brief Atualiza as informações cadastrais de uma pessoa (Nome, Senha ou Papel).
    /// @param pessoa Objeto contendo o e-mail chave e os novos atributos de domínio.
    /// @return true se atualizado com sucesso; false se o registro não for localizado.
    ///
    virtual bool atualizar(const Pessoa& pessoa) = 0;

    ///
    /// @brief Remove permanentemente o cadastro de uma pessoa do sistema.
    /// @param email Objeto Email da conta a ser excluída.
    /// @return true se excluído com sucesso; false se o registro não existir ou possuir vínculos bloqueantes.
    ///
    virtual bool excluir(const Email& email) = 0;

    ///
    /// @brief Lista todas as pessoas cadastradas no sistema.
    /// @return Vetor contendo a lista de todas as instâncias de Pessoa.
    ///
    virtual std::vector<Pessoa> listar() = 0;
};

///
/// @brief Interface da Camada de Apresentação para o Módulo de Pessoas.
/// @details Responsável pelos menus de criação de conta, consulta de perfil, edição e exclusão.
///
class IApresentacaoPessoas {
public:
    virtual ~IApresentacaoPessoas() = default;

    ///
    /// @brief Executa a tela de criação de nova conta (auto-registro de usuário).
    /// @return true se a conta foi criada com sucesso; false se cancelada ou falha.
    ///
    virtual bool criarConta() = 0;

    ///
    /// @brief Executa o menu de gerenciamento e edição do perfil da pessoa logada.
    /// @param usuarioLogado Referência para o usuário autenticado na sessão.
    ///
    virtual void gerenciarConta(Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço de negócio.
    /// @param servico Ponteiro para uma implementação concreta de IServicoPessoas.
    ///
    virtual void setServico(IServicoPessoas* servico) = 0;
};

// =============================================================================
// MÓDULO 4: PROJETOS
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Projetos.
/// @details Declara as regras de negócio e persistência para Projetos Kanban.
///
class IServicoProjetos {
public:
    virtual ~IServicoProjetos() = default;

    ///
    /// @brief Registra um novo projeto associado ao Gestor criador.
    /// @param projeto Objeto Projeto validado.
    /// @param emailGestor E-mail do usuário criador (deve possuir papel GESTOR).
    /// @return true se criado com sucesso; false se o ID já existir ou o usuário não for Gestor.
    ///
    virtual bool criar(const Projeto& projeto, const Email& emailGestor) = 0;

    ///
    /// @brief Consulta os detalhes de um projeto a partir do seu identificador.
    /// @param id Identificador único do projeto.
    /// @param projeto Referência do objeto onde os dados do projeto serão gravados.
    /// @return true se o projeto for localizado; false caso contrário.
    ///
    virtual bool ler(const Identificador& id, Projeto& projeto) = 0;

    ///
    /// @brief Atualiza os dados de um projeto existente (Nome, Descrição, Início ou Término).
    /// @param projeto Objeto Projeto com os dados atualizados.
    /// @return true se atualizado com sucesso; false se o projeto não for encontrado.
    ///
    virtual bool atualizar(const Projeto& projeto) = 0;

    ///
    /// @brief Remove um projeto e encerra seus vínculos.
    /// @param id Identificador do projeto a ser excluído.
    /// @return true se excluído com sucesso; false se não existir.
    ///
    virtual bool excluir(const Identificador& id) = 0;

    ///
    /// @brief Lista todos os projetos associados a um determinado Gestor.
    /// @param emailGestor E-mail do Gestor responsável.
    /// @return Vetor de projetos criados pelo gestor informado.
    ///
    virtual std::vector<Projeto> listarPorGestor(const Email& emailGestor) = 0;

    ///
    /// @brief Lista todos os projetos cadastrados no sistema (acessível para Gestor e Desenvolvedor).
    /// @return Vetor contendo todos os projetos existentes.
    ///
    virtual std::vector<Projeto> listarTodos() = 0;
};

///
/// @brief Interface da Camada de Apresentação para o Módulo de Projetos.
/// @details Responsável pelas telas de listagem, criação, alteração e seleção de projetos.
///
class IApresentacaoProjetos {
public:
    virtual ~IApresentacaoProjetos() = default;

    ///
    /// @brief Executa o menu de projetos para o usuário autenticado.
    /// @param usuarioLogado Dados da pessoa autenticada para controle de permissões (GESTOR vs DESENVOLVEDOR).
    ///
    virtual void executar(const Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço de negócio.
    /// @param servico Ponteiro para uma implementação concreta de IServicoProjetos.
    ///
    virtual void setServico(IServicoProjetos* servico) = 0;
};

// =============================================================================
// MÓDULO 5: QUADROS
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Quadros.
/// @details Gerencia os Quadros Kanban, seus Cartões de Atividade associados,
///          o controle estrito do Limite de WIP na coluna FAZENDO e o cálculo de métricas.
///
class IServicoQuadros {
public:
    virtual ~IServicoQuadros() = default;

    // --- Serviços de Quadro ---
    virtual bool criarQuadro(const Quadro& quadro, const Identificador& idProjeto) = 0;
    virtual bool lerQuadro(const Identificador& idQuadro, Quadro& quadro) = 0;
    virtual bool atualizarQuadro(const Quadro& quadro) = 0;
    virtual bool excluirQuadro(const Identificador& idQuadro) = 0;
    virtual std::vector<Quadro> listarQuadrosPorProjeto(const Identificador& idProjeto) = 0;

    // --- Serviços de Cartão de Atividade ---
    virtual bool criarCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro) = 0;
    virtual bool lerCartao(const Identificador& idCartao, CartaoDeAtividade& cartao) = 0;
    virtual bool atualizarCartao(const CartaoDeAtividade& cartao) = 0;
    virtual bool excluirCartao(const Identificador& idCartao) = 0;
    virtual std::vector<CartaoDeAtividade> listarCartoesPorQuadro(const Identificador& idQuadro) = 0;

    // --- Movimentação e Validação de WIP ---
    ///
    /// @brief Movimenta o cartão entre as colunas ("A FAZER" -> "FAZENDO" -> "FEITO").
    /// @details Se o destino for "FAZENDO", verifica se a quantidade de cartões não excede o Limite de WIP do quadro.
    ///          Atualiza os timestamps correspondentes para auditoria de ciclo.
    /// @param idCartao Identificador do cartão.
    /// @param novoEstado Próximo estado do fluxo.
    /// @param momentoTransicao Timestamp do instante da movimentação.
    /// @return true se o movimento for autorizado e concluído; false se violar o limite de WIP.
    ///
    virtual bool moverCartao(const Identificador& idCartao, const Estado& novoEstado, const Timestamp& momentoTransicao) = 0;

    // --- Métricas Kanban ---
    virtual int lerWip(const Identificador& idQuadro) = 0;
    virtual int lerTempoDeCiclo(const Identificador& idCartao) = 0;
    virtual double lerTempoDeCicloMedio(const Identificador& idQuadro) = 0;
    virtual int lerLeadTime(const Identificador& idCartao) = 0;
    virtual double lerLeadTimeMedio(const Identificador& idQuadro) = 0;
};

///
/// @brief Interface da Camada de Apresentação para o Módulo de Quadros.
/// @details Renderiza a visão do quadro Kanban (colunas A FAZER, FAZENDO, FEITO),
///          permitindo manipular cartões e consultar métricas operacionais.
///
class IApresentacaoQuadros {
public:
    virtual ~IApresentacaoQuadros() = default;

    ///
    /// @brief Abre o menu interativo e visual do quadro Kanban.
    /// @param idQuadro Identificador do quadro aberto.
    /// @param usuarioLogado Dados do usuário autenticado para validação de privilégios.
    ///
    virtual void exibirQuadro(const Identificador& idQuadro, const Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço de negócio.
    /// @param servico Ponteiro para uma implementação concreta de IServicoQuadros.
    ///
    virtual void setServico(IServicoQuadros* servico) = 0;
};

#endif // INTERFACES_HPP_INCLUDED

