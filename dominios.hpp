#ifndef DOMINIOS_HPP_INCLUDED
#define DOMINIOS_HPP_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <algorithm>


using namespace std; //Para seguirmos o Google C++ Style Guide SEMPRE USAR std::string em variáveis e métodos de texto neste arquivo

///
/// @brief Representa o domínio Estado, podendo ser utilizado para controle do andamento de uma atividade.
/// @details Garante que o estado seja restrito a uma das três opções pré-existentes: 
///          "A FAZER", "FAZENDO" ou "FEITO". 
///          Qualquer tentativa de atribuir outro valor resultará em lançamento de exceção.
///
class Estado {
	private:
		std::string estado; ///< Objeto primitivo std::string contendo o estado de uma atividade.

	///
	/// @brief Método privado de validação do estado.
	/// @details Verifica se a string informada pertence à lista de opções válidas.
	/// @param estado Objeto std::string contendo o texto a ser validado.
	/// @throw std::invalid_argument Lançada se a string informada não for "A FAZER", "FAZENDO" ou "FEITO".
	///
		void validar(const std::string& estado) const;
	
	public:
	
		inline static const std::string A_FAZER = "A FAZER";
		inline static const std::string FAZENDO = "FAZENDO";
		inline static const std::string FEITO = "FEITO";

	///
	/// @brief Construtor da classe Estado, deve ser utilizado na criação de um novo cartão de atividade.
	/// @details Inicializa o objeto com o estado "A FAZER" por padrão.
	/// @param estadoInicial Objeto std::string contendo o estado padrão: A_FAZER.
	/// @throw std::invalid_argument Lançada se o estadoInicial ou argumento informado for inválido.
	///
		explicit Estado(const std::string& estadoInicial = A_FAZER) { setEstado(estadoInicial); }
	
	///
	/// @brief Atribui e valida o valor do estado
	/// @details Invoca o método validar() e, se aprovado, armazena o novo estado.
	/// @param novoEstadoObjeto std::string contendo o novo estado.
	/// @throw std::invalid_argument Lançada se o novoEstado for diferente das opções permitidas.
	///
		void setEstado(const std::string& novoEstado);
	
	///
	/// @brief Obtém o valor do estado armazenado
	/// @return Objeto std::string contendo o estado atual.
	///
		std::string getEstado() const{ return estado; }
	
	///
	/// @brief Retorna lista contendo as opções válidas pré-existentes de estado.
	///		   Alterações na lista devem ser realizadas diretamente nesse método após a modificação dos nomes das variáveis de estado A_FAZER, FAZENDO e FEITO
	/// @return Objeto std::vector<std::string> contendo {"A FAZER", "FAZENDO", "FEITO"}.
	///
		static std::vector<std::string> getOpcoes() { return {A_FAZER, FAZENDO, FEITO}; }

};

	inline void Estado::validar(const std::string& novoEstado) const {
	    std::vector<std::string> opcoes = getOpcoes();
	    
	    // Busca se o valor informado está entre as opções permitidas
	    if (std::find(opcoes.begin(), opcoes.end(), novoEstado) == opcoes.end()) { //Se find não encontra o elemento procurado, retorna o ponteiro para o final da lista
	        throw std::invalid_argument("Estado invalido: O valor deve ser 'A FAZER', 'FAZENDO' ou 'FEITO'.");
	    }
	}

	inline void Estado::setEstado(const std::string& novoEstado) {
	    validar(novoEstado);
	    this->estado = novoEstado;
	}

///
/// @brief Representa o domínio Nome a ser utilizado nas 4 entidades (Pessoa, Projeto, Quadro e Cartão de Atividade).
/// @details Armazena e valida uma cadeia de texto de 1 a 15 caracteres composta por letras e espaços internos.
///
class Nome {
		private:
		    std::string nome; ///< Objeto primitivo std::string contendo o texto do nome armazenado.
		
		    ///
		    /// @brief Valida a string que representa o nome
		    /// @details Verifica se a string tem de 1 a 15 caracteres, sem espaços no início ou fim, e se todo espaço é seguido por letra.
		    /// @param nome Objeto std::string contendo o texto a ser avaliado.
		    /// @throw std::invalid_argument Lançada se o texto descumprir qualquer regra do domínio Nome.
		    ///
		    void validar(const std::string& nome) const;
		
