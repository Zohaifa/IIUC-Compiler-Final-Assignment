#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> FIRST, FOLLOW;
set<char> nonTerminals;
char startSymbol;

// FIRST function
void findFirst(char c) {
    for (string prod : grammar[c]) {
        if (!isupper(prod[0])) {
            FIRST[c].insert(prod[0]);
        } else {
            if (FIRST[prod[0]].empty())
                findFirst(prod[0]);

            for (char x : FIRST[prod[0]])
                FIRST[c].insert(x);
        }
    }
}

// FOLLOW function
void findFollow(char c) {
    for (auto g : grammar) {
        char lhs = g.first;
        for (string prod : g.second) {
            for (int i = 0; i < prod.size(); i++) {
                if (prod[i] == c) {
                    if (i + 1 < prod.size()) {
                        char next = prod[i + 1];
                        if (!isupper(next))
                            FOLLOW[c].insert(next);
                        else {
                            for (char x : FIRST[next])
                                if (x != '#')
                                    FOLLOW[c].insert(x);
                        }
                    } else {
                        if (c != lhs)
                            for (char x : FOLLOW[lhs])
                                FOLLOW[c].insert(x);
                    }
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter grammar (use # for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        char lhs = s[0];
        nonTerminals.insert(lhs);
        if (i == 0) startSymbol = lhs;

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

    // FIRST
    for (char nt : nonTerminals)
        findFirst(nt);

    // FOLLOW
    FOLLOW[startSymbol].insert('$');
    for (char nt : nonTerminals)
        findFollow(nt);

    // Output FIRST
    cout << "\nFIRST sets:\n";
    for (char nt : nonTerminals) {
        cout << "FIRST(" << nt << ") = { ";
        for (char x : FIRST[nt])
            cout << x << " ";
        cout << "}\n";
    }

    // Output FOLLOW
    cout << "\nFOLLOW sets:\n";
    for (char nt : nonTerminals) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (char x : FOLLOW[nt])
            cout << x << " ";
        cout << "}\n";
    }

    return 0;
}
