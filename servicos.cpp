#include "servicos.hpp"

// =============================================================================
// SERVIÇO DE AUTENTICAÇÃO
// =============================================================================

bool ServicoAutenticacao::autenticar(const Email& email, const Senha& senha, Pessoa& pessoa) {
    Pessoa cadastrada;
    // Se a pessoa informada não estiver cadastrada retorna falso
    if (!ContainerPessoas::getInstancia()->buscarPessoa(email, cadastrada)) {
        return false;
    }
    // Se a senha informada não corresponder à senha cadastrada retorna falso
    if (cadastrada.getSenha().getSenha() != senha.getSenha()) {
        return false;
    }
    pessoa = cadastrada;
    return true;
}

// =============================================================================
// SERVIÇO DE PESSOAS
// =============================================================================

bool ServicoPessoas::criar(const Pessoa& pessoa) {
    //Lança excessão para tentativa de criação de novo usuário com email já cadastrado
    if (!ContainerPessoas::getInstancia()->inserirPessoa(pessoa)) {
        throw std::runtime_error("ServicoPessoas: Ja existe uma pessoa cadastrada com o e-mail '" +
                                 pessoa.getEmail().getEmail() + "'.");
    }
    return true;
}

bool ServicoPessoas::ler(const Email& email, Pessoa& pessoa) {
    //O retorno será falso se a pessoa informada não for encontrada
    return ContainerPessoas::getInstancia()->buscarPessoa(email, pessoa);
}

bool ServicoPessoas::atualizar(const Pessoa& pessoa) {
    Pessoa existente;
    //Confirma se a pessoa informada está cadastrada antes de processar a atualização de dados
    if (!ContainerPessoas::getInstancia()->buscarPessoa(pessoa.getEmail(), existente)) {
        throw std::runtime_error("ServicoPessoas: Falha ao atualizar. Pessoa com e-mail '" +
                                 pessoa.getEmail().getEmail() + "' nao existe.");
    }

    // Impede alteração de papel de gestor para desenvolvedor se houver projetos vinculados
    // Requisito 15
    if (existente.getPapel().getPapel() == Papel::GESTOR &&
        pessoa.getPapel().getPapel() == Papel::DESENVOLVEDOR) {
        std::vector<Projeto> projetosVinculados = ContainerProjetos::getInstancia()->listarProjetosPorGestor(pessoa.getEmail());
        if (!projetosVinculados.empty()) {
            throw std::runtime_error("ServicoPessoas: Nao e possivel alterar papel para DESENVOLVEDOR pois o usuario possui projetos vinculados sob sua gestao.");
        }
    }

    if (!ContainerPessoas::getInstancia()->atualizarPessoa(pessoa)) {
        throw std::runtime_error("ServicoPessoas: Falha ao acessar dados para atualizacao da pessoa '" +
                                 pessoa.getEmail().getEmail() + "'. Tente novamente.");
    }
    return true;
}

bool ServicoPessoas::excluir(const Email& email) {
    if (!ContainerPessoas::getInstancia()->existePessoa(email)) {
        throw std::runtime_error("ServicoPessoas: Falha ao excluir. Pessoa com e-mail '" +
                                 email.getEmail() + "' nao localizada.");
    }

    // Impede a exclusao de gestores com projetos ativos
    std::vector<Projeto> projetosVinculados = ContainerProjetos::getInstancia()->listarProjetosPorGestor(email);
    if (!projetosVinculados.empty()) {
        throw std::runtime_error("ServicoPessoas: Nao e possivel excluir a conta '" + email.getEmail() +
                                 "' pois possui projetos vinculados sob sua gestao.");
    }

    ContainerPessoas::getInstancia()->removerPessoa(email);
    return true;
}

std::vector<Pessoa> ServicoPessoas::listar() {
    return ContainerPessoas::getInstancia()->listarPessoas();
}

// =============================================================================
// SERVIÇO DE PROJETOS
// =============================================================================

