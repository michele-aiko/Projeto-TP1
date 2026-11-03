```
#ifndef TESTEUNIT_HPP_INCLUDED
#define TESTEUNIT_HPP_INCLUDED

#include "servicos.hpp"

///
/// @brief Classe de Teste Unitário para validação dos Serviços de Projetos.
///
class TUTesteServicos {
private:
    ServicoProjetos servicoProj;

    void testarCriacaoProjeto();

public:
    void executar();
};

#endif // TESTEUNIT_HPP_INCLUDED
