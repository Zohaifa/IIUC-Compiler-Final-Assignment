#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
using namespace std;

struct Item {
    string lhs;
    string rhs;
    int dot;

    bool operator<(const Item& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        return dot < other.dot;
    }

    bool operator==(const Item& other) const {
        return lhs == other.lhs &&
               rhs == other.rhs &&
               dot == other.dot;
    }
};

struct ProductionRule {
    int id;
    string lhs;
    string rhs;
};

map<string, vector<string>> grammar;
vector<ProductionRule> productions;
vector<set<Item>> states;
map<pair<int, string>, int> transitions;
set<string> terminals;
set<string> nonTerminals;
string startSymbol;

// closure
set<Item> closure(set<Item> I) {
    bool changed = true;
    while (changed) {
        changed = false;
        set<Item> newItems = I;

        for (auto item : I) {
            if (item.dot < (int)item.rhs.size()) {
                string B = "";
                if (isupper(item.rhs[item.dot])) {
                    B = string(1, item.rhs[item.dot]);
                    if (grammar.count(B)) {
                        for (string prod : grammar[B]) {
                            Item newItem{B, prod, 0};
                            if (!newItems.count(newItem)) {
                                newItems.insert(newItem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        I = newItems;
    }
    return I;
}

// goto
set<Item> goTo(set<Item> I, string X) {
    set<Item> J;
    for (auto item : I) {
        if (item.dot < (int)item.rhs.size()) {
            string symbol = "";
            if (isupper(item.rhs[item.dot])) {
                symbol = string(1, item.rhs[item.dot]);
            } else {
                symbol = string(1, item.rhs[item.dot]);
            }
            if (symbol == X) {
                J.insert({item.lhs, item.rhs, item.dot + 1});
            }
        }
    }
    return closure(J);
}

// Build DFA
void buildDFA() {
    // I0 - create augmented grammar
    grammar["S'"].push_back(startSymbol);
    
    Item start = {"S'", grammar["S'"][0], 0};
    set<Item> I0;
    I0.insert(start);
    states.push_back(closure(I0));

    // build states
    for (int i = 0; i < (int)states.size(); i++) {
        set<string> symbols;

        for (auto item : states[i]) {
            if (item.dot < (int)item.rhs.size()) {
                if (isupper(item.rhs[item.dot])) {
                    symbols.insert(string(1, item.rhs[item.dot]));
                } else {
                    symbols.insert(string(1, item.rhs[item.dot]));
                }
            }
        }

        for (string X : symbols) {
            set<Item> J = goTo(states[i], X);
            if (J.empty()) continue;

            int k = -1;
            bool found = false;
            for (k = 0; k < (int)states.size(); k++) {
                if (states[k] == J) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                states.push_back(J);
                k = states.size() - 1;
            }
            transitions[{i, X}] = k;
        }
    }
}

// Build parsing tables
void buildParsingTables() {
    map<int, map<string, string>> ACTION;
    map<int, map<string, int>> GOTO;

    // For each state
    for (int i = 0; i < (int)states.size(); i++) {
        // For each item in state i
        for (auto item : states[i]) {
            if (item.dot < (int)item.rhs.size()) {
                // Item is not complete
                string symbol = string(1, item.rhs[item.dot]);
                
                // Check if we can go to next state
                if (transitions.count({i, symbol})) {
                    int next_state = transitions[{i, symbol}];
                    
                    if (isupper(item.rhs[item.dot])) {
                        // Non-terminal - GOTO entry
                        GOTO[i][symbol] = next_state;
                    } else {
                        // Terminal - ACTION shift entry
                        ACTION[i][symbol] = "s" + to_string(next_state);
                    }
                }
            } else {
                // Item is complete (reduce item)
                if (item.lhs == "S'" && item.rhs == startSymbol) {
                    // Accept item
                    ACTION[i]["$"] = "acc";
                } else {
                    // Find production number for this reduce action
                    int prodId = -1;
                    for (int j = 0; j < (int)productions.size(); j++) {
                        if (productions[j].lhs == item.lhs && productions[j].rhs == item.rhs) {
                            prodId = j;
                            break;
                        }
                    }
                    
                    // Add reduce action for all terminals
                    for (string term : terminals) {
                        ACTION[i][term] = "r" + to_string(prodId);
                    }
                    ACTION[i]["$"] = "r" + to_string(prodId);
                }
            }
        }
    }

    // Print ACTION table
    cout << "\n========== ACTION TABLE ==========\n";
    cout << "State\t";
    for (string term : terminals) {
        cout << term << "\t";
    }
    cout << "$\n";
    
    for (int i = 0; i < (int)states.size(); i++) {
        cout << i << "\t";
        for (string term : terminals) {
            if (ACTION[i].count(term)) {
                cout << ACTION[i][term] << "\t";
            } else {
                cout << "-\t";
            }
        }
        if (ACTION[i].count("$")) {
            cout << ACTION[i]["$"];
        } else {
            cout << "-";
        }
        cout << "\n";
    }

    // Print GOTO table
    cout << "\n========== GOTO TABLE ==========\n";
    cout << "State\t";
    for (string nt : nonTerminals) {
        if (nt != "S'") cout << nt << "\t";
    }
    cout << "\n";
    
    for (int i = 0; i < (int)states.size(); i++) {
        cout << i << "\t";
        for (string nt : nonTerminals) {
            if (nt != "S'") {
                if (GOTO[i].count(nt)) {
                    cout << GOTO[i][nt] << "\t";
                } else {
                    cout << "-\t";
                }
            }
        }
        cout << "\n";
    }
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter grammar (format: A -> a | b):\n";
    
    for (int i = 0; i < n; i++) {
        string s;
        getline(cin, s);

        // Parse LHS
        int arrowPos = s.find("->");
        string lhs = s.substr(0, arrowPos);
        
        // Remove leading/trailing spaces
        lhs.erase(0, lhs.find_first_not_of(" "));
        lhs.erase(lhs.find_last_not_of(" ") + 1);

        nonTerminals.insert(lhs);
        
        if (startSymbol.empty()) {
            startSymbol = lhs;
        }

        // Parse RHS
        string rhs_part = s.substr(arrowPos + 2);
        
        string temp = "";
        for (char c : rhs_part) {
            if (c == '|') {
                temp.erase(0, temp.find_first_not_of(" "));
                temp.erase(temp.find_last_not_of(" ") + 1);
                
                grammar[lhs].push_back(temp);
                ProductionRule pr;
                pr.id = productions.size();
                pr.lhs = lhs;
                pr.rhs = temp;
                productions.push_back(pr);
                
                // Extract terminals
                for (char ch : temp) {
                    if (ch != ' ' && !isupper(ch)) {
                        terminals.insert(string(1, ch));
                    }
                }
                
                temp.clear();
            } else {
                temp += c;
            }
        }
        
        temp.erase(0, temp.find_first_not_of(" "));
        temp.erase(temp.find_last_not_of(" ") + 1);
        
        grammar[lhs].push_back(temp);
        ProductionRule pr;
        pr.id = productions.size();
        pr.lhs = lhs;
        pr.rhs = temp;
        productions.push_back(pr);
        
        // Extract terminals
        for (char ch : temp) {
            if (ch != ' ' && !isupper(ch)) {
                terminals.insert(string(1, ch));
            }
        }
    }

    // Build DFA from canonical LR(0) items
    buildDFA();

    // Print canonical LR(0) items (DFA states)
    cout << "\n========== CANONICAL LR(0) ITEMS (DFA) ==========\n";
    for (int i = 0; i < (int)states.size(); i++) {
        cout << "\nI" << i << ":\n";
        for (auto item : states[i]) {
            cout << "  " << item.lhs << " -> ";
            for (int j = 0; j < (int)item.rhs.size(); j++) {
                if (j == item.dot) cout << ".";
                cout << item.rhs[j];
            }
            if (item.dot == (int)item.rhs.size()) cout << ".";
            cout << "\n";
        }
    }

    // Build and print parsing tables
    buildParsingTables();

    return 0;
}
