#include "dominios.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>


// Metodos email
void Email::validarEmail(string novoEmail){ //Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-), não pode iniciar ou terminar com ponto ou hífen, ponto ou hifen deve ser seguido de letra(s) ou digito(s)

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

