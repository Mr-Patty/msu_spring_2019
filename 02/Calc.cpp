#include <iostream>
#include <sstream>
#include <string>
#include <exception>

using namespace std;

enum Token {
    Error,
    Number,
    End,
    Plus,
    Minus,
    Mul,
    Div
};

struct Tokenizer {
    int64_t number = 0;
    char prev;
    Token t;

    Tokenizer(Token y, char prev = 0, int64_t x = 0) : number(x), prev(prev), t(y) {}
};

Tokenizer token(istream* exp) {
    char c;
    while(exp->get(c)) {
        switch (c) {
            case ' ':
                continue;
            case '-':
                return Tokenizer(Minus, '-');
            case '+':
                return Tokenizer(Plus, '+');
            case '*':
                return Tokenizer(Mul, '*');
            case '/':
                return Tokenizer(Div, '/');
            default:
                if (c >= '0' && c <= '9')
                    return Tokenizer(Number, c, c - '0');
                else
                    return Tokenizer(Error, c);
        }
    }
    return Tokenizer(End);

}

int64_t prim(istream* exp) {
    Tokenizer res = token(exp);

    switch (res.t) {
        case Minus:
            return -prim(exp);
        case Plus:
            return prim(exp);
        case Number:
            return res.number;
        case Error:
            throw invalid_argument("invalid expression");
        case End:
            throw logic_error("End expression");
    }
}

int64_t term(istream* exp) {
    int64_t left = prim(exp);

    while (true) {
        Tokenizer res = token(exp);

        switch(res.t) {
            case Mul:
                left *= prim(exp);
                break;
            case Div: {
                int64_t d = prim(exp);
                if (d == 0)
                    throw runtime_error("Divide by Zero");
                else
                    left /= d;
                break;
            }
            case Error:
                throw invalid_argument("invalid expression");
            default:
                exp->putback(res.prev);
                return left;
        }
    }
}

int64_t expr(istream* exp) {
    int64_t left = term(exp);

    while (true) {
        Tokenizer res = token(exp);

        switch (res.t) {
            case Plus:
                left += term(exp);
                break;
            case Minus:
                left -= term(exp);
                break;
            case Error:
                throw invalid_argument("invalid expression");
            default:
                return left;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "error" << endl;
        return 1;
    }
    auto text = new std::istringstream(argv[1]);
    try{
        cout << expr(text) << endl;
    }

    catch (runtime_error& e) {
        cout << "error" << endl;
        return 1;
    }
    catch  (logic_error& e) {
        cout << "error" << endl;
        return 1;
    }
    return 0;
}
