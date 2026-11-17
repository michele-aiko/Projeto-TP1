#include "dominios.hpp"
#include <cctype>
#include <sstream>
#include <stdexcept>

// =============================================================================
// METODOS DA CLASSE TEXTO
// =============================================================================
void Texto::validar(const std::string& texto) const {
    if (texto.empty() || texto.length() > 30) {
        throw std::invalid_argument("Texto invalido: deve conter entre 1 e 30 caracteres.");
    }
    if (!std::isupper(static_cast<unsigned char>(texto.front()))) {
        throw std::invalid_argument("Texto invalido: o primeiro caractere deve ser letra maiuscula.");
    }
    if (texto.back() != '.') {
        throw std::invalid_argument("Texto invalido: o ultimo caractere deve ser um ponto (.).");
    }

    for (std::size_t i = 0; i < texto.length(); ++i) {
        char c = texto[i];
        bool ehPontuacao = (c == ',' || c == ';' || c == '.' || c == ':' || c == '?' || c == '!');
        
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && !ehPontuacao) {
            throw std::invalid_argument("Texto invalido: contem caractere nao permitido.");
        }

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

// =============================================================================
// METODOS DA CLASSE NOME
// =============================================================================
void Nome::validar(const std::string& nome) const {
    if (nome.empty() || nome.length() > 15) {
        throw std::invalid_argument("Nome invalido: deve conter entre 1 e 15 caracteres.");
    }
    if (nome.front() == ' ' || nome.back() == ' ') {
        throw std::invalid_argument("Nome invalido: nao pode iniciar ou terminar com espaco em branco.");
    }

    for (std::size_t i = 0; i < nome.length(); ++i) {
        char c = nome[i];
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ') {
            throw std::invalid_argument("Nome invalido: deve conter apenas letras e espacos.");
        }
        if (c == ' ') {
            if (i + 1 < nome.length() && !std::isalpha(static_cast<unsigned char>(nome[i + 1]))) {
                throw std::invalid_argument("Nome invalido: espaco em branco deve ser seguido por letra.");
            }
        }
    }
}

// =============================================================================
// METODOS DA CLASSE EMAIL
// =============================================================================
void Email::validar(const std::string& novoEmail) const {
    std::size_t posArroba = novoEmail.find('@');
    if (posArroba == std::string::npos) {
        throw std::invalid_argument("Email invalido: ausencia do caractere '@'.");
    }

    std::string parteLocal = novoEmail.substr(0, posArroba);
    std::string dominio = novoEmail.substr(posArroba + 1);

    if (parteLocal.empty() || parteLocal.length() > LIMLOCAL ||
        dominio.empty() || dominio.length() > LIMDOMINIO) {
        throw std::invalid_argument("Email invalido: tamanho excedido para parte local ou dominio.");
    }

    std::string partesEmail[1] = {parteLocal, dominio};
    for (int p = 0; p < 2; ++p) {
        const std::string& str = partesEmail[p];
        if (str.empty() || str.front() == '.' || str.front() == '-' || str.back() == '.' || str.back() == '-') {
            throw std::invalid_argument("Email invalido: nao pode iniciar ou terminar com ponto ou hifen.");
        }

        for (std::size_t i = 0; i < str.length(); ++i) {
            char c = str[i];
            if (!std::isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '-') {
                throw std::invalid_argument("Email invalido: caractere nao permitido.");
            }
            if (c == '.' || c == '-') {
                if (i + 1 < str.length() && !std::isalnum(static_cast<unsigned char>(str[i + 1]))) {
                    throw std::invalid_argument("Email invalido: ponto ou hifen deve ser seguido por letra ou digito.");
                }
            }
        }
    }
}

// =============================================================================
// METODOS DA CLASSE IDENTIFICADOR
// =============================================================================
void Identificador::validar(const std::string& identificador) const {
    if (identificador.length() != TAMANHO) {
        throw std::invalid_argument("Identificador invalido: deve conter exatamente 6 caracteres.");
    }
    for (std::size_t i = 0; i < 3; ++i) {
        if (!std::isalpha(static_cast<unsigned char>(identificador[i]))) {
            throw std::invalid_argument("Identificador invalido: os 3 primeiros caracteres devem ser letras.");
        }
    }
    for (std::size_t i = 3; i < TAMANHO; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(identificador[i]))) {
            throw std::invalid_argument("Identificador invalido: os 3 ultimos caracteres devem ser digitos.");
        }
    }
}

// =============================================================================
// METODOS DA CLASSE LIMITE
// =============================================================================
void Limite::validar(int limite) const {
    if (limite < LIMITE_MINIMO || limite > LIMITE_MAXIMO) {
        throw std::invalid_argument("Limite invalido: o valor deve estar entre 1 e 25.");
    }
}

