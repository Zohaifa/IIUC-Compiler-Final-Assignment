#include <iostream>
#include <stack>
#include <string>
#include <sstream>
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

void generateTAC(string rpn) {
    stack<string> st;
    int tempCount = 1;
    
    stringstream ss(rpn);
    string token;
    
    cout << "\n3-address code:\n";
    
    while (ss >> token) {
        if (token.length() == 1 && (token[0] == '+' || token[0] == '-' || 
                                     token[0] == '*' || token[0] == '/')) {
            // Operator found
            string arg2 = st.top(); st.pop();
            string arg1 = st.top(); st.pop();
            string temp = "T" + to_string(tempCount++);
            
            cout << temp << " = " << arg1 << " " << token << " " << arg2 << endl;
            
            st.push(temp);
        }
        else {
            // Operand found
            st.push(token);
        }
    }
}

int main() {
    cout << "Enter expression: ";
    
    string expr;
    getline(cin, expr);
    
    string rpn = toRPN(expr);
    
    cout << "\nInfix: " << expr << endl;
    
    generateTAC(rpn);
    
    return 0;
}
