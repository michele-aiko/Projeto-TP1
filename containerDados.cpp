#include "containerDados.hpp"

// =============================================================================
// OPERAÇÕES DE PESSOA (ContainerPessoas)
// =============================================================================

ContainerPessoas* ContainerPessoas::instancia = nullptr;

ContainerPessoas* ContainerPessoas::getInstancia() {
    if (instancia == nullptr) {
        instancia = new ContainerPessoas();
    }
    return instancia;
}

bool ContainerPessoas::inserirPessoa(const Pessoa& pessoa) {
    std::string chave = pessoa.getEmail().getEmail();
    if (tabelaPessoas.find(chave) != tabelaPessoas.end()) {
        return false;
    }
    tabelaPessoas[chave] = pessoa;
    return true;
}

bool ContainerPessoas::buscarPessoa(const Email& email, Pessoa& pessoa) const {
    std::string chave = email.getEmail();
    auto it = tabelaPessoas.find(chave);
    if (it != tabelaPessoas.end()) {
        pessoa = it->second;
        return true;
    }
    return false;
}

bool ContainerPessoas::atualizarPessoa(const Pessoa& pessoa) {
    std::string chave = pessoa.getEmail().getEmail();
    auto it = tabelaPessoas.find(chave);
    if (it != tabelaPessoas.end()) {
        it->second = pessoa;
        return true;
    }
    return false;
}

bool ContainerPessoas::removerPessoa(const Email& email) {
    std::string chave = email.getEmail();
    return (tabelaPessoas.erase(chave) > 0);
}

std::vector<Pessoa> ContainerPessoas::listarPessoas() const {
    std::vector<Pessoa> lista;
    for (const auto& par : tabelaPessoas) {
        lista.push_back(par.second);
    }
    return lista;
}

bool ContainerPessoas::existePessoa(const Email& email) const {
    std::string chave = email.getEmail();
    return (tabelaPessoas.find(chave) != tabelaPessoas.end());
}

// =============================================================================
// OPERAÇÕES DE PROJETO (ContainerProjetos)
// =============================================================================

ContainerProjetos* ContainerProjetos::instancia = nullptr;

ContainerProjetos* ContainerProjetos::getInstancia() {
    if (instancia == nullptr) {
        instancia = new ContainerProjetos();
    }
    return instancia;
}

bool ContainerProjetos::inserirProjeto(const Projeto& projeto, const Email& emailGestor) {
    std::string chaveId = projeto.getIdentificador().getIdentificador();
    if (tabelaProjetos.find(chaveId) != tabelaProjetos.end()) {
        return false;
    }
    tabelaProjetos[chaveId] = projeto;
    vinculoProjetoGestor[chaveId] = emailGestor.getEmail();
    return true;
}

bool ContainerProjetos::buscarProjeto(const Identificador& id, Projeto& projeto) const {
    std::string chave = id.getIdentificador();
    auto it = tabelaProjetos.find(chave);
    if (it != tabelaProjetos.end()) {
        projeto = it->second;
        return true;
    }
    return false;
}

bool ContainerProjetos::atualizarProjeto(const Projeto& projeto) {
    std::string chave = projeto.getIdentificador().getIdentificador();
    auto it = tabelaProjetos.find(chave);
    if (it != tabelaProjetos.end()) {
        it->second = projeto;
        return true;
    }
    return false;
}

bool ContainerProjetos::removerProjeto(const Identificador& id) {
    std::string chave = id.getIdentificador();
    vinculoProjetoGestor.erase(chave);
    return (tabelaProjetos.erase(chave) > 0);
}