		public:
		    ///
		    /// @brief Construtor da classe Nome.
		    /// @details Inicializa o objeto com "Nome Padrao" ou com o valor fornecido via parâmetro.
		    /// @param nomeInicial Objeto std::string contendo o valor inicial do nome.
		    /// @throw std::invalid_argument Lançada se o nomeInicial ou argumento fornecido for inválido.
		    ///
		    explicit Nome(const std::string& nomeInicial = "Nome Padrao") { setNome(nomeInicial); }
		
		    ///
		    /// @brief Atribui e valida o nome.
		    /// @param novoNOme Objeto std::string contendo o texto a ser armazenado.
		    /// @throw std::invalid_argument Lançada se novoValor não atender aos critérios de validação.
		    ///
		    void setNome(const std::string& novoNome);
		
		    ///
		    /// @brief Obtém o nome armazenado.
		    /// @return Objeto std::string contendo o nome validado.
		    ///
		    std::string getNome() const { return nome; }
		
};

	inline void Nome::setNome(const std::string& novoNome) {
	    validar(novoNome); // Se for inválido, dispara std::invalid_argument
	    this->nome = novoNome;
	}

///
/// @brief Representa o domínio Tamanho, sugestão de uso no cartão de atividades tipo Kanban indicando a estimativa de porte da tarefa.
/// @details Limita o tamanho estimado das tarefas as opções pré-definidas: "PEQUENO", "MEDIO" ou "GRANDE".
///		     Qualquer tentativa de atribuir um valor fora desta lista resultará no lançamento de exceção.
///
class Tamanho {
		private:
		    std::string tamanho; ///< Objeto primitivo std::string contendo o tamanho validado.
		
		    ///
		    /// @brief Valida se o tamanho informado pertence à lista de opções permitidas.
		    /// @details Percorre o vetor retornado por getOpcoes() e verifica se a string informada é válida.
		    /// @param tamanho Objeto std::string contendo o texto do tamanho a ser validado.
		    /// @throw std::invalid_argument Lançada se o texto for diferente de "PEQUENO", "MEDIO" ou "GRANDE".
		    ///
		    void validar(const std::string& tamanho) const;
		
		public:
		    /// @brief Constante estática pública representando o tamanho PEQUENO.
		    inline static const std::string PEQUENO = "PEQUENO";
		
		    /// @brief Constante estática pública representando o tamanho MEDIO.
		    inline static const std::string MEDIO = "MEDIO";
		
		    /// @brief Constante estática pública representando o tamanho GRANDE.
		    inline static const std::string GRANDE = "GRANDE";
		
		    ///
		    /// @brief Construtor da classe Tamanho.
		    /// @details Inicializa o objeto com "PEQUENO" por padrão ou valida o parâmetro informado.
		    /// @param tamanhoInicial Objeto std::string contendo o tamanho inicial.
		    /// @throw std::invalid_argument Lançada se tamanhoInicial for inválido.
		    ///
		    explicit Tamanho(const std::string& tamanhoInicial = PEQUENO) { setTamanho(tamanhoInicial); } // Garante que a validação seja executada na criação do objeto
		
		    ///
		    /// @brief Atribui e valida o tamanho da atividade.
		    /// @details Executa a validação e, em caso de sucesso, armazena o novo valor.
		    /// @param novoTamanho Objeto std::string contendo o novo tamanho.
		    /// @throw std::invalid_argument Lançada se novoTamanho não pertencer às opções permitidas.
		    ///
		    void setTamanho(const std::string& novoTamanho);
		
		    ///
		    /// @brief Obtém o tamanho armazenado.
		    /// @return Objeto std::string contendo o tamanho validado.
		    ///
		    std::string getTamanho() const { return tamanho; }
		
