#ifndef SERVICOS_HPP_INCLUDED
#define SERVICOS_HPP_INCLUDED
 
#include "interfaces.hpp"
#include "containerDados.hpp"
#include <stdexcept>
#include <string>
#include <vector>

///
/// @brief Serviço de negócio do Módulo de Autenticação (processa o login).
/// @details Realiza a interface IServicoAutenticacao. Confere e-mail e senha usando o ContainerPessoas.
///
class ServicoAutenticacao : public IServicoAutenticacao {
public:
    virtual ~ServicoAutenticacao() = default;

    ///
    /// @brief Verifica se o e-mail e a senha informados pertencem a uma conta cadastrada.
    /// @details E-mail inexistente ou senha errada NÃO lançam exceção: o resultado é apenas false.
    /// @param email E-mail digitado no login.
    /// @param senha Senha digitada no login.
    /// @param pessoa Recebe e guarda os dados da conta informados pelo usuário somente se o login for válido.
    /// @return true se as credenciais estão corretas; false caso contrário.
    ///
    bool autenticar(const Email& email, const Senha& senha, Pessoa& pessoa) override;
};

///
/// @brief Implementacao dos servicos de negocio para o Modulo de Pessoas.
/// @details Realiza a interface IServicoPessoas. Utiliza ContainerPessoas e consulta
///          ContainerProjetos para garantir integridade referencial na exclusao e na
///          alteracao de papel de gestor.
///
// Requisitos 1 a 5
class ServicoPessoas : public IServicoPessoas {
public:
    virtual ~ServicoPessoas() = default;

    ///
    /// @brief Cadastra uma nova pessoa no repositorio.
    /// @param pessoa Objeto Pessoa preenchido e validado.
    /// @return true se cadastrada com sucesso.
    /// @throw std::runtime_error Se o e-mail ja estiver cadastrado.
    ///
    // Requisito 1
    bool criar(const Pessoa& pessoa) override;

    ///
    /// @brief Consulta uma pessoa pelo e-mail.
    /// @param email E-mail da conta pesquisada.
    /// @param pessoa Recebe e guarda os dados encontrados em objeto tipo Pessoa.
    /// @return true se localizada; false caso contrario.
    ///
    // Requisito 2
    bool ler(const Email& email, Pessoa& pessoa) override;

    ///
    /// @brief Atualiza Nome, Senha e Papel de uma pessoa.
    /// @details Mantem o e-mail inalterado e impede que gestores com projetos
    ///          ativos alterem seu papel para desenvolvedor.
    /// @param pessoa Recebe e guarda os dados atualizados em objeto Pessoa correspondente ao email informado.
    /// @return true se atualizada com sucesso.
    /// @throw std::runtime_error Se nao for localizada ou se alteração de Papel violar integridade de gestor.
    ///
    // Requisitos 3 e 13
    bool atualizar(const Pessoa& pessoa) override;

    ///
    /// @brief Remove o cadastro de uma pessoa.
    /// @details Impede a exclusao se a pessoa possuir projetos sob sua gestao.
    /// @param email E-mail da conta a ser excluida.
    /// @return true se excluida com sucesso.
    /// @throw std::runtime_error Se nao existir ou se possuir projetos sob sua gestao.
    ///
    // Requisitos 4 e 15
    bool excluir(const Email& email) override;

    ///
    /// @brief Lista todas as pessoas cadastradas.
    /// @return Vetor com as pessoas cadastradas (retorna vazio se não houver pessoas cadastradas no sistema).
    ///
    // Requisito 5
    std::vector<Pessoa> listar() override;
};

///
/// @brief Implementação dos serviço para Projetos, conforme regras de negócio e persistência.
/// @details Realiza a interface IServicoProjetos lançando exceções específicas para falhas de negócio.
///          Utiliza ContainerProjetos e, na exclusão em cascata, ContainerQuadros e ContainerCartoes.
///
// Requisitos de 6 a 10
class ServicoProjetos : public IServicoProjetos {
public:
    virtual ~ServicoProjetos() = default;

