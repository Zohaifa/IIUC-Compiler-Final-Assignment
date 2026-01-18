#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

map<string, vector<string>> grammar;
map<string, vector<string>> newGrammar;

// helper
string commonPrefix(const string& a, const string& b) {
    int i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i])
        i++;
    return a.substr(0, i);
}

// find longest common prefix of all strings
string longestCommonPrefix(const vector<string>& v) {
    if (v.empty()) return "";

    string prefix = v[0];
    for (int i = 1; i < v.size(); i++) {
        prefix = commonPrefix(prefix, v[i]);
        if (prefix.empty()) break;
    }
    return prefix;
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
                temp.clear();
            } else {
                temp += c;
            }
        }
        grammar[lhs].push_back(temp);
    }

    // LEFT FACTORING
    for (auto g : grammar) {
        string A = g.first;
        vector<string> prods = g.second;

        string prefix = longestCommonPrefix(prods);

        if (prefix.empty() || prods.size() == 1) {
            newGrammar[A] = prods;
            continue;
        }

        string Aprime = A + "'";

        // A -> prefix A'
        newGrammar[A].push_back(prefix + Aprime);

        // A' productions
        for (string p : prods) {
            string rest = p.substr(prefix.size());
            if (rest.empty())
                newGrammar[Aprime].push_back("#");
            else
                newGrammar[Aprime].push_back(rest);
        }
    }

    // Output
    cout << "\nGrammar after left factoring:\n";
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