		    ///
		    /// @brief Retorna a lista contendo as opções válidas pré-existentes.
			/// @details Alterações na lista devem ser realizadas diretamente nesse método após a modificação dos nomes das variáveis de tamanho: PEQUENO, MEDIO, GRANDE
		    /// @return Objeto std::vector<std::string> contendo {"PEQUENO", "MEDIO", "GRANDE"}.
		    ///
		    static std::vector<std::string> getOpcoes()  { return {PEQUENO, MEDIO, GRANDE}; }
};

	inline void Tamanho::validarTamanho(const std::string& tamanho) const {
	    std::vector<std::string> opcoes = getOpcoes();
	
	    // Busca se o valor informado está presente no vetor de opções permitidas
	    if (std::find(opcoes.begin(), opcoes.end(), tamanho) == opcoes.end()) {
	        throw std::invalid_argument("Tamanho invalido: O valor deve ser 'PEQUENO', 'MEDIO' ou 'GRANDE'.");
    	}
	}

	inline void Tamanho::setTamanho(const std::string& novoTamanho) {
    	validar(novoTamanho); // Se for inválido, lança std::invalid_argument
    	this->tamanho = novoTamanho;
	}


///
/// @brief Representa um texto de até 30 caracteres
/// @details Armazena e valida texto que deve iniciar com letra maiúscula, terminar 
///			 obrigatoriamente com ponto (.) e não pode conter pontuações consecutivas.
///
class Texto {
		private:
		    std::string texto; ///< Objeto primitivo std::string contendo o texto validado.
		
		    ///
		    /// @brief Valida o texto segundo as regras de formatação
		    /// @details Integra todas as checagens de tamanho, maiúscula inicial, ponto final,
		    ///          caracteres permitidos e proibição de pontuações consecutivas.
		    /// @param texto Objeto std::string contendo o texto a ser avaliado.
		    /// @throw std::invalid_argument Lançada se o texto descumprir qualquer regra do domínio.
		    ///
		    void validar(const std::string& texto) const;
		
		public:
		    ///
		    /// @brief Construtor da classe Texto.
		    /// @details Inicializa o objeto com "Texto valido." por padrão ou valida o parâmetro informado.
		    /// @param textoInicial Objeto std::string contendo o texto inicial.
		    /// @throw std::invalid_argument Lançada se textoInicial for inválido.
		    ///
		    explicit Texto(const std::string& textoInicial = "Texto valido.") { setTexto(textoInicial); } // Passa pela validação ao criar o objeto
		
		    ///
		    /// @brief Atribui e valida o texto.
		    /// @param novoTexto Objeto std::string contendo o novo texto a ser armazenado.
		    /// @throw std::invalid_argument Lançada se novoTexto não atender aos critérios de validação.
		    ///
		    void setTexto(const std::string& novoTexto);
		
		    ///
		    /// @brief Obtém o texto armazenado.
		    /// @return Objeto std::string contendo o texto validado.
		    ///
		    std::string getTexto() const { return texto; }

};

	inline void Texto::setTexto(const std::string& novoTexto) {
	    validar(novoTexto); // Se for inválido, lança std::invalid_argument
	    this->texto = novoTexto;
	}


///
/// @brief Domínio Email representa o usuário do sistema: deve ser utilizado no login e relacionado com o papel do usuário e suas permissões.
/// @details Encapsula o endereço de e-mail no formato parte-local@domínio. A parte local tem até 64 caracteres e o domínio até 255.
///			 Sugerimos inclusão como chave primária da entidade Pessoa
///
class Email { //Formato válido: parte-local@dominio
        private:
            static const int LIMLOCAL = 64; 
            static const int LIMDOMINIO = 255; 
            string email; ///< Objeto primitivo string que armazena o e-mail
///
/// @brief Valida o e-mail e deve ser incluído no método setEmail
/// @details Analisa se o email informado pelo usuário segue a estrutura parte-local@domínio e respeita as regras permitidas: 
///          Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-), não pode iniciar ou terminar com ponto ou hífen,
///			 ponto ou hifen deve ser seguido de letra(s) ou digito(s)
/// @param Recebe string passada por referência
/// @throw std::invalid_argument Lançada se a string não atender às regras de validação.
///
            void validar(string); 

