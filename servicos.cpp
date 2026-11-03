#include "servicos.hpp"

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