// =============================================================================
// METODOS DA CLASSE PAPEL
// =============================================================================
void Papel::validar(const std::string& papel) const {
    if (papel != GESTOR && papel != DESENVOLVEDOR) {
        throw std::invalid_argument("Papel invalido: O valor deve ser 'GESTOR' ou 'DESENVOLVEDOR'.");
    }
}

// =============================================================================
// METODOS DA CLASSE PRIORIDADE
// =============================================================================
void Prioridade::validar(const std::string& prioridade) const {
    if (prioridade != ALTA && prioridade != MEDIA && prioridade != BAIXA) {
        throw std::invalid_argument("Prioridade invalida: O valor deve ser 'ALTA', 'MEDIA' ou 'BAIXA'.");
    }
}

// =============================================================================
// METODOS DA CLASSE SENHA
// =============================================================================
void Senha::validar(const std::string& senha) const {
    if (senha.length() != TAMANHO) {
        throw std::invalid_argument("Senha invalida: deve conter exatamente 5 caracteres.");
    }

    bool possuiLetra = false;
    bool possuiDigito = false;

    for (std::size_t i = 0; i < TAMANHO; ++i) {
        unsigned char c = static_cast<unsigned char>(senha[i]);
        if (!std::isalnum(c)) {
            throw std::invalid_argument("Senha invalida: contem caracteres nao permitidos.");
        }
        if (std::isalpha(c)) possuiLetra = true;
        if (std::isdigit(c)) possuiDigito = true;
    }

    if (!possuiLetra || !possuiDigito) {
        throw std::invalid_argument("Senha invalida: deve conter pelo menos uma letra e pelo menos um digito.");
    }
}

// =============================================================================
// METODOS DA CLASSE TIMESTAMP
// =============================================================================
bool Timestamp::ehBissexto(int ano) const {
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
    return 0;
}

void Timestamp::validar(const std::string& timestamp) const {
    if (timestamp.length() < 15 || timestamp.length() > 16) {
        throw std::invalid_argument("Timestamp invalido: tamanho incorreto.");
    }

    std::size_t primeiroHifen = timestamp.find('-');
    if (primeiroHifen == std::string::npos || primeiroHifen < 1 || primeiroHifen > 2) {
        throw std::invalid_argument("Timestamp invalido: delimitador do dia incorreto.");
    }
    std::string strDia = timestamp.substr(0, primeiroHifen);

    std::size_t segundoHifen = timestamp.find('-', primeiroHifen + 1);
    if (segundoHifen == std::string::npos) {
        throw std::invalid_argument("Timestamp invalido: delimitador do mes incorreto.");
    }
    std::string strMes = timestamp.substr(primeiroHifen + 1, segundoHifen - (primeiroHifen + 1));
    if (strMes.length() != 3) {
        throw std::invalid_argument("Timestamp invalido: a sigla do mes deve conter exatamente 3 letras.");
    }

    std::size_t terceiroHifen = timestamp.find('-', segundoHifen + 1);
    if (terceiroHifen == std::string::npos) {
        throw std::invalid_argument("Timestamp invalido: delimitador do ano incorreto.");
    }
    std::string strAno = timestamp.substr(segundoHifen + 1, terceiroHifen - (segundoHifen + 1));
    if (strAno.length() != 4) {
        throw std::invalid_argument("Timestamp invalido: o ano deve conter exatamente 4 digitos.");
    }

    std::string strHorario = timestamp.substr(terceiroHifen + 1);
    if (strHorario.length() != 5 || strHorario[1] != ':') {
        throw std::invalid_argument("Timestamp invalido: horario deve estar no formato HH:MM.");
    }

    std::string strHora = strHorario.substr(0, 2);
    std::string strMinuto = strHorario.substr(3, 2);

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

    int dia = std::stoi(strDia);
    int ano = std::stoi(strAno);
    int hora = std::stoi(strHora);
    int minuto = std::stoi(strMinuto);

    if (ano < 2000 || ano > 2099) {
        throw std::invalid_argument("Timestamp invalido: o ano deve estar entre 2000 e 2099.");
    }

    int maxDias = obterDiasNoMes(strMes, ano);
    if (maxDias == 0) {
        throw std::invalid_argument("Timestamp invalido: sigla do mes nao reconhecida.");
    }

    if (dia < 1 || dia > maxDias) {
        throw std::invalid_argument("Timestamp invalido: dia incompativel com o mes e ano informados.");
    }

    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
        throw std::invalid_argument("Timestamp invalido: horario fora da faixa permitida (00:00 a 23:59).");
    }
}
