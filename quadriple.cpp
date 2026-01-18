#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Quad {
    string op;
    string arg1;
    string arg2;
    string result;
};

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

vector<Quad> generateQuadruples(string rpn) {
    vector<Quad> quads;
    stack<string> st;
    int tempCount = 1;
    
    stringstream ss(rpn);
    string token;
    
    while (ss >> token) {
        if (token.length() == 1 && (token[0] == '+' || token[0] == '-' || 
                                     token[0] == '*' || token[0] == '/')) {
            // Operator found
            string arg2 = st.top(); st.pop();
            string arg1 = st.top(); st.pop();
            string temp = "T" + to_string(tempCount++);
            
            Quad q;
            q.op = token;
            q.arg1 = arg1;
            q.arg2 = arg2;
            q.result = temp;
            
            quads.push_back(q);
            st.push(temp);
        }
        else {
            // Operand found
            st.push(token);
        }
    }
    
    return quads;
}

void printQuadruples(vector<Quad>& quads) {
    cout << "\n========== QUADRUPLES TABLE ==========\n\n";
    cout << "Operator\tArgument1\tArgument2\tResult\n";
    cout << "--------\t---------\t---------\t------\n";
    
    for (auto& q : quads) {
        cout << q.op << "\t\t" << q.arg1 << "\t\t" << q.arg2 << "\t\t" << q.result << "\n";
    }
}

int main() {
    cout << "=== Quadruples Table Generator ===\n\n";
    cout << "Enter expression: ";
    
    string expr;
    getline(cin, expr);
    
    string rpn = toRPN(expr);
    vector<Quad> quads = generateQuadruples(rpn);
    
    cout << "\nOptimized 3-address code:\n";
    for (auto& q : quads) {
        cout << q.result << " = " << q.arg1 << " " << q.op << " " << q.arg2 << "\n";
    }
    
    printQuadruples(quads);
    
    return 0;
}