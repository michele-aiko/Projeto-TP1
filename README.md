# Sistema de Gestão de Projetos Kanban (Projeto-TP1 - UnB)

Este repositório contém a implementação e documentação do **Sistema de Gestão de Projetos Kanban**, desenvolvido como Trabalho Prático para a disciplina **Técnicas de Programação 1 (TP1)** da **Universidade de Brasília (UnB)**.

---

## Visão Geral do Programa

O programa é uma aplicação em **C++** desenvolvida para ambiente de console (compatível com o **Code::Blocks**), com o objetivo de fornecer suporte à gestão de projetos ágeis utilizando o método **Kanban**.

### Funcionamento e Comportamento do Sistema
1. **Modelagem do Quadro Kanban**:
   - Cada quadro Kanban é organizado em 3 colunas de estado: **A FAZER**, **FAZENDO** e **FEITO**.
   - Permite definir um **Limite de WIP (Work in Progress)** para a coluna **FAZENDO** (entre 1 e 25 tarefas simultâneas).
2. **Perfis de Acesso e Autenticação**:
   - **Gestor**: Usuário criador do projeto. Possui permissões de administração (Criar, Ler, Atualizar, Excluir e Gerenciar Pessoas, Projetos, Quadros e Cartões).
   - **Desenvolvedor**: Membro do projeto. Possui permissões de leitura/listagem, movimentação de cartões entre colunas e consulta a métricas de desempenho.
   - **Controle de Acesso**: Todo usuário precisa cadastrar uma conta (`Email` e `Senha`) e realizar autenticação antes de acessar qualquer funcionalidade.

## Requisitos Funcionais e Mapeamento no Código

O sistema atende a **26 Requisitos Funcionais (Serviços)** definidos na especificação do trabalho:

|     ID  |               Serviço                |  Entidades Envolvidas |                 Permissões de Acesso                     |                Onde é Processado no Código                 |
|:-------:|:------------------------------------:|:---------------------:|:--------------------------------------------------------:|:----------------------------------------------------------:|
|**1-5**  |Criar, Ler, Atualizar, Excluir, Listar|**Pessoa**             |Gestor e Desenvolvedor                                    |`dominios.hpp`,`entidades.hpp`,Camadas de Apresent. e Serv. |
|**6-10** |Criar, Ler, Atualizar, Excluir, Listar|**Projeto**            |Criar/Atualizar/Excluir: Gestor; Ler/Listar: Gestor e Dev | Camadas de Apresentação e Serviço de Projetos              |
|**11-15**|Criar, Ler, Atualizar, Excluir, Listar|**Quadro**             |Criar/Atualizar/Excluir: Gestor; Ler/Listar: Gestor e Dev | Camadas de Apresentação e Serviço de Quadros               |
|**16-20**|Criar, Ler, Atualizar, Excluir, Listar|**Cartão de Atividade**|Criar/Atualizar/Excluir: Gestor; Ler/Listar: Gestor e Dev | Camadas de Apresentação e Serviço de Cartões               |
|**21**   |Mover Cartão de Atividade             |**Cartão de Atividade**|Gestor e Desenvolvedor                                    | Serviço — Transição `A FAZER → FAZENDO → FEITO`            |
|**22-23**|Tempo de Ciclo / Ciclo Médio          |**Cartão de Atividade**|Gestor e Desenvolvedor                                    | Serviço — Medição desde a coluna `FAZENDO` até `FEITO`     |
|**24-25**|Lead Time / Lead Time Médio           |**Cartão de Atividade**|Gestor e Desenvolvedor                                    | Serviço — Medição desde a coluna `A FAZER` até `FEITO`     |
|**26**   |Consultar WIP (Work in Progress)      |**Cartão de Atividade**|Gestor e Desenvolvedor                                    | Serviço — Contagem e validação da coluna `FAZENDO`         |

### Onde os Requisitos Ficam no Código:
- **Validadores de Tipos Básicos (`dominios.hpp` / `dominios.cpp`)**: Encapsulam e validam tipos nativos (`Email`, `Senha`, `Identificador`, `Limite`, `Nome`, `Texto`, `Timestamp`, `Estado`, `Prioridade`, `Tamanho`, `Papel`). Métodos `setValor()` lançam `std::invalid_argument` em caso de erro.
- **Estruturas de Dados (`entidades.hpp` / `entidades.cpp`)**: Representam as entidades do sistema (`Pessoa`, `Projeto`, `Quadro`, `CartaoDeAtividade`), compostas exclusivamente por atributos que são objetos de domínio.
- **Interface com Usuário (`controladoras_apresentacao.hpp` / `.cpp`)**: Apresenta menus, captura entradas via console e faz a pré-validação de formulários.
- **Regras de Negócio e Persistência (`controladoras_servico.hpp` / `.cpp`, `containers.hpp`)**: Valida o limite de WIP, calcula tempos (Lead Time, Cycle Time), aplica controle de permissões por papel e salva dados em memória ou banco SQLite.