        public:
///
/// @brief Construtor da classe Email, deve ser utilizado na criação de um novo usuário.
/// @details Inicializa o objeto com o email "parte-local@dominio" por padrão.
/// @param Objeto std::string contendo o email inicial (padrão: parte-local@dominio).
/// @throw std::invalid_argument Lançada se o novoEmail informado for inválido.
///			
			explicit Email(const std::string& novoEmail = "parte-local@dominio") { setEmail(novoEmail); }
///
/// @brief Armazena o e-mail a ser cadastrado, só será criado após a validação dos requisitos e se nenhuma exceção for lançada
/// @details Recebe uma string, invoca o método privado validar() e, se válido, cria o novo email do usuário.
/// @param Recebe string contendo o e-mail informado pelo usuário
/// @throw std::invalid_argument Lançada caso o formato do e-mail seja inválido.
///
            void setEmail(std::string);
///
/// @brief Obtém o e-mail do usuário, deve ser utilizado como método pela entidade Pessoa
/// @details Retorna uma cópia do objeto string contido no atributo email, não gera qualquer alteração desse objeto.
/// @return Objeto std::string contendo o endereço de e-mail.
///
            std::string getEmail() const { return email; }

};

inline void Email::setEmail(std::string novoEmail){
	validar(novoEmail);
	this->email = novoEmail;
}

///
/// @brief Representa o domínio Identificador, pode ser usado como chave-primária de projetos e quadros de atividades, por exemplo.
/// @details Armazena e valida um código identificador único composto por 6 caracteres:
///          os 3 primeiros devem ser letras (A-Z ou a-z) e os 3 últimos devem ser dígitos (0-9).
///          Exemplo de valor válido: "ABC123".
///
class Identificador {
		private:
		    static const std::size_t TAMANHO = 6; ///< Tamanho máximo para o código identificador.
		    std::string identificador;            ///< Objeto primitivo std::string contendo o código validado.
		
		    ///
		    /// @brief Valida o formato do identificador quanto ao seu limite e formato.
		    /// @details Avalia se a string possui 6 caracteres, sendo 3 letras seguidas de 3 dígitos.
		    /// @param identificador Objeto std::string contendo o código a ser avaliado.
		    /// @throw std::invalid_argument Lançada se o código descumprir as regras de formato.
		    ///
		    void validar(const std::string& identificador) const;
		
		public:
		    ///
		    /// @brief Construtor da classe Identificador.
		    /// @details Inicializa o objeto com o código padrão "ABC123" ou valida o argumento informado.
		    /// @param identificadorInicial Objeto std::string contendo o código inicial.
		    /// @throw std::invalid_argument Lançada se identificadorInicial for inválido.
		    ///
		    explicit Identificador(const std::string& identificadorInicial = "ABC123") { setIdentificador(identificadorInicial); }
		
		    ///
		    /// @brief Atribui e valida o identificador.
		    /// @details Valida o identificador informado como argumento, se aprovado, armazena o novo valor.
		    /// @param novoIdentificador Objeto std::string contendo o novo código identificador.
		    /// @throw std::invalid_argument Lançada se novoIdentificador for inválido.
		    ///
		    void setIdentificador(const std::string& novoIdentificador);
		
		    ///
		    /// @brief Obtém o identificador armazenado.
		    /// @return Objeto std::string contendo o código validado.
		    ///
		    std::string getIdentificador() const { return identificador; }
};

inline void Identificador::setIdentificador(const std::string& novoIdentificador) {
		        validar(novoIdentificador); // Garante a execução das regras antes da atribuição
		        this->identificador = novoIdentificador;
		    }


///
/// @brief Representa o Limite de WIP (Work in Progress) de quadros de gestão de projetos .
/// @details Armazena e valida a quantidade máxima de cartões de atividades simultâneos
///          permitidos na coluna FAZENDO. O valor deve ser obrigatoriamente um número inteiro
///          na faixa de 1 a 25.
///
class Limite {
private:
    static const int LIMITE_MINIMO = 1;  ///< Menor valor permitido para o limite de WIP.
    static const int LIMITE_MAXIMO = 25; ///< Maior valor permitido para o limite de WIP.
    int limite;                           ///< Atributo primitivo inteiro que armazena o limite validado.

