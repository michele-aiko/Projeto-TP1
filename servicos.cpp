#include "servicos.hpp"

// =============================================================================
// SERVIÇO DE PESSOAS
// =============================================================================

bool ServicoPessoas::criar(const Pessoa& pessoa) {
    if (!ContainerPessoas::getInstancia()->inserirPessoa(pessoa)) {
        throw std::runtime_error("ServicoPessoas: Ja existe uma pessoa cadastrada com o e-mail '" +
                                 pessoa.getEmail().getEmail() + "'.");
    }
    return true;
}

bool ServicoPessoas::ler(const Email& email, Pessoa& pessoa) {
    return ContainerPessoas::getInstancia()->buscarPessoa(email, pessoa);
}

bool ServicoPessoas::atualizar(const Pessoa& pessoa) {
    Pessoa existente;
    if (!ContainerPessoas::getInstancia()->buscarPessoa(pessoa.getEmail(), existente)) {
        throw std::runtime_error("ServicoPessoas: Falha ao atualizar. Pessoa com e-mail '" +
                                 pessoa.getEmail().getEmail() + "' nao existe.");
    }

    // Impede alterar papel de gestor para desenvolvedor se houver projetos vinculados
    if (existente.getPapel().getPapel() == Papel::GESTOR &&
        pessoa.getPapel().getPapel() == Papel::DESENVOLVEDOR) {
        std::vector<Projeto> projetosVinculados = ContainerProjetos::getInstancia()->listarProjetosPorGestor(pessoa.getEmail());
        if (!projetosVinculados.empty()) {
            throw std::runtime_error("ServicoPessoas: Nao e possivel alterar papel para DESENVOLVEDOR pois o usuario possui projetos vinculados sob sua gestao.");
        }
    }

    if (!ContainerPessoas::getInstancia()->atualizarPessoa(pessoa)) {
        throw std::runtime_error("ServicoPessoas: Falha ao persistir atualizacao da pessoa '" +
                                 pessoa.getEmail().getEmail() + "'.");
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
    if (!ContainerDados::getInstancia()->buscarPessoa(emailGestor, gestor)) {
        throw std::runtime_error("ServicoProjetos: Gestor responsavel nao encontrado no sistema.");
    }

    // Validação de Permissão de Acesso
    if (gestor.getPapel().getPapel() != Papel::GESTOR) {
        throw std::logic_error("ServicoProjetos: Permissao negada. Apenas usuarios com papel GESTOR podem criar projetos.");
    }

    // Persistência com validação de unicidade da chave primária
    if (!ContainerDados::getInstancia()->inserirProjeto(projeto, emailGestor)) {
        throw std::runtime_error("ServicoProjetos: Ja existe um projeto cadastrado com o identificador '" +
                                 projeto.getIdentificador().getIdentificador() + "'.");
    }

    return true;
}

bool ServicoProjetos::ler(const Identificador& id, Projeto& projeto) {
    if (!ContainerDados::getInstancia()->buscarProjeto(id, projeto)) {
        throw std::runtime_error("ServicoProjetos: Projeto com identificador '" + id.getIdentificador() + "' nao encontrado.");
    }
    return true;
}

bool ServicoProjetos::atualizar(const Projeto& projeto) {
    if (!ContainerDados::getInstancia()->atualizarProjeto(projeto)) {
        throw std::runtime_error("ServicoProjetos: Falha ao atualizar. Projeto '" +
                                 projeto.getIdentificador().getIdentificador() + "' nao existe.");
    }
    return true;
}

bool ServicoProjetos::excluir(const Identificador& id) {
    Projeto p;
    if (!ContainerDados::getInstancia()->buscarProjeto(id, p)) {
        throw std::runtime_error("ServicoProjetos: Falha ao excluir. Projeto '" + id.getIdentificador() + "' nao localizado.");
    }

    // Exclusão em cascata dos quadros vinculados
    std::vector<Quadro> quadros = ContainerDados::getInstancia()->listarQuadrosPorProjeto(id);
    for (const auto& q : quadros) {
        ContainerDados::getInstancia()->removerQuadro(q.getIdentificador());
    }

    ContainerDados::getInstancia()->removerProjeto(id);
    return true;
}

std::vector<Projeto> ServicoProjetos::listarPorGestor(const Email& emailGestor) {
    return ContainerDados::getInstancia()->listarProjetosPorGestor(emailGestor);
}

std::vector<Projeto> ServicoProjetos::listarTodos() {
    return ContainerDados::getInstancia()->listarTodosProjetos();
}