bool ServicoProjetos::criar(const Projeto& projeto, const Email& emailGestor) {
    Pessoa gestor;
    if (!ContainerPessoas::getInstancia()->buscarPessoa(emailGestor, gestor)) {
        throw std::runtime_error("ServicoProjetos: Gestor responsavel nao encontrado no sistema.");
    }

    // Validação de Permissão para criar projetos
    if (gestor.getPapel().getPapel() != Papel::GESTOR) {
        throw std::logic_error("ServicoProjetos: Permissao negada. Apenas usuarios com papel GESTOR podem criar projetos.");
    }

    // Verificação de existência de projeto com o identificador informado
    if (!ContainerProjetos::getInstancia()->inserirProjeto(projeto, emailGestor)) {
        throw std::runtime_error("ServicoProjetos: Ja existe um projeto cadastrado com o identificador '" +
                                 projeto.getIdentificador().getIdentificador() + "'.");
    }

    return true;
}

bool ServicoProjetos::ler(const Identificador& id, Projeto& projeto) {
    return ContainerProjetos::getInstancia()->buscarProjeto(id, projeto);
}

bool ServicoProjetos::atualizar(const Projeto& projeto) {
    if (!ContainerProjetos::getInstancia()->atualizarProjeto(projeto)) {
        throw std::runtime_error("ServicoProjetos: Falha ao atualizar. Projeto '" +
                                 projeto.getIdentificador().getIdentificador() + "' nao existe.");
    }
    return true;
}

bool ServicoProjetos::excluir(const Identificador& id) {
    if (!ContainerProjetos::getInstancia()->existeProjeto(id)) {
        throw std::runtime_error("ServicoProjetos: Falha ao excluir. Projeto '" +
                                 id.getIdentificador() + "' nao localizado.");
    }

    // Cascata: o ServicoQuadros apaga os cartões de cada quadro e o próprio quadro
    ServicoQuadros servicoQuadros;
    for (const auto& q : ContainerQuadros::getInstancia()->listarQuadrosPorProjeto(id)) {
        servicoQuadros.excluirQuadro(q.getIdentificador());
    }
    ContainerProjetos::getInstancia()->removerProjeto(id);
    return true;
}

std::vector<Projeto> ServicoProjetos::listarPorGestor(const Email& emailGestor) {
    return ContainerProjetos::getInstancia()->listarProjetosPorGestor(emailGestor);
}

std::vector<Projeto> ServicoProjetos::listarTodos() {
    return ContainerProjetos::getInstancia()->listarTodosProjetos();
}


// =============================================================================
// SERVIÇO DE QUADROS - MÉTODOS AUXILIARES PRIVADOS
// =============================================================================

bool ServicoQuadros::ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int ServicoQuadros::numeroDoMes(const std::string& mes) {
    static const char* const MESES[12] = {"JAN", "FEV", "MAR", "ABR", "MAI", "JUN",
                                          "JUL", "AGO", "SET", "OUT", "NOV", "DEZ"};
    for (int i = 0; i < 12; ++i) {
        if (mes == MESES[i]) return i;
    }
    return 0; // O domínio Timestamp já validou a sigla; este caso não ocorre.
}