    ///
    /// @brief Valida se o número inteiro fornecido está dentro da faixa permitida (1 a 25).
    /// @details Avalia se o parâmetro atende aos limites mínimo e máximo estipulados no TP1.
    /// @param limite Tipo primitivo int representando o valor numérico a ser testado.
    /// @throw std::invalid_argument Lançada se o inteiro for menor que 1 ou maior que 25.
    ///
    void validar(int limite) const;

public:
    ///
    /// @brief Construtor da classe Limite
    /// @details Inicializa o objeto com o valor padrão 5 ou valida o inteiro informado.
    /// @param valorInicial Tipo primitivo int contendo o limite desejado (padrão: 5).
    /// @throw std::invalid_argument Lançada se valorInicial for menor que 1 ou maior que 25.
    ///
    explicit Limite(int valorInicial = 5) { setValor(valorInicial); }

    ///
    /// @brief Atribui e valida a quantidade limite de WIP
    /// @details Chame a função privada validar() e, se aprovada, armazena o novo limite no atributo privado.
    /// @param novoValor Tipo primitivo int contendo o novo valor limite.
    /// @throw std::invalid_argument Lançada se novoValor estiver fora da faixa de 1 a 25.
    ///
    void setLimite(int novoValor) {
        validar(novoValor); // Garante a validação antes da atribuição
        this->limite = novoValor;
    }

    ///
    /// @brief Obtém o valor limite configurado (Implementação Inline - Padrão TP1).
    /// @return Tipo primitivo int contendo a quantidade limite armazenada.
    ///
    int getLimite() const {
        return limite;
    }
};


/// 
/// @brief Representa o domínio Papel desempenhado por um participante no sistema Kanban. 
/// @details Restringe o papel do usuário estritamente a duas opções válidas: 
/// 			- "GESTOR": Criador/administrador com permissões completas de gerenciamento. 
/// 			- "DESENVOLVEDOR": Membro do projeto com permissões operacionais e de consulta. 
/// 
class Papel { 
	private: 
		std::string papel; ///< Objeto primitivo std::string contendo o papel validado. 
		
		/// 
		/// @brief Valida se o papel informado corresponde a "GESTOR" ou "DESENVOLVEDOR". 
		/// @details Avalia se o texto fornecido coincide com uma das constantes permitidas da classe. 
		/// @param papel Objeto std::string contendo o texto a ser avaliado. 
		/// @throw std::invalid_argument Lançada se o texto for diferente de "GESTOR" e "DESENVOLVEDOR". 
		/// 
		void validar(const std::string& papel) const; 
	
	public: 
		/// @brief Constante estática pública representando a função de GESTOR, aquele que será responsável por criar projetos. 
		inline static const std::string GESTOR = "GESTOR"; 

		/// @brief Constante estática pública representando a função de DESENVOLVEDOR, aquele que fará parte da equipe do projeto. 
		inline static const std::string DESENVOLVEDOR = "DESENVOLVEDOR"; 

		/// 
		/// @brief Construtor da classe Papel 
		/// @details Inicializa o objeto com o papel padrão "GESTOR" ou valida a string informada. 
		/// @param valorInicial Objeto std::string contendo o papel inicial desejado (padrão: "GESTOR"). 
		/// @throw std::invalid_argument Lançada se valorInicial não for "GESTOR" nem "DESENVOLVEDOR". 
		/// 
		explicit Papel(const std::string& valorInicial = GESTOR) { setValor(valorInicial); } 

		/// 
		/// @brief Atribui e valida o papel do participante do projeto
		/// @details Invoca o método privado validar() e, se válido, armazena o novo papel no atributo valor. 
		/// @param novoValor Objeto std::string contendo o novo papel. 
		/// @throw std::invalid_argument Lançada se o novoValor for inválido. 
		/// 
		void setPapel(const std::string& novoValor) { 
			validar(novoValor); // Valida antes de persistir na memória 
			this->papel = novoValor; 
		} 

		/// 
		/// @brief Obtém o papel armazenado para a pessoa parte de um projeto
		/// @return Objeto std::string contendo o papel validado ("GESTOR" ou "DESENVOLVEDOR"). 
		/// 
		std::string getPapel() const { return papel; } 
		/// 
		/// 
		/// @brief Retorna um vetor contendo a lista com todas as opções válidas. 
		/// @return Objeto std::vector<std::string> contendo as opções de papeis de pessoas envolvidas no projeto: {"GESTOR", "DESENVOLVEDOR"}.
		/// 
		static std::vector<std::string> getOpcoes() {
			return {GESTOR, DESENVOLVEDOR};
		}
};

