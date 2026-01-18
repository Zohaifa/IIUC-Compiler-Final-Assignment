#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
using namespace std;

map<string, vector<string>> grammar;
map<string, set<char>> FIRST, FOLLOW;
map<string, map<char, string>> parseTable;

// FIRST function
void findFirst(string c) {
    for (string prod : grammar[c]) {
        if (!isupper(prod[0])) {
            FIRST[c].insert(prod[0]);
        } else {
            string first_char = string(1, prod[0]);
            if (FIRST[first_char].empty())
                findFirst(first_char);

            for (char x : FIRST[first_char])
                FIRST[c].insert(x);
        }
    }
}

// FOLLOW function
void findFollow(string c) {
    for (auto g : grammar) {
        string lhs = g.first;
        for (string prod : g.second) {
            for (int i = 0; i < prod.size(); i++) {
                if (prod[i] == c[0]) {
                    if (i + 1 < prod.size()) {
                        char next = prod[i + 1];
                        string next_str = string(1, next);
                        if (!isupper(next))
                            FOLLOW[c].insert(next);
                        else {
                            for (char x : FIRST[next_str])
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


// FIRST of RHS string
set<char> firstOfString(const string& s) {
    set<char> result;

    for (char c : s) {
        if (!isupper(c)) {
            result.insert(c);
            return result;
        }

        for (char x : FIRST[string(1, c)]) {
            if (x != '#')
                result.insert(x);
        }

        if (FIRST[string(1, c)].count('#') == 0)
            return result;
    }

    result.insert('#');
    return result;
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter grammar (# for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        string lhs = s.substr(0, s.find("->"));
        string rhs = s.substr(s.find("->") + 2);

        string temp = "";
        for (char c : rhs) {
            if (c == '|') {
                grammar[lhs].push_back(temp);
                temp.clear();
            } else {
                temp += c;
            }
        }
        grammar[lhs].push_back(temp);
    }

    // Extract non-terminals and start symbol
    set<string> nonTerminals;
    string startSymbol;
    for (auto g : grammar) {
        nonTerminals.insert(g.first);
        if (startSymbol.empty())
            startSymbol = g.first;
    }

    // FIRST
    for (string nt : nonTerminals)
        findFirst(nt);

    // FOLLOW
    FOLLOW[startSymbol].insert('$');
    for (string nt : nonTerminals)
        findFollow(nt);

    // Build LL(1) table
    bool isLL1 = true;

    for (auto g : grammar) {
        string A = g.first;

        for (string alpha : g.second) {
            set<char> firstAlpha = firstOfString(alpha);

            for (char t : firstAlpha) {
                if (t != '#') {
                    if (parseTable[A][t] != "") {
                        isLL1 = false;
                    }
                    parseTable[A][t] = alpha;
                }
            }

            if (firstAlpha.count('#')) {
                for (char b : FOLLOW[A]) {
                    if (parseTable[A][b] != "") {
                        isLL1 = false;
                    }
                    parseTable[A][b] = "#";
                }
            }
        }
    }

    // Output table
    cout << "\nLL(1) Parsing Table:\n";
    for (auto row : parseTable) {
        for (auto cell : row.second) {
            cout << "M[" << row.first << "," << cell.first << "] = "
                 << row.first << " -> " << cell.second << endl;
        }
    }

    if (isLL1)
        cout << "\nGrammar is LL(1)\n";
    else
        cout << "\nGrammar is NOT LL(1)\n";

    return 0;
}