//Método para converter uma data e hora no formato de texto (`"01-JAN-2024-14:30"`) em valor absoluto de minutos inteiros, considera datas desde 1º de janeiro de 2000.
//Com essa conversão, calcular o Tempo de Ciclo ou Lead Time entre duas datas é feito por subtração simples de dois números inteiros (minutosFim - minutosInicio).
long ServicoQuadros::emMinutos(const Timestamp& momento) {
    static const int DIAS_NO_MES[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const std::string textoData = momento.getTimeStamp(); // formato DIA-MES-ANO-HH:MM
    //Identificação das posições dos hífens que separam dia, mês, ano e horas
    const std::size_t posDia = textoData.find('-');
    const std::size_t posMes = textoData.find('-', posDia + 1);
    const std::size_t posAno = textoData.find('-', posMes + 1);

    const int dia = std::stoi(textoData.substr(0, posDia));
    const int mes = numeroDoMes(textoData.substr(posDia + 1, posMes - posDia - 1));
    const int ano = std::stoi(textoData.substr(posMes + 1, posAno - posMes - 1));
    const int hora = std::stoi(textoData.substr(posAno + 1, 2));
    const int minuto = std::stoi(textoData.substr(posAno + 4, 2));

    long totalDias = 0;
    for (int i = 2000; i < ano; ++i) totalDias += ehBissexto(i) ? 366 : 365;
    for (int j = 0; j < mes; ++j) {
        totalDias += DIAS_NO_MES[j];
        if (j == 1 && ehBissexto(ano)) totalDias += 1; 
    }
    totalDias += dia - 1;
    //O sufixo `L` nos números avisa o compilador C++ para realizar todas as multiplicações usando o tipo `long`
    return totalDias * 24L * 60L + hora * 60L + minuto;
}

int ServicoQuadros::contarEmFazendo(const Identificador& idQuadro) const {
    int total = 0;
    for (const auto& c : ContainerCartoes::getInstancia()->listarCartoesPorQuadro(idQuadro)) {
        if (c.getEstado().getEstado() == Estado::FAZENDO) ++total;
    }
    return total;
}

void ServicoQuadros::exigirQuadro(const Identificador& idQuadro) const {
    if (!ContainerQuadros::getInstancia()->existeQuadro(idQuadro)) {
        throw std::runtime_error("ServicoQuadros: Quadro '" + idQuadro.getIdentificador() + "' nao encontrado.");
    }
}

CartaoDeAtividade ServicoQuadros::exigirCartaoConcluido(const Identificador& idCartao) const {
    CartaoDeAtividade cartao;
    if (!ContainerCartoes::getInstancia()->buscarCartao(idCartao, cartao)) {
        throw std::runtime_error("ServicoQuadros: Cartao '" + idCartao.getIdentificador() + "' nao encontrado.");
    }
    if (cartao.getEstado().getEstado() != Estado::FEITO) {
        throw std::runtime_error("ServicoQuadros: Cartao '" + idCartao.getIdentificador() +
                                 "' ainda nao foi concluido (FEITO).");
    }
    return cartao;
}

void ServicoQuadros::removerCartoesDoQuadro(const Identificador& idQuadro) const {
    ContainerCartoes* cartoes = ContainerCartoes::getInstancia();
    for (const auto& c : cartoes->listarCartoesPorQuadro(idQuadro)) {
        cartoes->removerCartao(c.getIdentificador());
    }
}

// =============================================================================
// SERVIÇO DE QUADROS - QUADROS
// =============================================================================

bool ServicoQuadros::criarQuadro(const Quadro& quadro, const Identificador& idProjeto) {
    if (!ContainerProjetos::getInstancia()->existeProjeto(idProjeto)) {
        throw std::runtime_error("ServicoQuadros: Projeto '" + idProjeto.getIdentificador() + "' nao encontrado.");
    }
    if (!ContainerQuadros::getInstancia()->inserirQuadro(quadro, idProjeto)) {
        throw std::runtime_error("ServicoQuadros: Ja existe um quadro com o identificador '" +
                                 quadro.getIdentificador().getIdentificador() + "'.");
    }
    return true;
}

bool ServicoQuadros::lerQuadro(const Identificador& idQuadro, Quadro& quadro) {
    return ContainerQuadros::getInstancia()->buscarQuadro(idQuadro, quadro);
}

bool ServicoQuadros::atualizarQuadro(const Quadro& quadro) {
    const Identificador id = quadro.getIdentificador();
    exigirQuadro(id);

    if (quadro.getLimite().getLimite() < contarEmFazendo(id)) {
        throw std::runtime_error("ServicoQuadros: O novo limite de WIP e menor que a quantidade atual de cartoes em FAZENDO.");
    }
    if (!ContainerQuadros::getInstancia()->atualizarQuadro(quadro)) {
        throw std::runtime_error("ServicoQuadros: Falha ao atualizar o quadro '" + id.getIdentificador() + "'.");
    }
    return true;
}

bool ServicoQuadros::excluirQuadro(const Identificador& idQuadro) {
    exigirQuadro(idQuadro);
    removerCartoesDoQuadro(idQuadro);
    ContainerQuadros::getInstancia()->removerQuadro(idQuadro);
    return true;
}

std::vector<Quadro> ServicoQuadros::listarQuadrosPorProjeto(const Identificador& idProjeto) {
    return ContainerQuadros::getInstancia()->listarQuadrosPorProjeto(idProjeto);
}

// =============================================================================
// SERVIÇO DE QUADROS - CARTÕES DE ATIVIDADE
// =============================================================================

bool ServicoQuadros::criarCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro) {
    exigirQuadro(idQuadro);

    CartaoDeAtividade novo = cartao;
    novo.setEstado(Estado(Estado::A_FAZER)); // todo cartão nasce em A FAZER

    if (!ContainerCartoes::getInstancia()->inserirCartao(novo, idQuadro)) {
        throw std::runtime_error("ServicoQuadros: Ja existe um cartao com o identificador '" +
                                 cartao.getIdentificador().getIdentificador() + "'.");
    }
    return true;
}

