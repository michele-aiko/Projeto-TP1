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
        return true:
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
//classe email!!1!
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

bool Email::eAlfanumerico(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool Email::validarParteLocal(const string& local) const {
    if (local.empty() || local.length() > 64) return false;

    if (!eAlfanumerico(local.front()) || !eAlfanumerico(local.back())) return false;

    for (size_t i = 0; i < local.length(); i++) {
        char c = local[i];

        if (!eAlfanumerico(c) && c != '.' && c != '-') {
            return false;
        }

        if (c == '.' || c == '-') {
            if (i + 1 >= local.length() || !eAlfanumerico(local[i + 1])) {
                return false;
            }
        }
    }
    return true;
}

bool Email::validarDominio(const string& dominio) const {
    if (dominio.empty() || dominio.length() > 255) return false;

    if (dominio.front() == '.' || dominio.front() == '-' ||
        dominio.back() == '.' || dominio.back() == '-') {
        return false;
    }

    size_t inicioParte = 0;
    while (inicioParte < dominio.length()) {
        size_t fimParte = dominio.find('.', inicioParte);
        if (fimParte == string::npos) {
            fimParte = dominio.length();
        }

        string parte = dominio.substr(inicioParte, fimParte - inicioParte);

        if (parte.empty()) return false;

        if (parte.front() == '-' || parte.back() == '-') return false;

        for (char c : parte) {
            if (!eAlfanumerico(c) && c != '-') {
                return false;
            }
        }

        inicioParte = fimParte + 1;
    }

    return true;
}

void Email::validar(const string& email) {
    size_t posicaoArroba = email.find('@');

    if (posicaoArroba == string::npos || email.find('@', posicaoArroba + 1) != string::npos) {
        throw invalid_argument("Argumento invalido.");
    }

    string parteLocal = email.substr(0, posicaoArroba);
    string dominio = email.substr(posicaoArroba + 1);

    if (!validarParteLocal(parteLocal) || !validarDominio(dominio)) {
        throw invalid_argument("Argumento invalido.");
    }
}

void Email::setValor(const string& valor) {
    validar(valor);
    this->valor = valor;
}

string Email::getValor() const {
    return valor;
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

void Identificador::validar(const string& valor) {
    if (valor.length() != TAMANHO) {
        throw invalid_argument("Argumento invalido.");
    }

    for (int i = 0; i < 3; i++) {
        if (!isalpha(valor[i])) {
            throw invalid_argument("Argumento invalido.");
        }
    }

    for (int i = 3; i < 6; i++) {
        if (!isdigit(valor[i])) {
            throw invalid_argument("Argumento invalido.");
        }
    }
}

void Identificador::setValor(const string& valor) {
    validar(valor);
    this->valor = valor;
}

string Identificador::getValor() const {
    return valor;
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

void Limite::validar(int valor) {
    if (valor < 1 || valor > 25) {
        throw invalid_argument("Argumento invalido.");
    }
}

void Limite::setValor(int valor) {
    validar(valor);
    this->valor = valor;
}

int Limite::getValor() const {
    return valor;
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

void Senha::validar(const string& valor) {
    if (valor.length() != TAMANHO) {
        throw invalid_argument("Argumento invalido.");
    }

    bool temLetra = false;
    bool temDigito = false;

    for (char c : valor) {
        if (isalpha(c)) {
            temLetra = true;
        } else if (isdigit(c)) {
            temDigito = true;
        } else {
            throw invalid_argument("Argumento invalido.");
        }
    }

    if (!temLetra || !temDigito) {
        throw invalid_argument("Argumento invalido.");
    }
}

void Senha::setValor(const string& valor) {
    validar(valor);
    this->valor = valor;
}

string Senha::getValor() const {
    return valor;
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

bool Timestamp::ehBissexto(int ano) const {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int Timestamp::obterDiasNoMes(const string& mes, int ano) const {
    if (mes == "JAN") return 31;
    if (mes == "FEV") return ehBissexto(ano) ? 29 : 28;
    if (mes == "MAR") return 31;
    if (mes == "ABR") return 30;
    if (mes == "MAI") return 31;
    if (mes == "JUN") return 30;
    if (mes == "JUL") return 31;
    if (mes == "AGO") return 31;
    if (mes == "SET") return 30;
    if (mes == "OUT") return 31;
    if (mes == "NOV") return 30;
    if (mes == "DEZ") return 31;
    return 0;
}

void Timestamp::validar(const string& valor) {
    stringstream ss(valor);
    string sDia, mes, sAno, horario;

    if (!getline(ss, sDia, '-') ||
        !getline(ss, mes, '-') ||
        !getline(ss, sAno, '-') ||
        !getline(ss, horario)) {
        throw invalid_argument("Argumento invalido.");
    }

    // Verifica se sobrou algum trecho na string
    if (ss.rdbuf()->in_avail() > 0) {
        throw invalid_argument("Argumento invalido.");
    }

    int dia;
    try {
        dia = stoi(sDia);
    } catch (...) {
        throw invalid_argument("Argumento invalido.");
    }

    int ano;
    try {
        ano = stoi(sAno);
    } catch (...) {
        throw invalid_argument("Argumento invalido.");
    }
    if (ano < 2000 || ano > 2099) {
        throw invalid_argument("Argumento invalido.");
    }

    // Validar MÊS e DIA conforme quantidade de dias no mês
    int maxDias = obterDiasNoMes(mes, ano);
    if (maxDias == 0 || dia < 1 || dia > maxDias) {
        throw invalid_argument("Argumento invalido.");
    }

    // Validar HORÁRIO (Formato HH:MM)
    if (horario.length() != 5 || horario[2] != ':') {
        throw invalid_argument("Argumento invalido.");
    }

    int hora, minuto;
    try {
        hora = stoi(horario.substr(0, 2));
        minuto = stoi(horario.substr(3, 2));
    } catch (...) {
        throw invalid_argument("Argumento invalido.");
    }

    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
        throw invalid_argument("Argumento invalido.");
    }
}

void Timestamp::setValor(const string& valor) {
    validar(valor);
    this->valor = valor;
}

string Timestamp::getValor() const {
    return valor;
};

#endif // DOMINIOS_HPP_INCLUDED
