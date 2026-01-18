#include <iostream>
#include <stack>
#include <string>
using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string toRPN(string expr) {
    string output = "";
    stack<char> operators;
    
    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        // Operand
        if (isalnum(c)) {
            output += c;
            output += " ";
        }
        // Operator
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!operators.empty() && operators.top() != '(' && 
                   precedence(operators.top()) >= precedence(c)) {
                output += operators.top();
                output += " ";
                operators.pop();
            }
            operators.push(c);
        }
        // Left parenthesis
        else if (c == '(') {
            operators.push(c);
        }
        // Right parenthesis
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                output += operators.top();
                output += " ";
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove '('
        }
    }
    
    // Pop remaining operators
    while (!operators.empty()) {
        output += operators.top();
        output += " ";
        operators.pop();
    }
    
    return output;
}

int main() {
    cout << "Enter infix expression:";
    
    string expr;
    getline(cin, expr);
    
    string rpn = toRPN(expr);
    
    cout << "\nInfix:  " << expr << endl;
    cout << "RPN:    " << rpn << endl;
    
    return 0;
}
