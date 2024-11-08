#include <iostream>
#include <stack>
#include <string>
#include <cmath>

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

bool isValidExpression(const std::string& expression) {
    std::stack<char> parentheses;
    bool previousWasOperator = false; // متغیری برای ذخیره وضعیت قبلی عملگرها
    for (char c : expression) {
        if (c == '(') {
            parentheses.push(c);
            previousWasOperator = false; // پرانتزها انتهای یک عبارت عملگری را مشخص می‌کنند
        } else if (c == ')') {
            if (parentheses.empty() || parentheses.top() != '(')
                return false;
            parentheses.pop();
            previousWasOperator = false; // پرانتزها انتهای یک عبارت عملگری را مشخص می‌کنند
        } else if (isOperator(c)) {
            if (previousWasOperator) // اگر قبلی نیز عملگر بوده باشد، عبارت نامعتبر است
                return false;
            previousWasOperator = true;
        } else {
            previousWasOperator = false; // اگر کاراکتری به جز پرانتزها یا عملگرها بود، این اشکال ندارد
        }
    }
    return parentheses.empty();
}

double evaluateExpression(const std::string& expression) {
    std::stack<double> values;
    std::stack<char> operators;

    for (size_t i = 0; i < expression.length(); i++) {
        if (expression[i] == ' ')
            continue;
        else if (isdigit(expression[i])) {
            std::string operand;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.'))
                operand += expression[i++];
            i--;
            values.push(std::stod(operand));
        } else if (expression[i] == '(') {
            operators.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.pop(); // Remove '('
        } else if (isOperator(expression[i])) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(expression[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(expression[i]);
        }
    }

    while (!operators.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}

int main() {
    std::string expression;
    std::cout << "Enter an expression:";
    std::getline(std::cin, expression);

    if (!isValidExpression(expression)) {
        std::cout << "Invalid expression! Parentheses mismatch or consecutive operators." << std::endl;
        return 1;
    }

    double result = evaluateExpression(expression);
    std::cout << "Result: " << result << std::endl;

    return 0;
}