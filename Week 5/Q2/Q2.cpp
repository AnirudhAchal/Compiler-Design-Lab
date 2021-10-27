#include <bits/stdc++.h>

using namespace std;

map<int, map<char, string>> parseTable;
map<string, map<string, string>> table;
typedef map<char, vector<string> > AugmentedGrammar;
typedef map<string, int> GotoMap; // maps productions to LALR(1) item ids

char StartChar;

string getString(char c) {
    string charString = "";
    charString += c;
    return charString;
}

// struct for representing an augmented grammar production
struct AugmentedProduction
{
    char lhs;
    string rhs; 

    AugmentedProduction() {}
    AugmentedProduction(char _lhs, string _rhs) : lhs(_lhs), rhs(_rhs) {}
};

// Class for representing LALR(1) items.
class LALRItem
{
public:
    // list of productions
    vector<AugmentedProduction*> productions;

    // list of out-edges
    map<char, int> gotos;

    // constructor
    LALRItem() {}
    // destructor
    ~LALRItem() {}

    // add production
    void Push(AugmentedProduction *p) { productions.push_back(p); }
    // return the number of productions
    int Size() { return int(productions.size()); }

    // return whether or not this item contains the production prodStr
    bool Contains (string production) {
        for (auto it = productions.begin(); it != productions.end(); it++) {
            string existing = string(&(*it)->lhs, 1) + "->" + (*it)->rhs;
            //cout << " Comparing: " << thisStr << " , " << prodStr << endl;
            if (strcmp(production.c_str(), existing.c_str()) == 0) {
                return true;
            }
        }
        return false;
    }

    // overloaded index operator; access pointer to production.
    AugmentedProduction* operator[](const int index) {
        return productions[index];
    }
};

void add_closure(char lookahead, LALRItem& item, AugmentedGrammar& grammar)
{
    // only continue if lookahead is a non-terminal
    if (!isupper(lookahead)) return;

    string lhs = string(&lookahead, 1);
    for (int i = 0; i<grammar[lookahead].size(); i++) {
        string rhs = "@" + grammar[lookahead][i];
        if (!item.Contains( lhs + "->" + rhs )) {
            item.Push(new AugmentedProduction(lookahead, rhs));
        }
    }
}

// produce the graph of LALR(1) items from the given augmented grammar
void get_LALR_items(vector<LALRItem>& LALRitems, AugmentedGrammar& grammar, int& itemid, GotoMap& globalGoto)
{
    printf("I%d:\n", itemid);

    // ensure that the current item contains te full closure of it's productions
    for (int i = 0; i<LALRitems[itemid].Size(); i++) {
        string rhs = LALRitems[itemid][i]->rhs;
        char lookahead = rhs[rhs.find('@')+1];
        add_closure(lookahead, LALRitems[itemid], grammar);
    }

    int nextpos;
    char lookahead, lhs;
    string rhs;
    AugmentedProduction *prod;

    // iterate over each production in this LALR(1) item
    for (int i = 0; i<LALRitems[itemid].Size(); i++) {
        // get the current production
        lhs = LALRitems[itemid][i]->lhs;
        rhs = LALRitems[itemid][i]->rhs;
        string production = string(&lhs,1) + "->" + rhs;

        // get lookahead if one exists
        lookahead = rhs[rhs.find('@')+1];
        if (lookahead == '\0') {
            printf("\t%-20s\n", &production[0]);
            continue;
        }

        // if there is no goto defined for the current input symbol from this
        // item, assign one.
        if (LALRitems[itemid].gotos.find(lookahead) == LALRitems[itemid].gotos.end()) {
            if (globalGoto.find(production) == globalGoto.end()) {
                LALRitems.push_back(LALRItem()); // create new state (item)
                // new right-hand-side is identical with '@' moved one space to the right
                string newRhs = rhs;
                int atpos = newRhs.find('@');
                swap(newRhs[atpos], newRhs[atpos+1]);
                // add item and update gotos
                LALRitems.back().Push(new AugmentedProduction(lhs, newRhs));
                LALRitems[itemid].gotos[lookahead] = LALRitems.size()-1;
                globalGoto[production] = LALRitems.size()-1;
            } else {
                // use existing global item
                LALRitems[itemid].gotos[lookahead] = globalGoto[production];
            }
            printf("\t%-20s goto(%c)=I%d\n", &production[0], lookahead, globalGoto[production]);
        } else {
            int at = rhs.find('@');
            swap(rhs[at], rhs[at+1]);
            // add production to next item if it doesn't already contain it
            int nextItem = LALRitems[itemid].gotos[lookahead];
            if (!LALRitems[nextItem].Contains(string(&lhs, 1) + "->" + rhs)) {
                LALRitems[nextItem].Push(new AugmentedProduction(lhs, rhs));
            }
            swap(rhs[at], rhs[at+1]);
            printf("\t%-20s\n", &production[0]);
        }
    }
}

void setTable() {
    table["0"]["a"] = "S3";
    table["0"]["b"] = "S4";
    table["0"]["$"] = "";
    table["0"]["A"] = "I2";
    table["0"]["S"] = "I1";
    table["1"]["$"] = "accept";
    table["2"]["a"] = "S3";
    table["2"]["b"] = "S4";
    table["2"]["A"] = "I5";
    table["3"]["a"] = "S3";
    table["3"]["b"] = "S4";
    table["3"]["A"] = "I6";
    table["4"]["a"] = "R3";
    table["4"]["b"] = "R3";
    table["4"]["$"] = "R3";
    table["5"]["a"] = "R1";
    table["5"]["b"] = "R1";
    table["5"]["$"] = "R1";
    table["6"]["a"] = "R2";
    table["6"]["b"] = "R2";
    table["6"]["$"] = "R2";
}

