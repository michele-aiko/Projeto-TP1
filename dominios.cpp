#include "dominios.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>

//Metodos texto
void Texto::validar(const std::string& texto) const {
    // 1. Verificação de tamanho (de 1 a 30 caracteres)
    if (texto.empty() || texto.length() > 30) {
        throw std::invalid_argument("Texto invalido: deve conter entre 1 e 30 caracteres.");
    }

    // 2. Primeiro caractere deve ser letra maiúscula (A-Z)
    if (!std::isupper(static_cast<unsigned char>(texto.front()))) {
        throw std::invalid_argument("Texto invalido: o primeiro caractere deve ser letra maiuscula.");
    }

    // 3. Último caractere deve ser obrigatoriamente um ponto (.)
    if (texto.back() != '.') {
        throw std::invalid_argument("Texto invalido: o ultimo caractere deve ser um ponto (.).");
    }

    // 4. Inspeção caractere a caractere
    for (std::size_t i = 0; i < texto.length(); ++i) {
        char c = texto[i];

        // Avalia se o caractere atual é um dos sinais de pontuação permitidos
        bool ehPontuacao = (c == ',' || c == ';' || c == '.' || c == ':' || c == '?' || c == '!');

        // Garante que o caractere seja apenas letra, dígito, espaço ou pontuação permitida
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && !ehPontuacao) {
            throw std::invalid_argument("Texto invalido: contem caractere nao permitido.");
        }

        // Verifica se uma pontuação é seguida por outra pontuação consecutiva
        if (ehPontuacao && (i + 1 < texto.length())) {
            char proximo = texto[i + 1];
            bool proximoEhPontuacao = (proximo == ',' || proximo == ';' || proximo == '.' || 
                                       proximo == ':' || proximo == '?' || proximo == '!');
            
            if (proximoEhPontuacao) {
                throw std::invalid_argument("Texto invalido: um sinal de pontuacao nao pode ser seguido por outro.");
            }
        }
    }
}

//Metodos nome
void Nome::validar(const std::string& nome) const {
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


// Metodos email
void Email::validar(string novoEmail){ //Pode conter letra (a-z), dígito (0-9) ou ponto(.) ou hífen (-), não pode iniciar ou terminar com ponto ou hífen, ponto ou hifen deve ser seguido de letra(s) ou digito(s)

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
void Identificador::validar(const std::string& identificador) const {
    // 1. Verificação do tamanho exato (deve ter exatamente 6 caracteres)
    if (identificador.length() != TAMANHO) {
        throw std::invalid_argument("Identificador invalido: deve conter exatamente 6 caracteres.");
    }

    // 2. Verificação dos 3 primeiros caracteres (devem ser letras A-Z ou a-z)
    for (std::size_t i = 0; i < 3; ++i) {
        if (!std::isalpha(static_cast<unsigned char>(identificador[i]))) {
            throw std::invalid_argument("Identificador invalido: os 3 primeiros caracteres devem ser letras.");
        }
    }

    // 3. Verificação dos 3 últimos caracteres (devem ser dígitos 0-9)
    for (std::size_t i = 3; i < TAMANHO; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(identificador[i]))) {
            throw std::invalid_argument("Identificador invalido: os 3 ultimos caracteres devem ser digitos.");
        }
    }
}

// Metodos Limite
void Limite::validar(int limite) {
    if (limite < 1 || limite > 25) {
        throw invalid_argument("Argumento invalido.");
    }
}

// Metodos Papel
void Papel::validar(const std::string& papel) const { // Compara diretamente com as constantes públicas da classe 
	if (papel != GESTOR && papel != DESENVOLVEDOR) { 
		throw std::invalid_argument("Papel invalido: O valor deve ser 'GESTOR' ou 'DESENVOLVEDOR'."); 
	} 
}

// Metodos Prioridade
void Prioridade::validar(const std::string& prioridade) const {
    // Compara diretamente com as constantes públicas da classe
    if (prioridade != ALTA && prioridade != MEDIA && prioridade != BAIXA) {
        throw std::invalid_argument("Prioridade invalida: O valor deve ser 'ALTA', 'MEDIA' ou 'BAIXA'.");
    }
}


// Metodos senha 
void Senha::validar(const std::string& senha) const {
    // 1. Verificação de comprimento exato (5 caracteres)
    if (senha.length() != TAMANHO) {
        throw std::invalid_argument("Senha invalida: deve conter exatamente 5 caracteres.");
    }

    bool possuiLetra = false;
    bool possuiDigito = false;

    // 2. Inspeção caractere a caractere
    for (std::size_t i = 0; i < TAMANHO; ++i) {
        unsigned char c = static_cast<unsigned char>(senha[i]);

        // Caracteres permitidos: apenas letras (a-z, A-Z) e dígitos (0-9)
        if (!std::isalnum(c)) {
            throw std::invalid_argument("Senha invalida: contem caracteres nao permitidos (apenas letras e digitos).");
        }

        if (std::isalpha(c)) {
            possuiLetra = true;
        }

        if (std::isdigit(c)) {
            possuiDigito = true;
        }
    }

    // 3. Garantia de conter ao menos uma letra e ao menos um dígito
    if (!possuiLetra || !possuiDigito) {
        throw std::invalid_argument("Senha invalida: deve conter pelo menos uma letra e pelo menos um digito.");
    }
}


