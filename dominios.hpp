#ifndef DOMINIOS_HPP_INCLUDED
#define DOMINIOS_HPP_INCLUDED

#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <stdexcept>


// [17:46, 08/09/2026] Joyce unb: Eu terminei o "email" aquele dia, aí comecei o "identificador"
// [17:48, 08/09/2026] Joyce unb: Posso fazer também o "limite","papel","prioridade", "senha" e "timestamp" que já aproveito uns códigos de outros exercícios

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

//classe email
class Email {
private:
    string valor;
    void validar(const string& email);

    bool eAlfanumerico(char c) const;
    bool validarParteLocal(const string& local) const;
    bool validarDominio(const string& dominio) const;

public:
    void setValor(const string& valor);
    string getValor() const;
};


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
