#ifndef SERVICOS_HPP_INCLUDED
#define SERVICOS_HPP_INCLUDED

#include "interfaces.hpp"
#include "containerDados.hpp"

///
/// @brief Implementação do serviços conforme regras de negócio e persistência para Projetos.
/// @details Realiza a interface IServicoProjetos consumindo o contêiner de dados em memória.
///
class ServicoProjetos : public IServicoProjetos {
public:
    virtual ~ServicoProjetos() = default;

    bool criar(const Projeto& projeto, const Email& emailGestor) override;
    bool ler(const Identificador& id, Projeto& projeto) override;
    bool atualizar(const Projeto& projeto) override;
    bool excluir(const Identificador& id) override;
    std::vector<Projeto> listarPorGestor(const Email& emailGestor) override;
    std::vector<Projeto> listarTodos() override;
};

#endif // SERVICOS_HPP_INCLUDED
