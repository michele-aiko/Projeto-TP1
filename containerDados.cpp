#include "containerDados.hpp"

// Inicialização da instância estática do Singleton
ContainerDados* ContainerDados::instancia = nullptr;

ContainerDados* ContainerDados::getInstancia() {
    if (instancia == nullptr) {
        instancia = new ContainerDados();
    }
    return instancia;
}

// =============================================================================
// OPERAÇÕES DE PESSOA
// =============================================================================

bool ContainerDados::inserirPessoa(const Pessoa& pessoa) {
    // Usa o getter exclusivo do domínio Email: getEmail()
    std::string chave = pessoa.getEmail().getEmail();

    // Impede cadastro duplicado de e-mail (chave primária)
    if (tabelaPessoas.find(chave) != tabelaPessoas.end()) {
        return false;
    }

    tabelaPessoas[chave] = pessoa;
    return true;
}

bool ContainerDados::buscarPessoa(const Email& email, Pessoa& pessoa) const {
    std::string chave = email.getEmail();
    auto it = tabelaPessoas.find(chave);

    if (it != tabelaPessoas.end()) {
        pessoa = it->second;
        return true;
    }
    return false;
}

bool ContainerDados::atualizarPessoa(const Pessoa& pessoa) {
    std::string chave = pessoa.getEmail().getEmail();
    auto it = tabelaPessoas.find(chave);

    if (it != tabelaPessoas.end()) {
        it->second = pessoa;
        return true;
    }
    return false;
}

bool ContainerDados::removerPessoa(const Email& email) {
    std::string chave = email.getEmail();
    return (tabelaPessoas.erase(chave) > 0);
}

std::vector<Pessoa> ContainerDados::listarPessoas() const {
    std::vector<Pessoa> lista;
    for (const auto& par : tabelaPessoas) {
        lista.push_back(par.second);
    }
    return lista;
}

// =============================================================================
// OPERAÇÕES DE PROJETO
// =============================================================================

bool ContainerDados::inserirProjeto(const Projeto& projeto, const Email& emailGestor) {
    // Usa o getter exclusivo do domínio Identificador: getCodigo()
    std::string chaveId = projeto.getIdentificador().getIdentificador();

    // Impede ID duplicado
    if (tabelaProjetos.find(chaveId) != tabelaProjetos.end()) {
        return false;
    }

    tabelaProjetos[chaveId] = projeto;
    vinculoProjetoGestor[chaveId] = emailGestor.getEmail();
    return true;
}

bool ContainerDados::buscarProjeto(const Identificador& id, Projeto& projeto) const {
    std::string chave = id.getIdentificador();
    auto it = tabelaProjetos.find(chave);

    if (it != tabelaProjetos.end()) {
        projeto = it->second;
        return true;
    }
    return false;
}

bool ContainerDados::atualizarProjeto(const Projeto& projeto) {
    std::string chave = projeto.getIdentificador().getIdentificador();
    auto it = tabelaProjetos.find(chave);

    if (it != tabelaProjetos.end()) {
        it->second = projeto;
        return true;
    }
    return false;
}

bool ContainerDados::removerProjeto(const Identificador& id) {
    std::string chave = id.getIdentificador();
    vinculoProjetoGestor.erase(chave);
    return (tabelaProjetos.erase(chave) > 0);
}

std::vector<Projeto> ContainerDados::listarProjetosPorGestor(const Email& emailGestor) const {
    std::vector<Projeto> lista;
    std::string emailAlvo = emailGestor.getEmail();

    for (const auto& par : vinculoProjetoGestor) {
        if (par.second == emailAlvo) {
            auto it = tabelaProjetos.find(par.first);
            if (it != tabelaProjetos.end()) {
                lista.push_back(it->second);
            }
        }
    }
    return lista;
}

std::vector<Projeto> ContainerDados::listarTodosProjetos() const {
    std::vector<Projeto> lista;
    for (const auto& par : tabelaProjetos) {
        lista.push_back(par.second);
    }
    return lista;
}

// =============================================================================
// OPERAÇÕES DE QUADRO
// =============================================================================

