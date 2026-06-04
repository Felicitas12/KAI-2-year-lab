#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>

void level1() {
    std::cout << "\n=== LEVEL 1: Regular Expression ===\n";
    {
        std::ofstream out("level1.txt");
        out << "_+ABC123\n_-K1\n_+A99\nhello\n_BCD\n_+L5\n"
            "_-ABCDEFGHIJK999\n_+A0\n_-Z\n_+ABC1234\n";
    }
    std::ifstream file("level1.txt");
    if (!file.is_open()) { std::cerr << "Cannot open level1.txt\n"; return; }
    std::regex pattern(R"(_(\+|-)[A-K]+\d{1,3})");
    std::string line;
    std::cout << "Words matching _([+|-])[A-K]+\\d{1,3}:\n";
    while (std::getline(file, line))
        if (std::regex_match(line, pattern))
            std::cout << "  MATCH: " << line << "\n";
    file.close();
}

enum class State { S0, S1, S2, S3, S4, S5, S6, ERR };

State transition_switch(State state, char c) {
    switch (state) {
    case State::S0: return (c == '_') ? State::S1 : State::ERR;
    case State::S1: return (c == '+' || c == '-') ? State::S2 : State::ERR;
    case State::S2: return (c >= 'A' && c <= 'K') ? State::S3 : State::ERR;
    case State::S3:
        if (c >= 'A' && c <= 'K') return State::S3;
        if (c >= '0' && c <= '9') return State::S4;
        return State::ERR;
    case State::S4: return (c >= '0' && c <= '9') ? State::S5 : State::ERR;
    case State::S5: return (c >= '0' && c <= '9') ? State::S6 : State::ERR;
    case State::S6: return State::ERR;
    default:        return State::ERR;
    }
}

bool isAccepting(State s) {
    return s == State::S4 || s == State::S5 || s == State::S6;
}

bool validate_switch(const std::string& word) {
    State state = State::S0;
    for (char c : word) {
        state = transition_switch(state, c);
        if (state == State::ERR) return false;
    }
    return isAccepting(state);
}

void level2() {
    std::cout << "\n=== LEVEL 2: Finite Automaton (switch) ===\n";
    std::string input;
    std::cout << "Enter a word: ";
    std::cin >> input;
    std::cout << "\"" << input << "\" -> "
        << (validate_switch(input) ? "VALID" : "INVALID") << "\n";
}

enum class St { S0, S1, S2, S3, S4, S5, S6, ERR };
enum class CC { UNDER, SIGN, LETTER_AK, DIGIT, OTHER };

CC classify(char c) {
    if (c == '_')             return CC::UNDER;
    if (c == '+' || c == '-')     return CC::SIGN;
    if (c >= 'A' && c <= 'K')     return CC::LETTER_AK;
    if (c >= '0' && c <= '9')     return CC::DIGIT;
    return CC::OTHER;
}

std::map<std::pair<St, CC>, St> buildTable() {
    return {
        {{St::S0, CC::UNDER},     St::S1},
        {{St::S1, CC::SIGN},      St::S2},
        {{St::S2, CC::LETTER_AK}, St::S3},
        {{St::S3, CC::LETTER_AK}, St::S3},
        {{St::S3, CC::DIGIT},     St::S4},
        {{St::S4, CC::DIGIT},     St::S5},
        {{St::S5, CC::DIGIT},     St::S6},
    };
}

bool isAccepting3(St s) {
    return s == St::S4 || s == St::S5 || s == St::S6;
}

bool validate_table(const std::string& word) {
    static auto table = buildTable();
    St state = St::S0;
    for (char c : word) {
        auto it = table.find({ state, classify(c) });
        state = (it != table.end()) ? it->second : St::ERR;
        if (state == St::ERR) return false;
    }
    return isAccepting3(state);
}

std::vector<std::string> splitByDelimiters(const std::string& text) {
    std::regex delim(R"(#!|\?!|\*!)");
    std::sregex_token_iterator it(text.begin(), text.end(), delim, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> tokens;
    for (; it != end; ++it) {
        std::string tok = it->str();
        if (!tok.empty()) tokens.push_back(tok);
    }
    return tokens;
}

void level3() {
    std::cout << "\n=== LEVEL 3: Transition Table + for loop ===\n";
    {
        std::ofstream out("level3.txt");
        out << "_+ABC1#!_-K99?!hello*!_+ABCDEFGHIJK1?!_-A1234#!_+A1\n";
    }
    std::ifstream file("level3.txt");
    if (!file.is_open()) { std::cerr << "Cannot open level3.txt\n"; return; }
    std::string line, text;
    while (std::getline(file, line)) text += line;
    file.close();

    std::cout << "Transition table:\n"
        << "  State | _   | +/- | A-K | 0-9 | other\n"
        << "  ------+-----+-----+-----+-----+------\n"
        << "  S0    | S1  | ERR | ERR | ERR | ERR\n"
        << "  S1    | ERR | S2  | ERR | ERR | ERR\n"
        << "  S2    | ERR | ERR | S3  | ERR | ERR\n"
        << "  S3    | ERR | ERR | S3  | S4  | ERR\n"
        << "  S4    | ERR | ERR | ERR | S5  | ERR\n"
        << "  S5    | ERR | ERR | ERR | S6  | ERR\n"
        << "  S6    | ERR | ERR | ERR | ERR | ERR\n\n";

    std::vector<std::string> words = splitByDelimiters(text);
    std::cout << "Results:\n";
    for (const auto& w : words)
        std::cout << "  \"" << w << "\" -> "
        << (validate_table(w) ? "VALID" : "INVALID") << "\n";
}

int main() {
    level1();
    level2();
    level3();
    return 0;
}