std::vector<Projeto> ContainerProjetos::listarProjetosPorGestor(const Email& emailGestor) const {
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

std::vector<Projeto> ContainerProjetos::listarTodosProjetos() const {
    std::vector<Projeto> lista;
    for (const auto& par : tabelaProjetos) {
        lista.push_back(par.second);
    }
    return lista;
}

bool ContainerProjetos::existeProjeto(const Identificador& id) const {
    std::string chave = id.getIdentificador();
    return (tabelaProjetos.find(chave) != tabelaProjetos.end());
}

bool ContainerProjetos::obterGestorDoProjeto(const Identificador& idProjeto, Email& emailGestor) const {
    std::string chave = idProjeto.getIdentificador();
    auto it = vinculoProjetoGestor.find(chave);
    if (it != vinculoProjetoGestor.end()) {
        emailGestor.setEmail(it->second);
        return true;
    }
    return false;
}

// =============================================================================
// OPERAÇÕES DE QUADRO (ContainerQuadros)
// =============================================================================

ContainerQuadros* ContainerQuadros::instancia = nullptr;

ContainerQuadros* ContainerQuadros::getInstancia() {
    if (instancia == nullptr) {
        instancia = new ContainerQuadros();
    }
    return instancia;
}

bool ContainerQuadros::inserirQuadro(const Quadro& quadro, const Identificador& idProjeto) {
    std::string chaveId = quadro.getIdentificador().getIdentificador();
    if (tabelaQuadros.find(chaveId) != tabelaQuadros.end()) {
        return false;
    }
    tabelaQuadros[chaveId] = quadro;
    vinculoQuadroProjeto[chaveId] = idProjeto.getIdentificador();
    return true;
}

bool ContainerQuadros::buscarQuadro(const Identificador& id, Quadro& quadro) const {
    std::string chave = id.getIdentificador();
    auto it = tabelaQuadros.find(chave);
    if (it != tabelaQuadros.end()) {
        quadro = it->second;
        return true;
    }
    return false;
}

bool ContainerQuadros::atualizarQuadro(const Quadro& quadro) {
    std::string chave = quadro.getIdentificador().getIdentificador();
    auto it = tabelaQuadros.find(chave);
    if (it != tabelaQuadros.end()) {
        it->second = quadro;
        return true;
    }
    return false;
}

bool ContainerQuadros::removerQuadro(const Identificador& id) {
    std::string chave = id.getIdentificador();
    vinculoQuadroProjeto.erase(chave);
    return (tabelaQuadros.erase(chave) > 0);
}

std::vector<Quadro> ContainerQuadros::listarQuadrosPorProjeto(const Identificador& idProjeto) const {
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

bool ContainerQuadros::existeQuadro(const Identificador& id) const {
    std::string chave = id.getIdentificador();
    return (tabelaQuadros.find(chave) != tabelaQuadros.end());
}

bool ContainerQuadros::obterProjetoDoQuadro(const Identificador& idQuadro, Identificador& idProjeto) const {
    std::string chave = idQuadro.getIdentificador();
    auto it = vinculoQuadroProjeto.find(chave);
    if (it != vinculoQuadroProjeto.end()) {
        idProjeto.setIdentificador(it->second);
        return true;
    }
    return false;
}

// =============================================================================
// OPERAÇÕES DE CARTÃO DE ATIVIDADE (ContainerCartoes)
// =============================================================================

ContainerCartoes* ContainerCartoes::instancia = nullptr;

ContainerCartoes* ContainerCartoes::getInstancia() {
    if (instancia == nullptr) {
        instancia = new ContainerCartoes();
    }
    return instancia;
}

bool ContainerCartoes::inserirCartao(const CartaoDeAtividade& cartao, const Identificador& idQuadro) {
    std::string chaveId = cartao.getIdentificador().getIdentificador();
    if (tabelaCartoes.find(chaveId) != tabelaCartoes.end()) {
        return false;
    }
    tabelaCartoes[chaveId] = cartao;
    vinculoCartaoQuadro[chaveId] = idQuadro.getIdentificador();
    return true;
}

bool ContainerCartoes::buscarCartao(const Identificador& id, CartaoDeAtividade& cartao) const {
    std::string chave = id.getIdentificador();
    auto it = tabelaCartoes.find(chave);
    if (it != tabelaCartoes.end()) {
        cartao = it->second;
        return true;
    }
    return false;
}

bool ContainerCartoes::atualizarCartao(const CartaoDeAtividade& cartao) {
    std::string chave = cartao.getIdentificador().getIdentificador();
    auto it = tabelaCartoes.find(chave);
    if (it != tabelaCartoes.end()) {
        it->second = cartao;
        return true;
    }
    return false;
}

bool ContainerCartoes::removerCartao(const Identificador& id) {
    std::string chave = id.getIdentificador();
    vinculoCartaoQuadro.erase(chave);
    return (tabelaCartoes.erase(chave) > 0);
}

std::vector<CartaoDeAtividade> ContainerCartoes::listarCartoesPorQuadro(const Identificador& idQuadro) const {
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

bool ContainerCartoes::obterQuadroDoCartao(const Identificador& idCartao, Identificador& idQuadro) const {
    std::string chave = idCartao.getIdentificador();
    auto it = vinculoCartaoQuadro.find(chave);
    if (it != vinculoCartaoQuadro.end()) {
        idQuadro.setIdentificador(it->second);
        return true;
    }
    return false;
}

bool ContainerCartoes::existeCartao(const Identificador& id) const {
    std::string chave = id.getIdentificador();
    return (tabelaCartoes.find(chave) != tabelaCartoes.end());
}