    ///
    /// @brief Cria um novo projeto associado ao Gestor criador.
    /// @param projeto Projeto já validado.
    /// @param emailGestor E-mail de quem está criando (precisa ter papel GESTOR).
    /// @return true se criou.
    /// @throw std::logic_error Se o usuário criador não possuir o papel de GESTOR.
    /// @throw std::runtime_error Se o identificador do projeto já estiver cadastrado ou se o gestor não existir.
    ///
    // Requisito 6
    bool criar(const Projeto& projeto, const Email& emailGestor) override;

    ///
    /// @brief Consulta um projeto pelo identificador.
    /// @param id Identificador do projeto.
    /// @param projeto Recebe e guarda os dados encontrados em objeto tipo Projeto.
    /// @return true se encontrou; false se não existe (sem exceção).
    ///
    // Requisito 7
    bool ler(const Identificador& id, Projeto& projeto) override;

    ///
    /// @brief Atualiza os dados de um projeto existente.
    /// @details DAdos atualizáveis são: nome, descrição, início e término. O identificador não pode ser alterado.
    /// @param projeto Projeto com o identificador chave e os novos dados informados pelo usuário.
    /// @return true se atualizou.
    /// @throw std::runtime_error Se o projeto a ser atualizado não existir.
    ///
    // Requisitos 8 e 13
    bool atualizar(const Projeto& projeto) override;

    ///
    /// @brief Remove um projeto e todos os seus quadros e cartões associados.
    /// @details Exclusão realizada em cascata, evitando comprometer a integridade dos containers de dados.
    /// @param id Identificador do projeto informado pelo usuário
    /// @throw std::runtime_error Se o projeto não for encontrado para exclusão.
    ///
    // Requisitos 9 e 15
    bool excluir(const Identificador& id) override;

    ///
    /// @brief Lista os projetos de um gestor.
    /// @param emailGestor E-mail do gestor informado, não precisa ser o mesmo do usuário logado.
    /// @return Vetor com os projetos do gestor (vazio se não houver).
    ///
    std::vector<Projeto> listarPorGestor(const Email& emailGestor) override;

    ///
    /// @brief Lista todos os projetos do sistema.
    /// @return Vetor com todos os projetos (vazio se não houver).
    ///
    // Requisito 10
    std::vector<Projeto> listarTodos() override;
};

///
/// @brief IMplementação dos serviços do Módulo de Quadros.
/// @details Realiza a interface IServicoQuadros: quadros, cartões de atividade, regra do limite de WIP,
///          movimentação entre colunas e métricas (WIP, tempo de ciclo e lead time).
///          Os tempos são calculados em minutos a partir dos Timestamps dos cartões.
///          Observação: a interface não recebe o usuário logado, então a checagem de papel
///          (GESTOR/DESENVOLVEDOR) nessas operações é feita na camada de apresentação.
///
// Requisitos 11 a 26
class ServicoQuadros : public IServicoQuadros {
private:
    /// @brief Informa se um ano é bissexto (usado no cálculo de datas).
    /// @param ano Ano com quatro dígitos.
    /// @return true se o ano for bissexto.
    static bool ehBissexto(int ano);

    /// @brief Converte a sigla de um mês ("JAN" a "DEZ") em número de 0 a 11.
    /// @param mes Sigla do mês, já validada pelo domínio Timestamp.
    /// @return Número do mês (JAN = 0).
    static int numeroDoMes(const std::string& mes);

    /// @brief Converte um Timestamp em minutos corridos desde 01-JAN-2000-00:00.
    /// @details Permite subtrair dois instantes para obter uma duração em minutos.
    /// @param momento Data e hora a converter.
    /// @return Quantidade de minutos desde 01-JAN-2000-00:00.
    static long emMinutos(const Timestamp& momento);

