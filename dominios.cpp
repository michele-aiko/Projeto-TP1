#include "dominios.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>


// Metodos email
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


// Metodos identificador
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



// Metodos Limite
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



// Metodos senha 
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


// Metodos timestamp

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

//calcula minutos desde 2000, usado para calcular diferença de valores
long Timestamp::paraMinutos() const {
    stringstream ss(valor);
    string sDia, mes, sAno, horario;
    getline(ss, sDia, '-');
    getline(ss, mes, '-');
    getline(ss, sAno, '-');
    getline(ss, horario);

    int dia = stoi(sDia);
    int ano = stoi(sAno);
    int hora = stoi(horario.substr(0, 2));
    int minuto = stoi(horario.substr(3, 2));

    static const string meses[] = {"JAN","FEV","MAR","ABR","MAI","JUN","JUL","AGO","SET","OUT","NOV","DEZ"};

    long totalDias = 0;
    for (int a = 2000; a < ano; a++) {
        totalDias += ehBissexto(a) ? 366 : 365;
    }
    for (int i = 0; i < 12; i++) {
        if (mes == meses[i]) break;
        totalDias += obterDiasNoMes(meses[i], ano);
    }
    totalDias += dia - 1;

    return totalDias * 1440 + hora * 60 + minuto;
}