// Metodos timestamp
bool Timestamp::ehBissexto(int ano) const {
    // Um ano é bissexto se é divisível por 4 e não por 100, ou se é divisível por 400
    return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0));
}

int Timestamp::obterDiasNoMes(const std::string& mes, int ano) const {
    if (mes == "JAN" || mes == "MAR" || mes == "MAI" || mes == "JUL" ||
        mes == "AGO" || mes == "OUT" || mes == "DEZ") {
        return 31;
    }
    if (mes == "ABR" || mes == "JUN" || mes == "SET" || mes == "NOV") {
        return 30;
    }
    if (mes == "FEV") {
        return ehBissexto(ano) ? 29 : 28;
    }
    return 0; // Mês inválido
}

void Timestamp::validar(const std::string& timestamp) const {
    // Formato esperado: "DD-MES-AAAA-HH:MM" (16 caracteres) ou "D-MES-AAAA-HH:MM" (15 caracteres)
    if (timestamp.length() < 15 || timestamp.length() > 16) {
        throw std::invalid_argument("Timestamp invalido: tamanho incorreto.");
    }

    // 1. Extração da parte do DIA (antes do primeiro hífen)
    std::size_t primeiroHifen = timestamp.find('-');
    if (primeiroHifen == std::string::npos || primeiroHifen < 1 || primeiroHifen > 2) {
        throw std::invalid_argument("Timestamp invalido: delimitador do dia incorreto.");
    }
    std::string strDia = timestamp.substr(0, primeiroHifen);

    // 2. Extração da parte do MÊS (entre o primeiro e o segundo hífen)
    std::size_t segundoHifen = timestamp.find('-', primeiroHifen + 1);
    if (segundoHifen == std::string::npos) {
        throw std::invalid_argument("Timestamp invalido: delimitador do mes incorreto.");
    }
    std::string strMes = timestamp.substr(primeiroHifen + 1, segundoHifen - (primeiroHifen + 1));
    if (strMes.length() != 3) {
        throw std::invalid_argument("Timestamp invalido: a sigla do mes deve conter exatamente 3 letras.");
    }

    // 3. Extração da parte do ANO (entre o segundo e o terceiro hífen)
    std::size_t terceiroHifen = timestamp.find('-', segundoHifen + 1);
    if (terceiroHifen == std::string::npos) {
        throw std::invalid_argument("Timestamp invalido: delimitador do ano incorreto.");
    }
    std::string strAno = timestamp.substr(segundoHifen + 1, terceiroHifen - (segundoHifen + 1));
    if (strAno.length() != 4) {
        throw std::invalid_argument("Timestamp invalido: o ano deve conter exatamente 4 digitos.");
    }

    // 4. Extração da parte do HORÁRIO (após o terceiro hífen)
    std::string strHorario = timestamp.substr(terceiroHifen + 1);
    if (strHorario.length() != 5 || strHorario != ':') {
        throw std::invalid_argument("Timestamp invalido: horario deve estar no formato HH:MM.");
    }
    std::string strHora = strHorario.substr(0, 2);
    std::string strMinuto = strHorario.substr(3, 2);

    // 5. Validação de dígitos numéricos
    auto contemApenasDigitos = [](const std::string& s) {
        for (char c : s) {
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    };

    if (!contemApenasDigitos(strDia) || !contemApenasDigitos(strAno) ||
        !contemApenasDigitos(strHora) || !contemApenasDigitos(strMinuto)) {
        throw std::invalid_argument("Timestamp invalido: dia, ano, hora e minuto devem ser compostos por digitos.");
    }

    // Conversão de strings para inteiros
    int dia = std::stoi(strDia);
    int ano = std::stoi(strAno);
    int hora = std::stoi(strHora);
    int minuto = std::stoi(strMinuto);

    // 6. Validação do ANO (2000 a 2099)
    if (ano < 2000 || ano > 2099) {
        throw std::invalid_argument("Timestamp invalido: o ano deve estar entre 2000 e 2099.");
    }

    // 7. Validação do MÊS e do DIA considerando bissextos
    int maxDias = obterDiasNoMes(strMes, ano);
    if (maxDias == 0) {
        throw std::invalid_argument("Timestamp invalido: sigla do mes nao reconhecida.");
    }
    if (dia < 1 || dia > maxDias) {
        throw std::invalid_argument("Timestamp invalido: dia incompativel com o mes e ano informados.");
    }

    // 8. Validação do HORÁRIO (00:00 a 23:59)
    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
        throw std::invalid_argument("Timestamp invalido: horario fora da faixa permitida (00:00 a 23:59).");
    }
}

