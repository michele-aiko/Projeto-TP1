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
		void validarEstado(const std::string& estado) const;
	
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

	inline void Estado::validarEstado(const std::string& novoEstado) const {
	    std::vector<std::string> opcoes = getOpcoes();
	    
	    // Busca se o valor informado está entre as opções permitidas
	    if (std::find(opcoes.begin(), opcoes.end(), novoEstado) == opcoes.end()) { //Se find não encontra o elemento procurado, retorna o ponteiro para o final da lista
	        throw std::invalid_argument("Estado invalido: O valor deve ser 'A FAZER', 'FAZENDO' ou 'FEITO'.");
	    }
	}

	inline void Estado::setEstado(const std::string& novoEstado) {
	    validarEstado(novoEstado);
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
		    void validarNome(const std::string& nome) const;
		
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
	    validarNome(novoNome); // Se for inválido, dispara std::invalid_argument
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
		    void validarTamanho(const std::string& tamanho) const;
		
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
    	validarTamanho(novoTamanho); // Se for inválido, lança std::invalid_argument
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
		    void validarTexto(const std::string& texto) const;
		
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
            void validarEmail(string); 

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
	validarEmail(novoEmail);
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
		    void validarIdentificador(const std::string& identificador) const;
		
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
		        validarIdentificador(novoIdentificador); // Garante a execução das regras antes da atribuição
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



//papel dos usuarios
class Papel {
private:
    string valor;

    void validar(const string& valor) {
        if (valor != GESTOR && valor != DESENVOLVEDOR) {
            throw invalid_argument("Argumento invalido.");
        }
    }

public:
    inline static const string GESTOR = "GESTOR";
    inline static const string DESENVOLVEDOR = "DESENVOLVEDOR";

    Papel(const string& valorInicial = GESTOR) {
        setValor(valorInicial);
    }

    void setValor(const string& novoValor) {
        validar(novoValor);
        valor = novoValor;
    }

    string getValor() const {
        return valor;
    }

    static vector<string> getOpcoes() {
        return {GESTOR, DESENVOLVEDOR};
    }
};

//classe das prioridades
class Prioridade {
private:
    string valor;

    void validar(const string& valor) {
        if (valor != ALTA && valor != MEDIA && valor != BAIXA) {
            throw invalid_argument("Argumento invalido.");
        }
    }

public:
    inline static const string ALTA = "ALTA";
    inline static const string MEDIA = "MEDIA";
    inline static const string BAIXA = "BAIXA";

    Prioridade(const string& valorInicial = BAIXA) {
        setValor(valorInicial);
    }

    void setValor(const string& novoValor) {
        validar(novoValor);
        valor = novoValor;
    }

    string getValor() const {
        return valor;
    }

    static vector<string> getOpcoes() {
        return {ALTA, MEDIA, BAIXA};
    }
};

//classe das senhas
class Senha {
private:
    static const int TAMANHO = 5;
    string valor;
    void validar(const string& valor);

public:
    void setValor(const string& valor);
    string getValor() const;
};


//classe da data, mais dificil
class Timestamp {
private:
    string valor;

    bool ehBissexto(int ano) const;
    int obterDiasNoMes(const string& mes, int ano) const;
    void validar(const string& valor);

public:
    void setValor(const string& valor);
    string getValor() const;
};

#endif // DOMINIOS_HPP_INCLUDED
