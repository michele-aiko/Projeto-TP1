#ifndef DOMINIOS_HPP_INCLUDED
#define DOMINIOS_HPP_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <stdexcept>


using namespace std;

//CLASSE ESTADO AMIGOS TP1
class Estado {
    private:
        std::string valor;

    public:
        inline static const string PENDENTE = "Pendente";
        inline static const string FAZENDO = "Fazendo";
        inline static const string CONCLUIDA = "Concluida";

        Estado(const string& valorInicial = PENDENTE) : valor(valorInicial) {}
        void setValor(const string& novoValor){
            valor = novoValor;
        }

        string getValor() const {
            return valor;
        }

        static vector<string> getOpcoes() {
            return {PENDENTE, FAZENDO, CONCLUIDA};
        }
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

class Email { //Formato válido: parte-local@dominio
        private:
            static const int LIMLOCAL = 64; 
            static const int LIMDOMINIO = 255; 
            string parteLocalEmail;
            string dominioEmail;
            void validarEmail(string); 

        public:
	    //Construtores precisa??
            Email();
            Email(string,string);
	    //Setters e Getters
            void setEmail(string);
            string getEmail() const;

};

inline void Email::validarEmail(string novoEmail){ //Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-), não pode iniciar ou terminar com ponto ou hífen, ponto ou hifen deve ser seguido de letra(s) ou digito(s)
	string parteLocal;
	string dominio;

	//Divide novoEmail entre as variáveis parteLocal e dominio
	size_t posArroba = novoEmail.find('@'); //Busca posição de '@' na string
	if(posArroba == string::npos){ //A função find retorna "npos" se não encontrar o caracter de busca
		throw invalid_argument("Argumento inválido");
	}
	parteLocal = novoEmail.substr(0, posArroba); //Armazena o caracter da posição '0' até a posição imediatamente anterior ao "@"
	dominio = novoEmail.substr(posArroba + 1); //Omitindo o 1º parametro, armazena todos os caracteres da posição indicada até o caracter imediatamente anterior ao '/0'
	
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
	
	string parteLocal;
	string dominio;
	
	size_t posArroba = novoEmail.find('@');
	parteLocal = novoEmail.substr(0, posArroba);
	dominio = novoEmail.substr(posArroba + 1);
	
	this->parteLocalEmail = parteLocal;
	this->dominioEmail = dominio;
}

inline string Email::getEmail() const{
	string email = parteLocalEmail + '@' + dominioEmail;
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
