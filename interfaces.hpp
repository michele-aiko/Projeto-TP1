#ifndef INTERFACES_HPP_INCLUDED
#define INTERFACES_HPP_INCLUDED

#include "dominios.hpp"
#include "entidades.hpp"
#include <stdexcept>
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
///          Depende exclusivamente das interfaces abstratas dos demais módulos, nunca de suas implementações.
///
class IApresentacaoControle {
public:
    ///
    /// @brief Destrutor virtual padrão da interface.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IApresentacaoControle() = default;

    ///
    /// @brief Inicia o loop de execução principal do sistema.
    /// @details Mantém o fluxo interativo do console até que o usuário escolha encerrar o programa.
    ///          Antes de iniciar, a implementação deve verificar se todos os módulos dependentes estão disponíveis.
    /// @throw std::runtime_error Se algum módulo de apresentação ou de serviço dependente estiver indisponível.
    ///
    virtual void executar() = 0;
};

// =============================================================================
// MÓDULO 2: AUTENTICAÇÃO
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Autenticação.
/// @details Declara os serviços de negócio que necessitam de validação e autenticação de credenciais de acesso.
///
class IServicoAutenticacao {
public:
    ///
    /// @brief Destrutor virtual padrão da interface de serviços de autenticação.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IServicoAutenticacao() = default;

    ///
    /// @brief Autentica um usuário no sistema com base em seu e-mail e senha.
    /// @details Consulta o cadastro de pessoas e compara a senha informada com a armazenada. Credenciais incorretas
    ///          ou e-mail não cadastrado não são exceção: apenas resultam em retorno false e a referência pessoa não é alterada.
    /// @param email Objeto da classe Email.
    /// @param senha Objeto da classe Senha.
    /// @param pessoa Referência para o objeto Pessoa a ser preenchido caso a autenticação tenha sucesso.
    /// @return true se as credenciais forem válidas e autenticadas; false caso contrário.
    ///
    virtual bool autenticar(const Email& email, const Senha& senha, Pessoa& pessoa) = 0;
};

///
/// @brief Interface da Camada de Apresentação para o Módulo de Autenticação.
/// @details Responsável por solicitar, obter dados e pré-validar as credenciais do usuário na tela de login.
///
class IApresentacaoAutenticacao {
public:
    ///
    /// @brief Destrutor virtual padrão da interface de apresentação de autenticação.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IApresentacaoAutenticacao() = default;

    ///
    /// @brief Executa o fluxo de login e senha no console.
    /// @details Solicita e-mail e senha, constrói os objetos de domínio e aciona o serviço de autenticação.
    ///          Erros de formato são tratados na própria tela e resultam em retorno false.
    /// @param pessoa Referência para o objeto Pessoa preenchido em caso de sucesso.
    /// @return true se o usuário efetuou o login com sucesso; false se cancelou ou falhou.
    /// @throw std::runtime_error Se o serviço IServicoAutenticacao não estiver disponível.
    ///
    virtual bool autenticar(Pessoa& pessoa) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço.
    /// @details Conecta a tela de interação com o usuário ao serviço que ela vai utilizar.
    ///          Deve ser chamado antes que a tela seja disponibilizada.
    /// @param servico Endereço (ponteiro) de um objeto de serviço de autenticação já criado, como um ServicoAutenticacao.
    ///                Esse objeto deve existir enquanto a tela estiver em uso.
    ///
    virtual void setServico(IServicoAutenticacao* servico) = 0;
};

// =============================================================================
// MÓDULO 3: PESSOAS (USUÁRIOS / CONTAS)
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Pessoas.
/// @details Declara as operações de criar, ler, atualizar e excluir (CRUD) dos usuários e controle cadastral
///
//(Requisitos Funcionais 1 a 5)
class IServicoPessoas {
public:
    ///
    /// @brief Destrutor virtual padrão da interface de serviços de Pessoas.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IServicoPessoas() = default;

    ///
    /// @brief Cadastra uma nova pessoa no repositório do sistema.
    /// @details O e-mail é a chave primária: não podem existir duas pessoas com o mesmo e-mail.
    /// @param pessoa Objeto Pessoa devidamente preenchido e validado.
    /// @return true se cadastrado com sucesso.
    /// @throw std::runtime_error Se já existir uma pessoa cadastrada com o e-mail informado.
    ///
    virtual bool criar(const Pessoa& pessoa) = 0;

