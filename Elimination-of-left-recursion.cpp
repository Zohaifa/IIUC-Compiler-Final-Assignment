#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

map<string, vector<string>> grammar;
map<string, vector<string>> newGrammar;

bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter grammar (# for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        string lhs = s.substr(0, 1);
        string rhs = s.substr(3);

        string temp = "";
        for (char c : rhs) {
            if (c == '|') {
                grammar[lhs].push_back(temp);
                temp = "";
            } else {
                temp += c;
            }
        }
        grammar[lhs].push_back(temp);
    }

    // Eliminate direct left recursion
    for (auto g : grammar) {
        string A = g.first;
        vector<string> alpha, beta;

        for (string prod : g.second) {
            if (prod[0] == A[0])
                alpha.push_back(prod.substr(1));
            else
                beta.push_back(prod);
        }

        if (alpha.empty()) {
            newGrammar[A] = grammar[A];
        } else {
            string Aprime = A + "'";

            // A -> βA'
            for (string b : beta)
                newGrammar[A].push_back(b + Aprime);

            // A' -> αA' | #
            for (string a : alpha)
                newGrammar[Aprime].push_back(a + Aprime);

            newGrammar[Aprime].push_back("#");
        }
    }

    // Output grammar
    cout << "\nGrammar after eliminating left recursion:\n";
    for (auto g : newGrammar) {
        cout << g.first << " -> ";
        for (int i = 0; i < g.second.size(); i++) {
            cout << g.second[i];
            if (i != g.second.size() - 1)
                cout << " | ";
        }
        cout << endl;
    }

    return 0;
}
