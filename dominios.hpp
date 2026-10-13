#ifndef DOMINIOS_HPP_INCLUDED
#define DOMINIOS_HPP_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <stdexcept>


using namespace std;

///
/// @brief Representa o domínio Estado de um cartão de atividade Kanban.
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
/// @param Objeto std::string contendo o texto a ser validado.
/// @throw std::invalid_argument Lançada se a string informada não for "A FAZER", "FAZENDO" ou "FEITO".
///
    void validar(const std::string& estado) const;

public:

    inline static const std::string A_FAZER = "A FAZER";
    inline static const std::string FAZENDO = "FAZENDO";
    inline static const std::string FEITO = "FEITO";

    ///
    /// @brief Construtor da classe Estado, deve incluir o método de validação do estado.
    /// @details Inicializa o objeto com o estado "A FAZER" por padrão ou valida o parâmetro informado.
    /// @param Objeto std::string contendo o estado inicial (padrão: Estado::A_FAZER).
    /// @throw std::invalid_argument Lançada se o estadoInicial informado for inválido.
    ///
    Estado(const std::string& estadoInicial = A_FAZER);

    ///
    /// @brief Atribui e valida o valor do estado
    /// @details Invoca o método validar() e, se aprovado, armazena o novo estado.
    /// @param Objeto std::string contendo o novo estado.
    /// @throw std::invalid_argument Lançada se o novoEstado for diferente das opções permitidas.
    ///
    void setEstado(const std::string& novoEstado);

    ///
    /// @brief Obtém o valor do estado armazenado
    /// @return Objeto std::string contendo o estado atual.
    ///
    string getEstado() const;

    ///
    /// @brief Retorna a lista contendo as opções válidas pré-existentes de estado.
    /// @return Objeto std::vector<std::string> contendo {"A FAZER", "FAZENDO", "FEITO"}.
    ///
    static std::vector<std::string> getOpcoes();
};

//CLASSE NOME AMIGOS TP1
class Nome {
    private:
        string valor;

    bool validar(const string& valorTestar) const {
        if (valorTestar.empty() || valorTestar.length() > 15){
            return false;
        }

        if (valorTestar.front() == ' ') {
            return false;
        }

        if (valorTestar.back() == ' ') {
            return false;
        }

        for (size_t i = 0; i < valorTestar.length(); i++){
            char c = valorTestar[i];
            if (!std::isalpha(c) && c != ' ') {
                return false;
            }

            if (c == ' ') {
                if (!std::isalpha(valorTestar[i + 1])){
                    return false;
                }
            }
        }
        return true;
    }
    public:
        Nome(const std::string& valorInicial = "Nome Padrao"){
            setValor(valorInicial);
        }

        void setValor(const std::string& novoValor) {
            if (!validar(novoValor)) {
                throw std::invalid_argument("Nome em formato invalido.");
            }
            valor = novoValor;
        }

        string getValor() const {
            return valor;
        }

};
//CLASSE TAMANHO AMIGOS TP1
class Tamanho {
    private:
        std::string valor;

    public:
        inline static const string PEQUENO = "Pequeno";
        inline static const string MEDIO = "Medio";
        inline static const string GRANDE = "Grande";

        Tamanho(const string& valorInicial = PEQUENO) : valor(valorInicial) {}
        void setValor(const string& novoValor){
            valor = novoValor;
        }

        string getValor() const {
            return valor;
        }

        static vector<string> getOpcoes() {
            return {PEQUENO, MEDIO, GRANDE};
        }
};
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
/// @details Encapsula o endereço de e-mail no formato parte-local@domínio. A parte local tem até 64 caracteres e o domínio até 255. Deve ser incluído como chave primária da entidade Pessoa
///
class Email { //Formato válido: parte-local@dominio
        private:
            static const int LIMLOCAL = 64; 
            static const int LIMDOMINIO = 255; 
            string email; ///< Objeto primitivo string que armazena o e-mail
///
/// @brief Valida o e-mail e deve ser incluído no método setEmail
/// @details Analisa se o email informado pelo usuário segue a estrutura parte-local@domínio e respeita as regras permitidas: 
///          Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-), não pode iniciar ou terminar com ponto ou hífen, ponto ou hifen deve ser seguido de letra(s) ou digito(s)
/// @param Recebe string passada por referência
/// @throw std::invalid_argument Lançada se a string não atender às regras de validação.
///
            void validarEmail(string); 

        public:
///
/// @brief Armazena o e-mail a ser cadastrado, só será criado após a validação dos requisitos e se nenhuma exceção for lançada
/// @details Recebe uma string, invoca o método privado validar() e, se válido, cria o novo email do usuário.
/// @param Recebe string contendo o e-mail informado pelo usuário
/// @throw std::invalid_argument Lançada caso o formato do e-mail seja inválido.
///
            void setEmail(string);
///
/// @brief Obtém o e-mail do usuário, deve ser utilizado como método pela entidade Pessoa
/// @details Retorna uma cópia do objeto string contido no atributo email, não gera qualquer alteração desse objeto.
/// @return Objeto std::string contendo o endereço de e-mail.
///
            string getEmail() const;

};

inline void Email::validarEmail(string novoEmail){ //Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-), não pode iniciar ou terminar com ponto ou hífen, ponto ou hifen deve ser seguido de letra(s) ou digito(s)

	//Divide novoEmail entre as variáveis parteLocal e dominio
	size_t posArroba = novoEmail.find('@'); //Busca posição de '@' na string
	
	if(posArroba == string::npos){ //A função find retorna "npos" se não encontrar o '@'
		throw invalid_argument("Argumento inválido");
	}
	
	string parteLocal = novoEmail.substr(0, posArroba); //Armazena o caracter da posição '0' até a posição imediatamente anterior ao "@"
	string dominio = novoEmail.substr(posArroba + 1); //Omitindo o 1º parametro, armazena todos os caracteres da posição indicada até o caracter imediatamente anterior ao '/0'
	
	//Testar email conforme critérios estabelecidos nos requisitos não pode iniciar ou terminar com ponto ou hífen, ponto ou hifen deve ser seguido de letra(s) ou digito(s)
	//Limites
	if(parteLocal.length() > LIMLOCAL || dominio.length() > LIMDOMINIO){
		throw invalid_argument("Argumento inválido");
	}
	
    string partesEmail[2] = {parteLocal, dominio}; // O array neste formato permite passar as duas strings ao mesmo tempo pelos testes sem repetir código
    
    for(int p = 0; p < 2; p++) {
       	string str = partesEmail[p];
        
        // Verifica se a string está vazia ou inicia/termina com . ou -
       	if (str.empty() || str.front() == '.' || str.front() == '-' || str.back() == '.' || str.back() == '-') {
           		throw invalid_argument("Argumento inválido");
		}

    	//Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-)
    	for(size_t i = 0; i < str.length(); i++){
       		char c = str[i];
            
       		if(!isalnum(c) && c != '.' && c != '-'){
           		throw invalid_argument("Argumento inválido");
       		}
            
         // Ponto ou hífen seguido de letra ou dígito
       		if(c == '.' || c == '-'){
           		char proximo = str[i + 1];
             		if(!isalnum(proximo)){
                   			throw invalid_argument("Argumento inválido");
               		}
       		}
    	}
	}
}

inline void Email::setEmail(string novoEmail){
	validarEmail(novoEmail);
	this->email = novoEmail;
}

inline string Email::getEmail() const{
	return email;
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