    ///
    /// @brief Consulta os dados cadastrais de uma pessoa por meio de seu e-mail (chave primária).
    /// @details Não lança exceção para pessoa inexistente: o resultado é informado pelo valor de retorno.
    /// @param email Objeto Email da conta desejada.
    /// @param pessoa Referência do objeto onde os dados encontrados serão gravados.
    /// @return true se a pessoa foi localizada; false caso contrário.
    ///
    virtual bool ler(const Email& email, Pessoa& pessoa) = 0;

    ///
    /// @brief Atualiza as informações cadastrais de uma pessoa (Nome, Senha ou Papel).
    /// @details O e-mail (chave primária) não pode ser alterado. Um GESTOR que possua projetos sob sua gestão
    ///          não pode ter o papel alterado para DESENVOLVEDOR, para preservar a integridade referencial.
    /// @param pessoa Objeto contendo o e-mail chave e os novos atributos de domínio.
    /// @return true se atualizado com sucesso.
    /// @throw std::runtime_error Se o registro não for localizado ou se a alteração violar a integridade de gestor.
    ///
    virtual bool atualizar(const Pessoa& pessoa) = 0;

    ///
    /// @brief Remove permanentemente o cadastro de uma pessoa do sistema.
    /// @details A exclusão é bloqueada enquanto a pessoa for gestora de algum projeto, evitando inconsistência.
    /// @param email Objeto Email da conta a ser excluída.
    /// @return true se excluído com sucesso.
    /// @throw std::runtime_error Se o registro não existir ou possuir vínculos que impeçam a exclusão (projetos sob sua gestão).
    ///
    virtual bool excluir(const Email& email) = 0;

    ///
    /// @brief Lista todas as pessoas cadastradas no sistema.
    /// @details Retorna vetor vazio se não houver pessoas cadastradas.
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
    ///
    /// @brief Destrutor virtual padrão da interface de apresentação de Pessoas.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IApresentacaoPessoas() = default;

    ///
    /// @brief Executa a tela de criação de nova conta (auto-registro de usuário).
    /// @details Captura e-mail, nome, senha e papel, valida os formatos pelas classes de domínio e aciona o serviço.
    ///          Erros de formato e de negócio são exibidos na própria tela e resultam em retorno false.
    /// @return true se a conta foi criada com sucesso; false se cancelada ou falha.
    /// @throw std::runtime_error Se o serviço IServicoPessoas não estiver disponível.
    ///
    virtual bool criarConta() = 0;

    ///
    /// @brief Executa o menu de gerenciamento e edição do perfil da pessoa logada.
    /// @details Oferece leitura, atualização, exclusão da própria conta e listagem de pessoas. A referência recebida
    ///          é mantida em sincronia com as alterações feitas (por exemplo, novo nome ou novo papel).
    /// @param usuarioLogado Referência para o usuário autenticado na sessão.
    /// @throw std::runtime_error Se o serviço IServicoPessoas não estiver disponível.
    ///
    virtual void gerenciarConta(Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço.
    /// @details Conecta a tela de interação com o usuário ao serviço que ela vai utilizar.
    ///          Deve ser chamado antes que a tela seja disponibilizada.
    /// @param servico Endereço (ponteiro) de um objeto de serviço de autenticação já criado, como um ServicoAutenticacao.
    ///                Esse objeto deve existir enquanto a tela estiver em uso.
    ///
    virtual void setServico(IServicoPessoas* servico) = 0;
};

// =============================================================================
// MÓDULO 4: PROJETOS
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Projetos.
/// @details Declara as regras de negócio e persistência para Projetos Kanban
///
//(Requisitos Funcionais 6 a 10)
class IServicoProjetos {
public:
    ///
    /// @brief Destrutor virtual padrão da interface de serviços de Projetos.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IServicoProjetos() = default;

    ///
    /// @brief Registra um novo projeto associado a um usuário Gestor.
    /// @details O gestor do projeto é, por definição, a pessoa que o criou. O identificador é chave primária única.
    /// @param projeto Objeto Projeto validado.
    /// @param emailGestor E-mail do usuário criador (deve possuir papel GESTOR).
    /// @return true se criado com sucesso.
    /// @throw std::logic_error Se o usuário criador não possuir o papel GESTOR.
    /// @throw std::runtime_error Se o identificador do projeto já existir ou se o usuário criador não for encontrado.
    ///
    virtual bool criar(const Projeto& projeto, const Email& emailGestor) = 0;

    ///
    /// @brief Consulta os detalhes de um projeto a partir do seu identificador.
    /// @details Disponível para GESTOR e DESENVOLVEDOR. Não lança exceção para projeto inexistente:
    ///          o resultado é informado pelo valor de retorno e a referência projeto não é alterada.
    /// @param id Identificador único do projeto.
    /// @param projeto Referência do objeto onde os dados do projeto serão gravados.
    /// @return true se o projeto for localizado; false caso contrário.
    ///
    virtual bool ler(const Identificador& id, Projeto& projeto) = 0;