///
/// @brief Representa o domínio Prioridade de uma atividade no quadro Kanban.
/// @details Restringe a prioridade de uma tarefa estritamente às opções pré-definidas:
///          - "ALTA"
///          - "MEDIA"
///          - "BAIXA"
///          Qualquer tentativa de atribuir valor diferente dessas opções resultará no
///          lançamento de exceção std::invalid_argument.
///
class Prioridade {
private:
    std::string prioridade; ///< Objeto primitivo std::string que armazena a prioridade validada.

    ///
    /// @brief Valida se a string fornecida corresponde a uma prioridade permitida.
    /// @details Compara o texto recebido com as constantes estáticas ALTA, MEDIA e BAIXA.
    /// @param valor Objeto std::string contendo a prioridade a ser avaliada.
    /// @throw std::invalid_argument Lançada se a prioridade for diferente de "ALTA", "MEDIA" e "BAIXA".
    ///
    void validar(const std::string& prioridade) const;

public:
    /// @brief Constante estática pública representando prioridade ALTA.
    inline static const std::string ALTA = "ALTA";

    /// @brief Constante estática pública representando prioridade MEDIA.
    inline static const std::string MEDIA = "MEDIA";

    /// @brief Constante estática pública representando prioridade BAIXA.
    inline static const std::string BAIXA = "BAIXA";

    ///
    /// @brief Construtor da classe Prioridade
    /// @details Inicializa o objeto com a prioridade padrão "BAIXA" ou valida o valor fornecido.
    /// @param valorInicial Objeto std::string contendo a prioridade inicial desejada (padrão: "BAIXA").
    /// @throw std::invalid_argument Lançada se valorInicial não for uma prioridade válida.
    ///
    explicit Prioridade(const std::string& valorInicial = BAIXA) {
        setPrioridade(valorInicial);
    }

    ///
    /// @brief Atribui e valida a prioridade da tarefa
    /// @details Invoca o método privado validar() e, se aceito, armazena o novo valor na memória.
    /// @param novoValor Objeto std::string contendo a nova prioridade.
    /// @throw std::invalid_argument Lançada se novoValor for inválido.
    ///
    void setPrioridade(const std::string& novoValor) {
        validar(novoValor); // Garante a validação antes da persistência
        this->prioridade = novoValor;
    }

    ///
    /// @brief Obtém a prioridade armazenada para uma atividade
    /// @return Objeto std::string contendo a prioridade validada ("ALTA", "MEDIA" ou "BAIXA").
    ///
    std::string getPrioridade() const {  return prioridade;  }

    ///
    /// @brief Retorna a lista com todas as opções de prioridade válidas.
    /// @return Objeto std::vector<std::string> contendo {"ALTA", "MEDIA", "BAIXA"}.
    ///
    static std::vector<std::string> getOpcoes() {
        return {ALTA, MEDIA, BAIXA};
    }
};

///
/// @brief Representa o domínio Senha de acesso do usuário no sistema Kanban.
/// @details Armazena e valida uma senha com comprimento exato de 5 caracteres alfanuméricos
///          (letras 'a'-'z', 'A'-'Z' e dígitos '0'-'9'), contendo obrigatoriamente ao menos uma
///          letra e ao menos um dígito.
///
class Senha {
private:
    static const std::size_t TAMANHO = 5; ///< Comprimento exato exigido para a senha.
    std::string senha;                    ///< Objeto std::string contendo a senha validada.