bool ServicoQuadros::lerCartao(const Identificador& idCartao, CartaoDeAtividade& cartao) {
    return ContainerCartoes::getInstancia()->buscarCartao(idCartao, cartao);
}

bool ServicoQuadros::atualizarCartao(const CartaoDeAtividade& cartao) {
    CartaoDeAtividade existente;
    if (!ContainerCartoes::getInstancia()->buscarCartao(cartao.getIdentificador(), existente)) {
        throw std::runtime_error("ServicoQuadros: Falha ao atualizar. Cartao '" +
                                 cartao.getIdentificador().getIdentificador() + "' nao existe.");
    }

    // Estado e timestamps só mudam em moverCartao
    CartaoDeAtividade atualizado = cartao;
    atualizado.setEstado(existente.getEstado());
    atualizado.setEntrada(existente.getEntrada());
    atualizado.setInicio(existente.getInicio());
    atualizado.setTermino(existente.getTermino());

    ContainerCartoes::getInstancia()->atualizarCartao(atualizado);
    return true;
}

bool ServicoQuadros::excluirCartao(const Identificador& idCartao) {
    if (!ContainerCartoes::getInstancia()->existeCartao(idCartao)) {
        throw std::runtime_error("ServicoQuadros: Falha ao excluir. Cartao '" +
                                 idCartao.getIdentificador() + "' nao localizado.");
    }
    ContainerCartoes::getInstancia()->removerCartao(idCartao);
    return true;
}

std::vector<CartaoDeAtividade> ServicoQuadros::listarCartoesPorQuadro(const Identificador& idQuadro) {
    return ContainerCartoes::getInstancia()->listarCartoesPorQuadro(idQuadro);
}

// =============================================================================
// SERVIÇO DE QUADROS - MOVIMENTAÇÃO E MÉTRICAS
// =============================================================================

bool ServicoQuadros::moverCartao(const Identificador& idCartao, const Estado& novoEstado,
                                 const Timestamp& momentoTransicao) {
    ContainerCartoes* cartoes = ContainerCartoes::getInstancia();

    CartaoDeAtividade cartao;
    if (!cartoes->buscarCartao(idCartao, cartao)) {
        throw std::runtime_error("ServicoQuadros: Cartao '" + idCartao.getIdentificador() + "' nao encontrado.");
    }

    const std::string origem = cartao.getEstado().getEstado();
    const std::string destino = novoEstado.getEstado();

    // Somente A FAZER -> FAZENDO e FAZENDO -> FEITO
    const bool iniciando = (origem == Estado::A_FAZER && destino == Estado::FAZENDO);
    const bool concluindo = (origem == Estado::FAZENDO && destino == Estado::FEITO);
    if (!iniciando && !concluindo) {
        throw std::runtime_error("ServicoQuadros: Movimento invalido de '" + origem + "' para '" + destino +
                                 "'. Permitido: A FAZER -> FAZENDO -> FEITO.");
    }

    // O horário da movimentação não pode ser anterior ao horário em que o cartão entrou no estado atual
    const Timestamp& referencia = iniciando ? cartao.getEntrada() : cartao.getInicio();
    if (emMinutos(momentoTransicao) < emMinutos(referencia)) {
        throw std::runtime_error("ServicoQuadros: O horario da movimentacao e anterior ao horario registrado no cartao.");
    }

    // Regra do WIP: só pode entrar em FAZENDO se WIP atual < limite WIP
    if (iniciando) { 
        Identificador idQuadro; 
        Quadro quadro; 
        
        if (!ContainerCartoes::getInstancia()->obterQuadroDoCartao(idCartao, idQuadro) || !ContainerQuadros::getInstancia()->buscarQuadro(idQuadro, quadro)) { 
            throw std::runtime_error("ServicoQuadros: Quadro do cartao '" + idCartao.getIdentificador() + "' nao encontrado."); 
        } 
        
        if (contarEmFazendo(idQuadro) >= quadro.getLimite().getLimite()) { 
            throw std::runtime_error("ServicoQuadros: Limite de WIP atingido. Conclua um cartao em FAZENDO antes de iniciar outro."); 
        } 
        
        cartao.setInicio(momentoTransicao); 
    
    } else { 
        cartao.setTermino(momentoTransicao); 
    } 
    
    cartao.setEstado(novoEstado); 
    ContainerCartoes::getInstancia()->atualizarCartao(cartao); 
    return true;
}