    ///
    /// @brief Atualiza os dados de um projeto existente (Nome, Descrição, Início ou Término).
    /// @details O identificador (chave primária) não pode ser alterado.
    /// @param projeto Objeto Projeto com os dados atualizados.
    /// @return true se atualizado com sucesso.
    /// @throw std::runtime_error Se o projeto não for encontrado.
    ///
    virtual bool atualizar(const Projeto& projeto) = 0;

    ///
    /// @brief Remove um projeto e encerra seus vínculos.
    /// @details A exclusão é feita em cascata: os quadros do projeto e os cartões de atividade desses quadros
    ///          também são removidos, evitando registros órfãos.
    /// @param id Identificador do projeto a ser excluído.
    /// @return true se excluído com sucesso.
    /// @throw std::runtime_error Se o projeto não for encontrado.
    ///
    virtual bool excluir(const Identificador& id) = 0;

    ///
    /// @brief Lista todos os projetos associados a um determinado Gestor.
    /// @details Retorna vetor vazio se o gestor não possuir projetos.
    /// @param emailGestor E-mail do Gestor responsável.
    /// @return Vetor de projetos criados pelo gestor informado.
    ///
    virtual std::vector<Projeto> listarPorGestor(const Email& emailGestor) = 0;

    ///
    /// @brief Lista todos os projetos cadastrados no sistema (acessível para Gestor e Desenvolvedor).
    /// @details Retorna vetor vazio se não houver projetos cadastrados.
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
    ///
    /// @brief Destrutor virtual padrão da interface de apresentação de Projetos.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IApresentacaoProjetos() = default;

    ///
    /// @brief Executa o menu de projetos para o usuário autenticado.
    /// @details As opções de criação e alteração são exibidas somente para o papel GESTOR; o DESENVOLVEDOR
    ///          dispõe apenas de leitura e listagem. Erros de formato, de permissão e de negócio são tratados na tela.
    /// @param usuarioLogado Dados da pessoa autenticada para controle de permissões (GESTOR / DESENVOLVEDOR).
    /// @throw std::runtime_error Se o serviço IServicoProjetos não estiver disponível
    ///
    virtual void executar(const Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço.
    /// @details Conecta a tela de interação com o usuário ao serviço que ela vai utilizar.
    ///          Deve ser chamado antes que a tela seja disponibilizada.
    /// @param servico Endereço (ponteiro) de um objeto de serviço de autenticação já criado, como um ServicoAutenticacao.
    ///                Esse objeto deve existir enquanto a tela estiver em uso.
    ///
    virtual void setServico(IServicoProjetos* servico) = 0;
};

// =============================================================================
// MÓDULO 5: QUADROS
// =============================================================================

///
/// @brief Interface da Camada de Serviço para o Módulo de Quadros.
/// @details Gerencia os Quadros Kanban, seus Cartões de Atividade associados,
///          controle do Limite de WIP na coluna FAZENDO e o cálculo de métricas
///
// (Requisitos Funcionais 11 a 26)
class IServicoQuadros {
public:
    ///
    /// @brief Destrutor virtual padrão da interface de serviços de Quadros.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IServicoQuadros() = default;

    ///
    /// @brief Cria um novo quadro vinculado a um projeto existente.
    /// @details O identificador do quadro é chave primária única.
    /// @param quadro Objeto Quadro devidamente preenchido e validado.
    /// @param idProjeto Identificador do projeto proprietário do quadro.
    /// @return true se criado com sucesso.
    /// @throw std::runtime_error Se o identificador do quadro já existir ou se o projeto não existir.
    ///
    virtual bool criarQuadro(const Quadro& quadro, const Identificador& idProjeto) = 0;

    ///
    /// @brief Consulta os dados de um quadro.
    /// @details Disponível para GESTOR e DESENVOLVEDOR. Não lança exceção para quadro inexistente.
    /// @param idQuadro Identificador do quadro pesquisado.
    /// @param quadro Referência onde os dados do quadro serão gravados.
    /// @return true se o quadro for localizado; false caso contrário.
    ///
    virtual bool lerQuadro(const Identificador& idQuadro, Quadro& quadro) = 0;

    ///
    /// @brief Atualiza Nome e Limite de WIP de um quadro
    /// @details O identificador (chave primária) não pode ser alterado. Um novo limite menor que o WIP atual
    ///          não pode ser aceito, para não deixar o quadro em estado inconsistente.
    /// @param quadro Objeto Quadro contendo o identificador chave e os novos atributos.
    /// @return true se atualizado com sucesso.
    /// @throw std::runtime_error Se o quadro não for localizado ou se o novo limite for menor que o WIP atual.
    ///
    virtual bool atualizarQuadro(const Quadro& quadro) = 0;

