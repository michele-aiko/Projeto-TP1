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
/// @param Objeto std::string contendo o texto a ser validado.
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
/// @param Objeto std::string contendo o estado inicial (padrão: A_FAZER).
/// @throw std::invalid_argument Lançada se o estadoInicial informado for inválido.
///
    explicit Estado(const std::string& estadoInicial = A_FAZER) { setEstado(estadoInicial); }

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
/// @brief Representa o domínio Nome a ser utilizado nas 4 entidades (Pessoa, Projeto, Quadro e Cartão de Atividade)
/// @details Texto de até 15 caracteres composto por letras, números, caracteres especiais e espaços.
///
class Nome {
    private:
        std::string nome;///< Objeto std::string contendo o texto do nome a ser armazenado.

    bool validarNome(const string& novoNome) const {
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
