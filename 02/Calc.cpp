#include <iostream>
#include <sstream>
// #include <vector>
// #include <algorithm>
#include <string>
#include <exception>

using namespace std;

enum Token {
    Number,
    End,
    Plus,
    Minus,
    Mul,
    Div
};

struct Result {
    int64_t number = 0;
    char prev;
    Token t;

    Result(Token y, char prev = 0, int64_t x = 0) : number(x), prev(prev), t(y) {}
};

Result token(istream* exp) {
    char c;
    while (exp->get(c) && isspace(c));

    switch (c) {
        case 0:
            return Result(End);
        case '-':
            return Result(Minus, '-');
        case '+':
            return Result(Plus, '+');
        case '*':
            return Result(Mul, '*');
        case '/':
            return Result(Div, '/');
        default:
            if (c >= '0' && c <= '9') {
                return Result(Number, c, c - '0');
            }
    }
}

int64_t prim(istream* exp) {
    Result res = token(exp);

    switch (res.t) {
        case Minus:
            return -prim(exp);
        case Plus:
            return prim(exp);
        case End:
            throw ("something");
        case Number:
            return res.number;
        default:
            throw ("something");
    }
}

int64_t term(istream* exp) {
    int64_t left = prim(exp);

    while (true) {
        Result res = token(exp);

        switch(res.t) {
            case Mul:
                left *= prim(exp);
                break;
            case Div:
                left /= prim(exp);
                break;
            default:
                exp->putback(res.prev);
                return left;
        }
    }
}

int64_t expr(istream* exp) {
    int64_t left = term(exp);

    while (true) {
        Result res = token(exp);

        switch (res.t) {
            case Plus:
                left += term(exp);
                break;
            case Minus:
                left -= term(exp);
                break;
            default:
                return left;
        }
    }
}

int main(int argc, char* argv[]) {
    std::istream* text = nullptr;
    text = new std::istringstream(argv[1]);
    cout << expr(text) << endl;
    return 0;
}