    ///
    /// @brief Remove um quadro e todos os seus cartões de atividade.
    /// @details A exclusão é feita em cascata sobre os cartões do quadro, evitando registros órfãos.
    /// @param idQuadro Identificador do quadro a ser excluído.
    /// @return true se excluído com sucesso.
    /// @throw std::runtime_error Se o quadro não for localizado.
    ///
    virtual bool excluirQuadro(const Identificador& idQuadro) = 0;

    ///
    /// @brief Lista os quadros vinculados a um projeto.
    /// @details Retorna vetor vazio se o projeto não possuir quadros.
    /// @param idProjeto Identificador do projeto pesquisado.
    /// @return Vetor contendo os quadros do projeto.
    ///
    virtual std::vector<Quadro> listarQuadrosPorProjeto(const Identificador& idProjeto) = 0;

    ///
    /// @brief Cria um novo cartão de atividade em um quadro.
    /// @details O identificador do cartão é chave primária única e o quadro informado deve existir.
    ///          O cartão é inicializado na coluna A FAZER.
    /// @param cartao Objeto CartaoDeAtividade devidamente preenchido e validado.
    /// @param idQuadro Identificador do quadro que receberá o cartão.
    /// @return true se criado com sucesso.
    /// @throw std::runtime_error Se o identificador do cartão já existir ou se o quadro não existir.
    ///
    virtual bool criarCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro) = 0;

    ///
    /// @brief Consulta os dados de um cartão de atividade.
    /// @details Disponível para GESTOR e DESENVOLVEDOR. Não lança exceção para cartão inexistente.
    /// @param idCartao Identificador do cartão pesquisado.
    /// @param cartao Referência onde os dados do cartão serão gravados.
    /// @return true se o cartão for localizado; false caso contrário.
    ///
    virtual bool lerCartao(const Identificador& idCartao, CartaoDeAtividade& cartao) = 0;

    ///
    /// @brief Atualiza os dados de um cartão de atividade
    /// @details O identificador (chave primária) não pode ser alterado. A mudança de coluna deve ser feita
    ///          exclusivamente por moverCartao, que aplica a regra de WIP.
    /// @param cartao Objeto CartaoDeAtividade contendo o identificador chave e os novos atributos.
    /// @return true se atualizado com sucesso.
    /// @throw std::runtime_error Se o cartão não for localizado.
    ///
    virtual bool atualizarCartao(const CartaoDeAtividade& cartao) = 0;

    ///
    /// @brief Remove um cartão de atividade.
    /// @details Remove também o vínculo do cartão com o seu quadro.
    /// @param idCartao Identificador do cartão a ser excluído.
    /// @return true se excluído com sucesso.
    /// @throw std::runtime_error Se o cartão não for localizado.
    ///
    virtual bool excluirCartao(const Identificador& idCartao) = 0;

    ///
    /// @brief Lista os cartões de atividade de um quadro.
    /// @details Retorna vetor vazio se o quadro não possuir cartões.
    /// @param idQuadro Identificador do quadro pesquisado.
    /// @return Vetor contendo os cartões do quadro.
    ///
    virtual std::vector<CartaoDeAtividade> listarCartoesPorQuadro(const Identificador& idQuadro) = 0;

    ///
    /// @brief Movimenta o cartão entre as colunas ("A FAZER" -> "FAZENDO" -> "FEITO").
    /// @details Somente são permitidas as transições A FAZER -> FAZENDO e FAZENDO -> FEITO.
    ///          Se o destino for "FAZENDO", verifica se a quantidade de cartões não excede o Limite de WIP do quadro.
    ///          Atualiza os timestamps correspondentes.
    /// @param idCartao Identificador do cartão.
    /// @param novoEstado Próximo estado do fluxo.
    /// @param momentoTransicao Timestamp do instante da movimentação.
    /// @return true se o movimento for autorizado e concluído.
    /// @throw std::runtime_error Se o cartão não for localizado, se a transição não for permitida
    ///        ou se o movimento violar o limite de WIP do quadro.
    ///
    virtual bool moverCartao(const Identificador& idCartao, const Estado& novoEstado, const Timestamp& momentoTransicao) = 0;

    ///
    /// @brief Obtém o WIP do quadro, ou seja, a quantidade de cartões na coluna FAZENDO.
    /// @details O valor nunca excede o Limite de WIP configurado no quadro.
    /// @param idQuadro Identificador do quadro pesquisado.
    /// @return Número de cartões em FAZENDO.
    /// @throw std::runtime_error Se o quadro não for localizado.
    ///
    virtual int lerWip(const Identificador& idQuadro) = 0;

