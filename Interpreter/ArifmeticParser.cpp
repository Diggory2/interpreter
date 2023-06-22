#include "ArifmeticParser.h"
#include "Atoi/Atoi.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
/*
Вещественные числа
Операции с []
call
*/

std::string ArifParser::parse_token() {
    while (std::isspace(*input)) ++input;

    if (std::isdigit(*input)) {
        std::string number;
        while (std::isdigit(*input) || *input == '.' ||
            (*input >= 'A' && *input <='F')
            || *input == 'x' || *input == 'b')
            number.push_back(*input++);
        return number;
    }

    static const std::string tokens[] =
    { "+", "-", "*", "/", "(", ")" };
    for (auto& t : tokens) {
        if (std::strncmp(input, t.c_str(), t.size()) == 0) {
            input += t.size();
            return t;
        }
    }

    return "";
}

Expression ArifParser::parse_simple_expression() {
    auto token = parse_token();
    if (token.empty()) throw std::runtime_error("Invalid input");

    if (token == "(") {
        auto result = parse();
        if (parse_token() != ")") throw std::runtime_error("Expected ')'");
        return result;
    }

    if (std::isdigit(token[0]))
        return Expression(token);

    return Expression(token, parse_simple_expression());
}

int get_priority(const std::string& binary_op) {
    if (binary_op == "+") return 1;
    if (binary_op == "-") return 1;
    if (binary_op == "*") return 2;
    if (binary_op == "/") return 2;

    return 0;
}

Expression ArifParser::parse_binary_expression(int min_priority) {
    auto left_expr = parse_simple_expression();

    for (;;) {
        auto op = parse_token();
        auto priority = get_priority(op);
        if (priority <= min_priority) {
            input -= op.size();
            return left_expr;
        }

        auto right_expr = parse_binary_expression(priority);
        left_expr = Expression(op, left_expr, right_expr);
    }
}

double ArifParser::Result(const char* input)
{
    this->input = input;
    return eval(parse());
}
Expression ArifParser::parse() {
    return parse_binary_expression(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double ArifParser::eval(const Expression& e) {
    switch (e.args.size()) {
    case 2: {
        auto a = eval(e.args[0]);
        auto b = eval(e.args[1]);
        if (e.token == "+") return a + b;
        if (e.token == "-") return a - b;
        if (e.token == "*") return a * b;
        if (e.token == "/") return a / b;
        throw std::runtime_error("Unknown binary operator");
    }

    case 1: {
        auto a = eval(e.args[0]);
        if (e.token == "+") return +a;
        if (e.token == "-") return -a;

        throw std::runtime_error("Unknown unary operator");
    }

    case 0:
        int res;
        Atoi at;
        if (at.ParseNumber(e.token, res))
            return (double)res;
        return strtod(e.token.c_str(), nullptr);// Parse
    }

    throw std::runtime_error("Unknown expression type");
}