int ServicoQuadros::lerWip(const Identificador& idQuadro) {
    exigirQuadro(idQuadro);
    return contarEmFazendo(idQuadro);
}

int ServicoQuadros::lerTempoDeCiclo(const Identificador& idCartao) {
    const CartaoDeAtividade cartao = exigirCartaoConcluido(idCartao);
    return static_cast<int>(emMinutos(cartao.getTermino()) - emMinutos(cartao.getInicio()));
}

double ServicoQuadros::lerTempoDeCicloMedio(const Identificador& idQuadro) {
    exigirQuadro(idQuadro);
    double soma = 0.0;
    int concluidos = 0;
    for (const auto& c : ContainerCartoes::getInstancia()->listarCartoesPorQuadro(idQuadro)) {
        if (c.getEstado().getEstado() == Estado::FEITO) {
            soma += static_cast<double>(emMinutos(c.getTermino()) - emMinutos(c.getInicio()));
            ++concluidos;
        }
    }
    return concluidos > 0 ? soma / concluidos : 0.0;
}

int ServicoQuadros::lerLeadTime(const Identificador& idCartao) {
    const CartaoDeAtividade cartao = exigirCartaoConcluido(idCartao);
    return static_cast<int>(emMinutos(cartao.getTermino()) - emMinutos(cartao.getEntrada()));
}

double ServicoQuadros::lerLeadTimeMedio(const Identificador& idQuadro) {
    exigirQuadro(idQuadro);
    double soma = 0.0;
    int concluidos = 0;
    for (const auto& c : ContainerCartoes::getInstancia()->listarCartoesPorQuadro(idQuadro)) {
        if (c.getEstado().getEstado() == Estado::FEITO) {
            soma += static_cast<double>(emMinutos(c.getTermino()) - emMinutos(c.getEntrada()));
            ++concluidos;
        }
    }
    return concluidos > 0 ? soma / concluidos : 0.0;
}
bool ServicoPessoas::criar(const Pessoa& pessoa){
    Pessoa temp;
    if (containerDados::getInstancia()->buscarPessoa(pessoa.getEmail(), temp)){
        throw std::runtime_error("Já existe uma pessoa cadastrada com este e-mail.");
    }

    if (!containerDados::getInstancia()->inserirPessoa(pessoa)){
        throw std::runtime_error("Falha ao cadastrar a pessoa.");
    }

    return true;
}

bool ServicoPessoas::ler(const Email& email, Pessoa& pessoa){
    if (!containerDados::getInstancia()->buscarPessoa(email, pessoa)){
        throw std::runtime_error("Pessoa não encontrada.");
    }

    return true;
}

bool ServicoPessoas::atualizar(const Pessoa& pessoa) {
    if (!ContainerDados::getInstancia()->atualizarPessoa(pessoa)) {
        throw std::runtime_error("ServicoPessoas: Falha ao atualizar. Registro de pessoa não encontrado.");
    }
    return true;
}

bool ServicoPessoas::excluir(const Email& email) {
    Pessoa p;
    if (!ContainerDados::getInstancia()->buscarPessoa(email, p)) {
        throw std::runtime_error("ServicoPessoas: Falha ao excluir. Pessoa não localizada.");
    }

    return true;
}

std::vector<Pessoa> ServicoPessoas::listar() {
    return ContainerDados::getInstancia()->listarPessoas();
}

// Método específico para a lógica de Autenticação/Login
bool ServicoPessoas::autenticar(const Email& email, const Senha& senha) {
    Pessoa pessoa;
    
    // Busca a pessoa pelo e-mail
    if (!ContainerDados::getInstancia()->buscarPessoa(email, pessoa)) {
        throw std::runtime_error("E-mail não cadastrado no sistema.");
    }

    // Valida se a senha informada confere com a senha armazenada
    if (pessoa.getSenha().getSenha() != senha.getSenha()) {
        throw std::runtime_error("Senha incorreta.");
    }

    return true;
}



