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
/// @brief Representa o domínio Estado de um cartão de atividade.
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

	inline void Nome::validarNome(const std::string& nome) const {
	    // 1. Verifica se a string é vazia ou excede o limite máximo de 15 caracteres
	    if (nome.empty() || nome.length() > 15) {
	        throw std::invalid_argument("Nome invalido: deve conter entre 1 e 15 caracteres.");
	    }
	
	    // 2. Verifica se o primeiro ou o último caractere é um espaço em branco
	    if (nome.front() == ' ' || nome.back() == ' ') {
	        throw std::invalid_argument("Nome invalido: nao pode iniciar ou terminar com espaco em branco.");
	    }
	
	    // 3. Percorre cada caractere da string para verificar os tipos de caracteres e regras de espaço
	    for (std::size_t i = 0; i < nome.length(); ++i) {
	        char c = nome[i];
	
	        // Garante que o caractere seja apenas letra (A-Z, a-z) ou espaço
	        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {
	            throw std::invalid_argument("Nome invalido: deve conter apenas letras e espacos.");
	        }
	
	        // Se for um espaço em branco, verifica se o caractere seguinte é uma letra
	        if (c == ' ') {
	            if (!std::isalpha(static_cast<unsigned char>(nome[i + 1]))) {
	                throw std::invalid_argument("Nome invalido: espaco em branco deve ser seguido por letra.");
	            }
	        }
	    }
	}

	inline void Nome::setNome(const std::string& novoNome) {
	    validarNome(novoNome); // Se for inválido, dispara std::invalid_argument
	    this->nome = novoNome;
	}

///
/// @brief Representa o domínio Tamanho, sugestão de uso no cartão de atividades tipo Kanban.
/// @details Limita o tamanho estimado das tarefas as opções pré-definidas:
///          "PEQUENO", "MEDIO" ou "GRANDE". Qualquer tentativa de atribuir um valor fora 
///          desta lista resultará no lançamento de exceção.
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


//CLASSE TEXTO AMIGOS TP1
class Texto {
    private:
        string valor;

        bool ehPontuacao(char c) const {
            return (c == ',' || c == ';' || c == '.' || c == ':' || c == '?' || c == '!');
        }

        bool validar(const string& valorTestar) const {
            if (valorTestar.empty() || valorTestar.length() > 30){
                return false;
            }

            if (!std::isupper(valorTestar.front())){
                return false;
            }

            for (size_t i = 0; i < valorTestar.length(); i++){
                char c = valorTestar[i];
                if (!std::isalnum(c) && c != ' ' && !ehPontuacao(c)){
                    return false;
                }

                if (ehPontuacao(c) && i + 1 < valorTestar.length()){
                    if (ehPontuacao(valorTestar[i + 1])){
                        return false;
                    }
                }
            }
            return true;
        }
    public:
        void setValor(const string& novoValor){
            if (!validar(novoValor)){
                throw std::invalid_argument("Texto invalido.");
            }
            valor = novoValor;
        }

        Texto(const string& valorInicial = "Texto valido."){
            setValor(valorInicial);
        }

        string getValor() const {
            return valor;
        }
};

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
			explicit Email(const std::string& novoEmail = "parte-local@dominio") { setEmail(novoEmail) }
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


//CLASSE IDENTIFICADOR
class Identificador {
private:
    static const int TAMANHO = 6;
    string valor;
    void validar(const string& valor);

public:
    void setValor(const string& valor);
    string getValor() const;
};


//classe limite(?)
class Limite {
private:
    int valor;
    void validar(int valor);

public:
    void setValor(int valor);
    int getValor() const;
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