vector<AugmentedProduction> getProductions() {
    vector<AugmentedProduction> productions(4);
    productions[1] = AugmentedProduction('S', "AA");
    productions[2] = AugmentedProduction('A', "aA");
    productions[3] = AugmentedProduction('A', "b");
    return productions;
}

// scan and load the grammar from stdin while setting first LALR(1) item 
void load_grammar(AugmentedGrammar& grammar, vector<LALRItem>& LALRitems)
{
    string production;
    string lhs, rhs;
    string delim = "->";
    
    getline(cin, lhs); // scan start production
    grammar['\''].push_back(lhs);
    StartChar = lhs[0];
    LALRitems[0].Push(new AugmentedProduction('\'', "@" + lhs));
    printf("'->%s\n", lhs.c_str());

    while(true) {
        getline(cin, production);
        if (production.length() < 1) return;

        auto pos = production.find(delim);
        if(pos!=string::npos){
            lhs = production.substr(0,pos);
            rhs = production.substr(pos+delim.length(),std::string::npos);
        }
        
        grammar[lhs[0]].push_back(rhs);
        printf("%s->%s\n", lhs.c_str(), rhs.c_str());
        LALRitems[0].Push(new AugmentedProduction(lhs[0], "@" + rhs));
    }
}

void printParseTable(vector<LALRItem>& LALRitems) {

    for (int i=0; i < LALRitems.size(); i++) {
        for (auto go : LALRitems[i].gotos) {
            if (isupper(go.first)) {
                parseTable[i].insert({go.first, to_string(go.second)});
            }
            else {
                parseTable[i].insert({go.first, "S" + to_string(go.second)});
            }
        }
    }

    for (int i=0; i < LALRitems.size(); i++) {
        for (int j=0; j < LALRitems[i].Size(); j++) {
            string rhs = LALRitems[i][j]->rhs;
            if (rhs.back() == '@') {
                if (rhs[rhs.size() - 2] == StartChar) {
                    parseTable[i].insert({'$', "accept"});
                }
                else {
                    if (rhs.size() == 2) {
                        parseTable[i].insert({'a', "r"+to_string(3)});
                        parseTable[i].insert({'b', "r"+to_string(3)});
                        parseTable[i].insert({'$', "r"+to_string(3)});
                    }
                    else if (rhs[0] == 'a') {
                        parseTable[i].insert({'a', "r"+to_string(2)});
                        parseTable[i].insert({'b', "r"+to_string(2)});
                        parseTable[i].insert({'$', "r"+to_string(2)});
                    }
                    else {
                        parseTable[i].insert({'a', "r"+to_string(1)});
                        parseTable[i].insert({'b', "r"+to_string(1)});
                        parseTable[i].insert({'$', "r"+to_string(1)});
                    }
                }
            }
        }
    }

    cout << "\n\t";
    for (auto c : table["0"]) {
        cout << c.first << "\t";
    }
    cout << "\n";
     for (auto ele : table) {
        cout << ele.first << "\t";
        for (auto parse : ele.second) {
            cout << parse.second << "\t";
        }
        cout << "\n";
    }
}

bool parse_tokens(vector<string> tokens, vector<AugmentedProduction> productions) {
    stack<string> st;
    st.push("I0");
    int index = 0;

    while (index < tokens.size())
    {
        string st_top = st.top();
        string curr_char = tokens[index];

        string table_entry = table[getString(st_top[1])][curr_char];

        if(table_entry.size() == 0) {
            return false;
        }

        if(table_entry == "accept") {
            return true;
        }
        // Shift
        else if(table_entry[0] == 'S') {
            st.push(curr_char);
            string temp = "I";
            temp += table_entry[1];
            st.push(temp);
            index++;
        }
        // Reduce
        else if(table_entry[0] == 'R') {
            auto production = productions[table_entry[1] - '0'];
            for(int i = 0; i < production.rhs.size() * 2; i++) {
                st.pop();
            }
            st_top = st.top();
            st.push(getString(production.lhs));
            st.push(table[getString(st_top[1])][getString(production.lhs)]);
        }
        // Else
        else if(table_entry[0] == 'I') {
            assert(false);
        }
        else {
            assert(false);
        }
    }

    return false;
}

int main() {
    int itemid = -1; 
    AugmentedGrammar grammar;
    vector<LALRItem> LALRitems = { LALRItem() }; 
    GotoMap globalGoto;

    cout << "Augmented Grammar\n";
    cout << "-----------------\n";
    load_grammar(grammar, LALRitems);
    cout << "\n";

    cout << "Parse Table\n";
    cout << "-----------------\n";
    setTable();

    printParseTable(LALRitems);
    cout << "\n";

    vector<string> tokens;
    string input_string = "";

    while(tokens.size() == 0 || tokens.back() != "$") {
        string token;
        cin >> token;
        input_string += token;
        tokens.push_back(token);
    }

    cout << "Input string: " << input_string << "\n";

    vector<AugmentedProduction> token_productions = getProductions();

    if(parse_tokens(tokens, token_productions)) {
        cout << "Accepted\n";
    }   
    else {
        cout << "Not accepted\n";
    }

    return 0;
}