    ///
    /// @brief Obtém o tempo de ciclo de um cartão.
    /// @details Calculado desde o momento em que o cartão passa para o estado FAZENDO até chegar em FEITO.
    /// @param idCartao Identificador do cartão pesquisado.
    /// @return Tempo de ciclo do cartão em minutos.
    /// @throw std::runtime_error Se o cartão não for localizado ou ainda não tiver sido concluído (FEITO).
    ///
    virtual int lerTempoDeCiclo(const Identificador& idCartao) = 0;

    ///
    /// @brief Obtém o tempo de ciclo médio de um quadro.
    /// @details Média dos tempos de ciclo dos cartões concluídos do quadro; retorna 0 se não houver cartões concluídos.
    /// @param idQuadro Identificador do quadro pesquisado.
    /// @return Tempo de ciclo médio do quadro em minutos.
    /// @throw std::runtime_error Se o quadro não for localizado.
    ///
    virtual double lerTempoDeCicloMedio(const Identificador& idQuadro) = 0;

    ///
    /// @brief Obtém o lead time de um cartão.
    /// @details Calculado desde o momento em que o cartão passa para o estado A FAZER até chegar em FEITO.
    /// @param idCartao Identificador do cartão pesquisado.
    /// @return Lead time do cartão em minutos.
    /// @throw std::runtime_error Se o cartão não for localizado ou ainda não tiver sido concluído (FEITO).
    ///
    virtual int lerLeadTime(const Identificador& idCartao) = 0;

    ///
    /// @brief Obtém o lead time médio de um quadro.
    /// @details Média dos lead times dos cartões concluídos do quadro; retorna 0 se não houver cartões concluídos.
    /// @param idQuadro Identificador do quadro pesquisado.
    /// @return Lead time médio do quadro em minutos.
    /// @throw std::runtime_error Se o quadro não for localizado.
    ///
    virtual double lerLeadTimeMedio(const Identificador& idQuadro) = 0;
};

///
/// @brief Interface da Camada de Apresentação para o Módulo de Quadros.
/// @details Apresenta as telas dos quadros Kanban (colunas A FAZER, FAZENDO, FEITO),
///          permitindo manipulação cartões e consulta de métricas operacionais.
///
class IApresentacaoQuadros {
public:
    ///
    /// @brief Destrutor virtual padrão da interface de apresentação de Quadros.
    /// @details Garante a destruição correta de objetos derivados referenciados por ponteiro para esta interface.
    ///
    virtual ~IApresentacaoQuadros() = default;

    ///
    /// @brief Abre tela de interação do usuário com o quadro Kanban.
    /// @details Exibe as colunas com seus cartões, o WIP atual e o limite, e permite criar, ler, atualizar,
    ///          excluir, listar e mover cartões e consultar suas métricas.
    ///          Criação, atualização e exclusão de cartões são restritas ao papel GESTOR.
    /// @param idQuadro Identificador do quadro aberto.
    /// @param usuarioLogado Dados do usuário autenticado para validação de privilégios.
    /// @throw std::runtime_error Se o serviço IServicoQuadros não estiver disponível
    ///
    virtual void exibirQuadro(const Identificador& idQuadro, const Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Executa o menu de gerenciamento dos quadros de um projeto.
    /// @details Permite listar e ler quadros (GESTOR e DESENVOLVEDOR) e criar, atualizar e excluir quadros (somente GESTOR).
    ///          Necessário para que seja possível criar o primeiro quadro de um projeto.
    /// @param idProjeto Identificador do projeto cujos quadros serão gerenciados.
    /// @param usuarioLogado Dados do usuário autenticado para validação de privilégios.
    /// @throw std::runtime_error Se o serviço IServicoQuadros não estiver disponível.
    ///
    virtual void gerenciarQuadros(const Identificador& idProjeto, const Pessoa& usuarioLogado) = 0;

    ///
    /// @brief Vincula a controladora de apresentação ao seu respectivo serviço.
    /// @details Conecta a tela de interação com o usuário ao serviço que ela vai utilizar.
    ///          Deve ser chamado antes que a tela seja disponibilizada.
    /// @param servico Endereço (ponteiro) de um objeto de serviço de autenticação já criado, como um ServicoAutenticacao.
    ///                Esse objeto deve existir enquanto a tela estiver em uso.
    ///
    virtual void setServico(IServicoQuadros* servico) = 0;
};

#endif // INTERFACES_HPP_INCLUDED