    /// @brief Conta os cartões do quadro que estão na coluna FAZENDO (o WIP).
    /// @param idQuadro Identificador do quadro.
    /// @return Quantidade de cartões em FAZENDO.
    int contarEmFazendo(const Identificador& idQuadro) const;

    /// @brief Garante que o quadro existe.
    /// @param idQuadro Identificador do quadro.
    /// @throw std::runtime_error Se o quadro não existir.
    void exigirQuadro(const Identificador& idQuadro) const;

    /// @brief Busca um cartão e garante que ele já está concluído (FEITO).
    /// @param idCartao Identificador do cartão.
    /// @return Cópia do cartão encontrado.
    /// @throw std::runtime_error Se o cartão não existir ou não estiver em FEITO.
    CartaoDeAtividade exigirCartaoConcluido(const Identificador& idCartao) const;

    /// @brief Remove todos os cartões de um quadro (parte da exclusão em cascata).
    /// @param idQuadro Identificador do quadro.
    void removerCartoesDoQuadro(const Identificador& idQuadro) const;

public:
    virtual ~ServicoQuadros() = default;

    ///
    /// @brief Cria um quadro dentro de um projeto.
    /// @param quadro Quadro já validado pelos domínios.
    /// @param idProjeto Projeto que será dono do quadro.
    /// @return true se criou.
    /// @throw std::runtime_error Se o projeto não existir ou o identificador do quadro já existir.
    ///
    // Requisito 11
    bool criarQuadro(const Quadro& quadro, const Identificador& idProjeto) override;

    ///
    /// @brief Consulta um quadro pelo identificador.
    /// @param idQuadro Identificador do quadro.
    /// @param quadro Recebe os dados encontrados.
    /// @return true se encontrou; false se não existe (sem exceção).
    ///
    // Requisito 12
    bool lerQuadro(const Identificador& idQuadro, Quadro& quadro) override;

    ///
    /// @brief Atualiza nome e limite de WIP de um quadro.
    /// @details Um novo limite menor que o WIP atual (cartões em FAZENDO) é recusado.
    /// @param quadro Quadro com o identificador chave e os novos dados.
    /// @return true se atualizou.
    /// @throw std::runtime_error Se o quadro não existir ou o novo limite for menor que o WIP atual.
    ///
    // Requisito 13
    bool atualizarQuadro(const Quadro& quadro) override;

    ///
    /// @brief Exclui um quadro e todos os seus cartões.
    /// @param idQuadro Identificador do quadro.
    /// @return true se excluiu.
    /// @throw std::runtime_error Se o quadro não existir.
    ///
    // Requisito 14
    bool excluirQuadro(const Identificador& idQuadro) override;

    ///
    /// @brief Lista os quadros de um projeto.
    /// @param idProjeto Identificador do projeto.
    /// @return Vetor com os quadros (vazio se não houver).
    ///
    // Requisito 15
    std::vector<Quadro> listarQuadrosPorProjeto(const Identificador& idProjeto) override;