bool ContainerDados::inserirQuadro(const Quadro& quadro, const Identificador& idProjeto) {
    std::string chaveId = quadro.getIdentificador().getIdentificador();

    if (tabelaQuadros.find(chaveId) != tabelaQuadros.end()) {
        return false;
    }

    tabelaQuadros[chaveId] = quadro;
    vinculoQuadroProjeto[chaveId] = idProjeto.getIdentificador();
    return true;
}

bool ContainerDados::buscarQuadro(const Identificador& id, Quadro& quadro) const {
    std::string chave = id.getIdentificador();
    auto it = tabelaQuadros.find(chave);

    if (it != tabelaQuadros.end()) {
        quadro = it->second;
        return true;
    }
    return false;
}

bool ContainerDados::atualizarQuadro(const Quadro& quadro) {
    std::string chave = quadro.getIdentificador().getIdentificador();
    auto it = tabelaQuadros.find(chave);

    if (it != tabelaQuadros.end()) {
        it->second = quadro;
        return true;
    }
    return false;
}

bool ContainerDados::removerQuadro(const Identificador& id) {
    std::string chave = id.getIdentificador();
    vinculoQuadroProjeto.erase(chave);
    return (tabelaQuadros.erase(chave) > 0);
}

std::vector<Quadro> ContainerDados::listarQuadrosPorProjeto(const Identificador& idProjeto) const {
    std::vector<Quadro> lista;
    std::string idProjetoAlvo = idProjeto.getIdentificador();

    for (const auto& par : vinculoQuadroProjeto) {
        if (par.second == idProjetoAlvo) {
            auto it = tabelaQuadros.find(par.first);
            if (it != tabelaQuadros.end()) {
                lista.push_back(it->second);
            }
        }
    }
    return lista;
}

// =============================================================================
// OPERAÇÕES DE CARTÃO DE ATIVIDADE
// =============================================================================

bool ContainerDados::inserirCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro) {
    std::string chaveId = cartao.getIdentificador().getIdentificador();

    if (tabelaCartoes.find(chaveId) != tabelaCartoes.end()) {
        return false;
    }

    tabelaCartoes[chaveId] = cartao;
    vinculoCartaoQuadro[chaveId] = idQuadro.getIdentificador();
    return true;
}

bool ContainerDados::buscarCartao(const Identificador& id, CartaoDeAtividade& cartao) const {
    std::string chave = id.getIdentificador();
    auto it = tabelaCartoes.find(chave);

    if (it != tabelaCartoes.end()) {
        cartao = it->second;
        return true;
    }
    return false;
}

bool ContainerDados::atualizarCartao(const CartaoDeAtividade& cartao) {
    std::string chave = cartao.getIdentificador().getIdentificador();
    auto it = tabelaCartoes.find(chave);

    if (it != tabelaCartoes.end()) {
        it->second = cartao;
        return true;
    }
    return false;
}

bool ContainerDados::removerCartao(const Identificador& id) {
    std::string chave = id.getIdentificador();
    vinculoCartaoQuadro.erase(chave);
    return (tabelaCartoes.erase(chave) > 0);
}

std::vector<CartaoDeAtividade> ContainerDados::listarCartoesPorQuadro(const Identificador& idQuadro) const {
    std::vector<CartaoDeAtividade> lista;
    std::string idQuadroAlvo = idQuadro.getIdentificador();

    for (const auto& par : vinculoCartaoQuadro) {
        if (par.second == idQuadroAlvo) {
            auto it = tabelaCartoes.find(par.first);
            if (it != tabelaCartoes.end()) {
                lista.push_back(it->second);
            }
        }
    }
    return lista;
}

bool ContainerDados::obterQuadroDoCartao(const Identificador& idCartao, Identificador& idQuadro) const {
    std::string chave = idCartao.getIdentificador();
    auto it = vinculoCartaoQuadro.find(chave);

    if (it != vinculoCartaoQuadro.end()) {
    
        idQuadro.setIdentificador(it->second);
        return true;
    }
    return false;
}