---

## Arquitetura do Sistema: Camadas, Serviços e Interfaces

O sistema adota o padrão de **Arquitetura em Camadas (Layered Architecture)** para garantir modularidade, testabilidade e facilidade de manutenção.

```
       +-------------------------------------------------------+
       |               CAMADA DE APRESENTAÇÃO                  |
       |  (Interface com Usuário, Menus e Validação de Entrada)|
       +---------------------------+---------------------------+
                                   |
                                   v
                (Depende apenas de Interfaces Abstratas)
       +---------------------------+---------------------------+
       |                 INTERFACES (IServico)                 |
       |         (Classes Abstratas com Métodos Virtuais)      |
       +---------------------------+---------------------------+
                                   |
                                   v
                         (Realiza as Interfaces)
       +---------------------------+---------------------------+
       |                 CAMADA DE SERVIÇO                     |
       |   (Lógica de Negócio, Validação de WIP e Métricas)   |
       +---------------------------+---------------------------+
                                   |
                                   v
       +---------------------------+---------------------------+
       |             PERSISTÊNCIA E CONTÊINERES                |
       |      (Estruturas em Memória ou Banco SQLite)          |
       +-------------------------------------------------------+
```

### 1. Camada de Apresentação (Presentation Layer)
- **Responsabilidade**: Exibir interfaces ao usuário (via `cin`/`cout`), capturar dados do teclado, exibir mensagens de erro/sucesso e realizar a **pré-validação** das entradas.
- **Regra de Arquitetura**: Não contém lógica de negócio. Ela depende de **interfaces abstratas** fornecidas pela camada de serviço.

### 2. Camada de Serviço (Service/Business Layer)
- **Responsabilidade**: Implementar a **lógica de negócio** e a **persistência de dados**.
- **Funções Principais**:
  - Validar permissões (Ex: se o usuário autenticado é Gestor para ações de edição/exclusão).
  - Validar a regra de WIP (Ex: impedir adição em `FAZENDO` se atingir o `Limite`).
  - Atualizar timestamps de `entrada`, `inicio` e `termino` para calcular **Lead Time** e **Cycle Time**.
  - Gerenciar e salvar os objetos em contêineres na memória (listas/mapas) ou banco de dados SQLite.

### 3. Interfaces entre Módulos
- **Declaração**: Definidas no arquivo `interfaces.hpp` usando **classes abstratas em C++** compostas por **métodos virtuais puros** (`virtual ... = 0;`).
- **Propósito**: Garantir a inversão de dependência (a camada de apresentação sabe o serviço a ser realizado, mas desconhece os detalhes de implementação).

### 4. Módulos de Software
A arquitetura é dividida em **4 módulos principais**:
1. **Módulo de Autenticação / Pessoa**: Gerenciamento de contas e login de usuários.
2. **Módulo de Projetos**: Criação e controle de projetos.
3. **Módulo de Quadros**: Organização de quadros Kanban e limites de WIP.
4. **Módulo de Cartões de Atividade**: Manipulação de tarefas, movimentação de colunas e métricas.

---

### Mapeamento de Entidades e seus Domínios

Cada classe de entidade é composta obrigatoriamente por atributos que são instâncias de classes de domínio. A tabela abaixo especifica os domínios necessários para a criação de cada uma das 4 entidades do sistema:

| Entidade | Atributo | Domínio Requerido | Descrição e Papel no Sistema |
|:---|:---|:---|:---|
| **Pessoa** | `email` | **Email** | Chave Primária. Endereço de e-mail de identificação do usuário. |
| | `nome` | **Nome** | Nome completo do usuário (até 15 caracteres). |
| | `senha` | **Senha** | Senha de acesso alfanumérica (5 caracteres). |
| | `papel` | **Papel** | Função no sistema (`GESTOR` ou `DESENVOLVEDOR`). |
| **Projeto** | `identificador` | **Identificador** | Chave Primária. Código de 6 caracteres do projeto (3 letras e 3 dígitos). |
| | `nome` | **Nome** | Nome do projeto (até 15 caracteres). |
| | `descricao` | **Texto** | Descrição do projeto (até 30 caracteres). |
| | `inicio` | **Timestamp** | Data e hora de início do projeto (`DIA-MÊS-ANO-HORÁRIO`). |
| | `termino` | **Timestamp** | Data e hora de término do projeto (`DIA-MÊS-ANO-HORÁRIO`). |
| **Quadro** | `identificador` | **Identificador** | Chave Primária. Código identificador do quadro Kanban. |
| | `nome` | **Nome** | Nome do quadro (até 15 caracteres). |
| | `limite` | **Limite** | Limite máximo de WIP para a coluna `FAZENDO` (inteiro de 1 a 25). |
| **Cartão de Atividade** | `identificador` | **Identificador** | Chave Primária. Código identificador do cartão. |
| | `nome` | **Nome** | Título/Nome da atividade (até 15 caracteres). |
| | `descricao` | **Texto** | Descrição detalhada da tarefa (até 30 caracteres). |
| | `prioridade` | **Prioridade** | Nível de urgência da tarefa (`ALTA`, `MEDIA`, `BAIXA`). |
| | `estado` | **Estado** | Coluna do quadro (`A FAZER`, `FAZENDO`, `FEITO`). |
| | `tamanho` | **Tamanho** | Estimativa de porte da tarefa (`GRANDE`, `MÉDIO`, `PEQUENO`). |
| | `entrada` | **Timestamp** | Data/hora em que a tarefa foi criada (`A FAZER`). |
| | `inicio` | **Timestamp** | Data/hora em que a tarefa entrou na coluna `FAZENDO`. |
| | `termino` | **Timestamp** | Data/hora em que a tarefa foi movida para `FEITO`. |

