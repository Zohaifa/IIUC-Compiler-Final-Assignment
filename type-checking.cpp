#include <bits/stdc++.h>
using namespace std;

map<string, string> symTable;

// Determine type of a token
string getType(string token) {
    // Number
    if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
        return (token.find('.') != string::npos) ? "float" : "int";
    }

    // Variable
    if (isalpha(token[0])) {
        if (symTable.count(token) == 0)
            return "error";
        return symTable[token];
    }

    return "error";
}

// Check expression type
string checkExpr(string expr) {
    // Remove leading/trailing spaces
    expr.erase(0, expr.find_first_not_of(" "));
    expr.erase(expr.find_last_not_of(" ") + 1);
    
    string resultType = "";
    stringstream ss(expr);
    string token, op;
    
    // Get first token's type
    ss >> token;
    resultType = getType(token);
    
    if (resultType == "error")
        return "error";
    
    // Process remaining tokens
    while (ss >> op >> token) {
        string tokenType = getType(token);
        
        if (tokenType == "error")
            return "error";
        
        // Type promotion: if either is float, result is float
        if (resultType == "float" || tokenType == "float")
            resultType = "float";
    }
    
    return resultType;
}

int main() {
    cout << "Enter statements (end with q):\n";

    string line;
    while (getline(cin, line)) {
        if (line == "q") break;

        stringstream ss(line);
        string first;
        ss >> first;

        // Declaration
        if (first == "int" || first == "float") {
            string var;
            ss >> var;
            symTable[var] = first;
            cout << "Declared: " << var << " as " << first << endl;
            continue;
        }

        // Assignment
        string lhs = first;
        if (symTable.count(lhs) == 0) {
            cout << "Error: Undeclared variable " << lhs << endl;
            continue;
        }

        string eq, rest;
        ss >> eq;
        getline(ss, rest);

        string rhsType = checkExpr(rest);
        if (rhsType == "error") {
            cout << "Error: Invalid expression\n";
            continue;
        }

        if (symTable[lhs] != rhsType) {
            cout << "Type Error: Cannot assign " << rhsType
                 << " to " << symTable[lhs] << endl;
        } else {
            cout << "Assignment successful: " << lhs << " = " << rhsType << endl;
        }
    }

    return 0;
}