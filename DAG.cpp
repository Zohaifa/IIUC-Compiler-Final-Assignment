#include <bits/stdc++.h>
using namespace std;

// Node in DAG
struct Node {
    string op;          
    string value;       
    Node* left = nullptr;
    Node* right = nullptr;
};

// Map to reuse common subexpressions
map<string, Node*> nodeMap;

// Function to create/reuse a DAG node
Node* getNode(string op, string leftVal, string rightVal) {
    string key = op + "(" + leftVal + "," + rightVal + ")";
    if (nodeMap.count(key)) return nodeMap[key]; 
    Node* n = new Node();
    n->op = op;
    n->left = new Node{leftVal};
    n->right = new Node{rightVal};
    nodeMap[key] = n;
    return n;
}

// Inorder print of DAG (for visualization)
void print(Node* n) {
    if (!n) return;
    if (n->op == "") { cout << n->value; return; }
    cout << "(";
    print(n->left);
    cout << " " << n->op << " ";
    print(n->right);
    cout << ")";
}

int main() {
    // Example expressions:
    // a = b + c
    // d = b + c + e

    Node* expr1 = getNode("+", "b", "c");         // b+c
    Node* a = expr1;                              // a = b+c

    Node* expr2 = getNode("+", "b", "c");         // reused b+c
    Node* d = getNode("+", expr2->value, "e");    // (b+c)+e

    cout << "a = "; print(a); cout << endl;
    cout << "d = "; print(d); cout << endl;

    return 0;
}