    ///
    /// @brief Valida se a senha atende a todas as restrições do TP1.
    /// @details Avalia o comprimento exato (5 caracteres), restrição a alfanuméricos e
    ///          a presença obrigatória de pelo menos uma letra e um dígito.
    /// @param senha Objeto std::string contendo a senha a ser auditada.
    /// @throw std::invalid_argument Lançada se a senha descumprir qualquer regra do domínio.
    ///
    void validar(const std::string& senha) const;

public:
    ///
    /// @brief Construtor da classe Senha
    /// @details Inicializa o objeto com uma senha padrão válida ("A1b2c") ou valida o parâmetro fornecido.
    /// @param valorInicial Objeto std::string contendo a senha inicial desejada (padrão: "A1b2c").
    /// @throw std::invalid_argument Lançada se valorInicial não atender às regras de validação.
    ///
    explicit Senha(const std::string& valorInicial = "A1b2c") {
        setSenha(valorInicial);
    }

    ///
    /// @brief Atribui e valida a senha de acesso
    /// @details Invoca o método privado validar() e, se aceito, armazena a nova senha.
    /// @param novoValor Objeto std::string contendo a nova senha.
    /// @throw std::invalid_argument Lançada se novoValor for inválido.
    ///
    void setSenha(const std::string& novoValor) {
        validar(novoValor); // Valida antes de persistir na memória
        this->senha = novoValor;
    }

    ///
    /// @brief Obtém a senha armazenada
    /// @return Objeto std::string contendo a senha validada.
    ///
    std::string getSenha() const {
        return senha;
    }
};


///
/// @brief Representa o domínio Timestamp no sistema Kanban.
/// @details Armazena e valida um carimbo de data e horário no formato DIA-MÊS-ANO-HORÁRIO.
///          Exemplo de valor válido: "15-JAN-2024-14:30".
///          Considera meses com 28, 29, 30 e 31 dias, além de regras completas para anos bissextos.
///
class Timestamp {
private:
    std::string timestamp; ///< Objeto primitivo std::string contendo a data e horário validados.

    ///
    /// @brief Verifica se um determinado ano é bissexto.
    /// @param ano Inteiro representando o ano (2000 a 2099).
    /// @return true se o ano for bissexto, false caso contrário.
    ///
    bool ehBissexto(int ano) const;

    ///
    /// @brief Determina a quantidade máxima de dias permitidos em um mês específico.
    /// @param mes Sigla do mês com 3 letras maiúsculas (ex: "JAN", "FEV").
    /// @param ano Inteiro representando o ano, necessário para o cálculo de fevereiro.
    /// @return Quantidade máxima de dias no mês (28, 29, 30 ou 31), ou 0 se a sigla for inválida.
    ///
    int obterDiasNoMes(const std::string& mes, int ano) const;

    ///
    /// @brief Valida se o formato do Timestamp atende a todas as restrições previstas
    /// @details Inspeciona a estrutura dos delimitadores hífen ('-') e dois-pontos (':'),
    ///          a faixa de valores para dia, mês, ano, hora e minuto, e a consistência do calendário.
    /// @param timestamp Objeto std::string contendo o carimbo temporal a ser auditado.
    /// @throw std::invalid_argument Lançada se a string descumprir qualquer regra do domínio Timestamp.
    ///
    void validar(const std::string& timestamp) const;

public:
    ///
    /// @brief Construtor da classe Timestamp
    /// @details Inicializa o objeto com um valor padrão válido ("01-JAN-2024-12:00") ou valida o parâmetro informado.
    /// @param valorInicial Objeto std::string contendo o timestamp inicial (padrão: "01-JAN-2024-12:00").
    /// @throw std::invalid_argument Lançada se valorInicial não atender aos critérios de validação.
    ///
    explicit Timestamp(const std::string& valorInicial = "01-JAN-2024-12:00") {
        setTimeStamp(valorInicial);
    }

    ///
    /// @brief Atribui e valida o valor do timestamp
    /// @details Invoca o método privado validar() e, se aceito, armazena o novo timestamp.
    /// @param novoValor Objeto std::string contendo a nova data e horário.
    /// @throw std::invalid_argument Lançada se novoValor for inválido.
    ///
    void setTimeStamp(const std::string& novoValor) {
        validar(novoValor); // Garante a validação antes de persistir na memória
        this->timestamp = novoValor;
    }

    ///
    /// @brief Obtém o timestamp armazenado
    /// @return Objeto std::string contendo o carimbo de data e hora validado.
    ///
    std::string getTimeStamp() const {
        return timestamp;
    }
};


#endif // DOMINIOS_HPP_INCLUDED