---

## Estrutura de Testes

### 1. Testes Unitários (Unit Testing)
- **Testes de Domínio (`TUDominio`)**:
  - Cada uma das 11 classes de domínio possui uma classe de teste dedicada.
  - Testam obrigatoriamente 2 cenários: **Cenário com Valor Válido** (deve armazenar com sucesso) e **Cenário com Valor Inválido** (deve disparar e capturar `std::invalid_argument`).
- **Testes de Entidade (`TUEntidade`)**:
  - Testam a atribuição e recuperação correta dos objetos de domínio em cada uma das 4 entidades através dos métodos públicos `set` e `get`.

### 2. Teste Fumaça (Smoke Test) e Demonstração em Vídeo
- **Execução Integrada**: O sistema deve ser compilado e executado de forma totalmente integrada.
- **Cobertura Completa**: Deve conter ao menos **1 caso de teste de sucesso para CADA UMA das 26 funcionalidades/serviços** descritas na especificação.
- **Vídeo Demonstrativo**: Gravação de tela curta mostrando a compilação no Code::Blocks e a execução bem-sucedida do teste fumaça cobrindo todos os casos de uso.

### 3. Tabela de Critérios de Avaliação do Trabalho

| Item | Critério Avaliado | Ações Necessárias para Obter 100% da Nota |
|:--:|:---|:---|
| **1** | **Classes de Domínio** | • Criar classe para cada um dos 11 domínios.<br>• Implementar `setValor` (com `throw std::invalid_argument`) e `getValor`.<br>• Documentar classe e membros públicos com Doxygen (`///`).<br>• Gerar pasta de documentação em HTML via Doxygen.<br>• Testes unitários demonstrando 100% de funcionamento. |
| **2** | **Classes de Entidade** | • Criar classes `Pessoa`, `Projeto`, `Quadro` e `CartaoDeAtividade`.<br>• Atributos obrigatoriamente do tipo de classes de domínio.<br>• Métodos públicos `set` e `get` para todos os atributos.<br>• Documentação Doxygen completa em cada classe e membro.<br>• Fornecer páginas HTML do Doxygen e testes unitários. |
| **3** | **Modelo de Arquitetura** | • Diagrama UML oficial da arquitetura (módulos, interfaces e relacionamentos em PDF).<br>• Descrição textual detalhada das responsabilidades de cada módulo.<br>• Declaração de interfaces em C++ usando classes puramente abstratas (`virtual = 0`).<br>• Documentação Doxygen de todas as interfaces e geração de HTML. |
| **4** | **Camada de Apresentação** | • Implementar classes controladoras de tela (UI) com pré-validação de entrada.<br>• Garantir dependência exclusiva das interfaces abstratas de serviço.<br>• Comentários Doxygen em todas as classes e membros.<br>• Documentação HTML e demonstração do correto funcionamento. |
| **5** | **Camada de Serviço** | • Implementar classes de serviço/negócio que realizam as interfaces abstratas.<br>• Lógica de negócio completa (WIP, permissões gestor/dev, métricas).<br>• Armazenamento funcional (memória ou SQLite).<br>• Documentação Doxygen, HTML e testes integrados com sucesso. |
| **6** | **Vídeo do Teste Fumaça** | • Fornecer vídeo executando o código integrado compilado no Code::Blocks.<br>• Executar com sucesso o teste fumaça cobrindo as **26 funcionalidades** sem falhas. |
---

## Como Compilar e Executar no Code::Blocks

1. Abra o arquivo de projeto `.cbp` no **Code::Blocks**.
2. Certifique-se de que todos os arquivos `.hpp` e `.cpp` estejam adicionados ao projeto em `Build Targets -> Debug / Release`.
3. Para compilar e executar: pressione `F9` (Build and Run).
4. Para gerar a documentação Doxygen:
   ```bash
   doxygen Doxyfile
   ```
   Acesse a pasta `html/index.html` em seu navegador.