    ///
    /// @brief Cria um cartão em um quadro.
    /// @details O cartão sempre nasce na coluna A FAZER, mesmo que o objeto recebido tenha outro estado.
    /// @param cartao Cartão já validado pelos domínios.
    /// @param idQuadro Quadro que receberá o cartão.
    /// @return true se criou.
    /// @throw std::runtime_error Se o quadro não existir ou o identificador do cartão já existir.
    ///
    // Requisito 16
    bool criarCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro) override;

    ///
    /// @brief Consulta um cartão pelo identificador.
    /// @param idCartao Identificador do cartão.
    /// @param cartao Recebe os dados encontrados.
    /// @return true se encontrou; false se não existe (sem exceção).
    ///
    // Requisito 17
    bool lerCartao(const Identificador& idCartao, CartaoDeAtividade& cartao) override;

    ///
    /// @brief Atualiza nome, descrição, prioridade e tamanho de um cartão.
    /// @details Estado e timestamps são mantidos como estão: só moverCartao pode mudá-los.
    /// @param cartao Cartão com o identificador chave e os novos dados.
    /// @return true se atualizou.
    /// @throw std::runtime_error Se o cartão não existir.
    ///
    // Requisito 18
    bool atualizarCartao(const CartaoDeAtividade& cartao) override;

    ///
    /// @brief Exclui um cartão.
    /// @param idCartao Identificador do cartão.
    /// @return true se excluiu.
    /// @throw std::runtime_error Se o cartão não existir.
    ///
    // Requisito 19
    bool excluirCartao(const Identificador& idCartao) override;

    ///
    /// @brief Lista os cartões de um quadro.
    /// @param idQuadro Identificador do quadro.
    /// @return Vetor com os cartões (vazio se não houver).
    ///
    // Requisito 20
    std::vector<CartaoDeAtividade> listarCartoesPorQuadro(const Identificador& idQuadro) override;

    ///
    /// @brief Move um cartão de coluna de um estado para outro.
    /// @details Só são aceitas as passagens A FAZER -> FAZENDO e FAZENDO -> FEITO. Ao entrar em FAZENDO,
    ///          o WIP não pode passar do limite do quadro. O horário da movimentação é gravado no cartão
    ///          (início ao entrar em FAZENDO, término ao entrar em FEITO) e não pode ser anterior ao horário anterior.
    /// @param idCartao Identificador do cartão.
    /// @param novoEstado Coluna de destino.
    /// @param momentoTransicao Data e hora da movimentação.
    /// @return true se moveu.
    /// @throw std::runtime_error Se o cartão não existir, a transição for inválida, o horário for anterior
    ///        ao registrado ou o limite de WIP for excedido.
    ///
    // Requisito 21
    bool moverCartao(const Identificador& idCartao, const Estado& novoEstado, const Timestamp& momentoTransicao) override;

    ///
    /// @brief Informa o WIP do quadro: quantos cartões estão em FAZENDO.
    /// @param idQuadro Identificador do quadro.
    /// @return Quantidade de cartões em FAZENDO.
    /// @throw std::runtime_error Se o quadro não existir.
    ///
    // Requisito 26
    int lerWip(const Identificador& idQuadro) override;

    ///
    /// @brief Tempo de ciclo de um cartão: de FAZENDO até FEITO.
    /// @param idCartao Identificador do cartão.
    /// @return Tempo de ciclo em minutos.
    /// @throw std::runtime_error Se o cartão não existir ou ainda não estiver em FEITO.
    ///
    // Requisito 22
    int lerTempoDeCiclo(const Identificador& idCartao) override;

    ///
    /// @brief Tempo de ciclo médio dos cartões concluídos de um quadro.
    /// @param idQuadro Identificador do quadro.
    /// @return Média em minutos; 0 se não houver cartões em FEITO.
    /// @throw std::runtime_error Se o quadro não existir.
    ///
    // Requisito 23
    double lerTempoDeCicloMedio(const Identificador& idQuadro) override;

    ///
    /// @brief Lead time de um cartão: da entrada em A FAZER até FEITO.
    /// @param idCartao Identificador do cartão.
    /// @return Lead time em minutos.
    /// @throw std::runtime_error Se o cartão não existir ou ainda não estiver em FEITO.
    ///
    // Requisito 24
    int lerLeadTime(const Identificador& idCartao) override;

    ///
    /// @brief Lead time médio dos cartões concluídos de um quadro.
    /// @param idQuadro Identificador do quadro.
    /// @return Média em minutos; 0 se não houver cartões em FEITO.
    /// @throw std::runtime_error Se o quadro não existir.
    ///
    // Requisito 25
    double lerLeadTimeMedio(const Identificador& idQuadro) override;
};

#endif // SERVICOS_HPP_INCLUDED
