#include<bits/stdc++.h>
#define EPSILON '#'
#define TERMINAL '$'

using namespace std;

// Delimiter pattern used in the grammar
const string delim = "->";

// The starting character of the grammar
char startChar;

// Map to store the grammar
unordered_map<char, vector<string>> grammar;

// Map to store the first sets for all non terminals
unordered_map<char,set<char>> firstMap;

// Map to store the follow sets for all non terminals
unordered_map<char,set<char>> followMap;

// Map to store states
unordered_map<int, unordered_map<char, set<pair<deque<char>, deque<char>>>>> states;

// Map to store the DFA transitions
unordered_map<int,set<pair<int,char>>> gotos;

// Map to store the production rules
map<pair<char,deque<char>>, int> prodRules;

// Map to store the rule numbers and the associated details (reverse map of prodRules)
unordered_map<int, pair<char, int>> ruleNumbers;

// Sets to store the symbols in the grammar
set<char> terminals, nonTerminals;

// Vector to store all symbols in the grammar
vector<char> symbols;

// Helper function to find the index of a given element in a vector
int findIdx(char key) {
    if (count(symbols.begin(), symbols.end(), key))
        return distance(symbols.begin(), find(symbols.begin(), symbols.end(), key));
    else
        return -1;
}

// Function to input the grammar from the given file
void inputGrammar(string filename) {
    ifstream GrammarFile(filename);
    string fileLine;
    bool foundFirstChar = false;

    while(getline(GrammarFile, fileLine)) {
        if(!foundFirstChar) {
            startChar = fileLine[0];
            foundFirstChar = true;
        }

        string temp;
        char lhs = fileLine[0];
        for(int i=delim.size() + 1; i < fileLine.size(); i++) {
            if(fileLine[i] == '|') {
                grammar[lhs].push_back(temp);
                temp.clear();
            }
            else {
                temp.push_back(fileLine[i]);
            }
        }
        grammar[lhs].push_back(temp);
    }

    GrammarFile.close();
}

// Helper recursive function to find the First set of the grammar
bool findFirstSetHelper(char lhs, char originalLhs, char last, set<char>& firstSet) {
    bool addNextCharToSet = false;

    for(auto rhs : grammar[lhs]) {
        bool addCharToSet = true;
        for(auto ch : rhs){
            if(ch == lhs)
                break;

            if(!addCharToSet)
                break;

            if(!isupper(ch) && ch != EPSILON) {
                firstSet.insert(ch);
                break;
            }

            else if(ch == EPSILON) {
                if(originalLhs == lhs || lhs == last)
                    firstSet.insert(ch);
                addNextCharToSet = true;
                break;
            }

            else {
                addCharToSet = findFirstSetHelper(ch, originalLhs, rhs.back(), firstSet);
                addNextCharToSet |= addCharToSet;
            }
        }
    }

    return addNextCharToSet;
}

// Function to find the First set of the grammar
void findFirstSet() {
    set<char> firstSet;

    for(auto rule : grammar) {
        firstSet.clear();
        findFirstSetHelper(rule.first, rule.first, rule.first, firstSet);

        for(auto ch : firstSet)
            firstMap[rule.first].insert(ch);
    }
}

// Function to find the Follow set of the grammar
void findFollowSet() {
    int i;
    followMap[startChar].insert(TERMINAL);
    int count = 10;

    while(count--) {
        for(auto rule : grammar) {
            for(auto str : rule.second) {
                for(i=0; i < str.size()-1; i++) {
                    if(isupper(str[i])) {
                        if(!isupper(str[i+1])) {
                            followMap[str[i]].insert(str[i+1]);
                        }

                        else {
                            char temp = str[i+1];
                            int j = i+1;

                            while(isupper(temp)) {
                                if(firstMap[temp].find(EPSILON) != firstMap[temp].end()) {
                                    for(auto ch : firstMap[temp]) {
                                        if(ch == EPSILON)
                                            continue;
                                        followMap[str[i]].insert(ch);
                                    }

                                    j++;
                                    if(j < str.size()) {
                                        temp = str[j];
                                        if(!isupper(temp)) {
                                            followMap[str[i]].insert(temp);
                                            break;
                                        }
                                    }

                                    else {
                                        for(auto ch : followMap[rule.first])
                                            followMap[str[i]].insert(ch);
                                        break;
                                    }
                                }

                                else {
                                    for(auto ch : firstMap[temp]) {
                                        followMap[str[i]].insert(ch);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }

                if(isupper(str.back())) {
                    for(auto ch : followMap[rule.first])
                        followMap[str[i]].insert(ch);
                }
            }
        }
    }
}

// Helper recursive function to find the DFA States for the SLR Grammar
void findStatesHelper(char lhs, char nextChar, int last, pair<deque<char>,deque<char>> currRule, int& numStates) {
    unordered_map<char,set<pair<deque<char>,deque<char>>>> currState;
    int rep = -2;

    if(last != -1){
        for(auto ele : gotos[last]){
            if(ele.second == nextChar){
                rep = ele.first;
                currState = states[ele.first];
            }
        }
    }

    currState[lhs].insert(currRule);

    int count = 10;
    while(count--) {
        for(auto rules : currState) {
            for(auto rule : rules.second) {
                // If there are characters after the dot
                if(!rule.second.empty()) {
                    if(isupper(rule.second.front())) {
                        for(auto prodRule : grammar[rule.second.front()]) {
                            deque<char> afterDot, beforeDot;
                            for(auto ch : prodRule)
                                afterDot.push_back(ch);
                            currState[rule.second.front()].insert({beforeDot, afterDot});
                        }
                    }
                }
            }
        }
    }

    for(auto state : states) {
        if(state.second == currState) {
            gotos[last].insert({state.first, nextChar});
            return;
        }
    }

    if(rep == -2) {
        states[++numStates] = currState;
        if(last != -1)
            gotos[last].insert({numStates, nextChar});
    }

    else {
        states[rep] = currState;
    }

    int copy = numStates;
    for(auto rules : currState) {
        for(auto rule : rules.second) {
            if(!rule.second.empty()) {
                rule.first.push_back(rule.second.front());
                rule.second.pop_front();
                findStatesHelper(rules.first, rule.first.back(), copy, rule, numStates);
            }
        }
    }
}

// Function to find the DFA States for the SLR grammar
void findStates() {
    int numStates = -1;
    deque<char> beforeDot, afterDot;
    afterDot.push_back(startChar);
    findStatesHelper('!', '\0', -1, {beforeDot, afterDot}, numStates);
}

// Function to print productions and store the production rules
void printProductions() {
    cout<<"\nProductions: "<<'\n';
    int numRules = 1;

    for(auto rule : grammar) {
        nonTerminals.insert(rule.first);
        for(auto rhs : rule.second) {
            cout << "Rule " << numRules << ": ";
            string ruleStr = "";
            ruleStr += rule.first;
            ruleStr += delim;

            deque<char> temp;
            for(auto ch : rhs) {
                ruleStr += ch;
                temp.push_back(ch);
                if(isupper(ch))
                    nonTerminals.insert(ch);
                else
                    terminals.insert(ch);
            }

            prodRules[{rule.first,temp}] = numRules;
            ruleNumbers[numRules] = {rule.first, temp.size()};
            cout << ruleStr << '\n';
            numRules++;
        }
    }

    terminals.insert(TERMINAL);
    for (auto ch : terminals)
        symbols.push_back(ch);
    for (auto ch : nonTerminals)
        symbols.push_back(ch);
}

// Function to print the DFA Graph
void printGraph() {
    cout<<"\nGraph: "<<'\n';
    for(auto state : states) {
        cout<< "\nI" << state.first << ": \n";
        for(auto rules : state.second) {
            string ruleStr = "";
            ruleStr += rules.first;
            ruleStr += delim;
            for(auto rule : rules.second) {
                for(auto ch : rule.first)
                    ruleStr += ch;
                ruleStr += '.';
                for(auto ch : rule.second)
                    ruleStr += ch;
                ruleStr += '|';
            }

            ruleStr.pop_back();  

            for(auto ch : ruleStr) {
                if(ch == '!')
                    cout << startChar << '\'';
                else cout << ch;
            }
            cout << '\n';
        }
    }

    cout << "\nEdges:\n";
    for(auto edge : gotos){
        for(auto ele : edge.second){
            cout << "I" << edge.first << " -> " << ele.second << " -> " << "I" << ele.first << "\n";
        }
    }
}

// Function to generate the parse table
vector<vector<string>> generateParseTable() {
    int numStates = states.size();
    vector<vector<string>> parseTable(numStates, vector<string>(symbols.size(), ""));

    for(int i=0; i < numStates; i++) {
        // Print terminals
        for(auto term : terminals) {
            bool foundTransition = false;
            if(gotos.find(i) != gotos.end()) {
                for(auto edge : gotos[i]) {
                    if(edge.second == term) {
                        foundTransition = true;
                        parseTable[i][findIdx(term)] = "S" + to_string(edge.first);
                        break;
                    }
                }
                if(foundTransition)
                    continue;
            }
            bool isAcceptingState = false;
            for(auto rule : states[i]) {
                if(rule.first == '!' && term == TERMINAL && (*rule.second.begin()).second.empty()) {
                    parseTable[i][findIdx(term)] = "AC";
                    isAcceptingState = true;
                    break;
                }
            }

            if (isAcceptingState)
                continue;

            for(auto rule : states[i]) {
                char lhs = rule.first;
                //  map<char, set<pair<deque<char>, deque<char>>>>
                for (auto ele : rule.second) {
                    if (ele.second.empty()) {
                        for(auto ch : followMap[lhs]) {
                            if(term == ch) {
                                parseTable[i][findIdx(term)] = "r" + to_string(prodRules[{lhs, ele.first}]);
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Print non terminals
        for(auto nonTerm : nonTerminals) {
            if(gotos.find(i) != gotos.end()) {
                for(auto edge : gotos[i]) {
                    if(edge.second == nonTerm) {
                        parseTable[i][findIdx(nonTerm)] = to_string(edge.first);
                        break;
                    }
                }
            }
        }
    }

    return parseTable;
}

// Function to print the parse table
void printParseTable(vector<vector<string>>& parseTable) {
    int r = parseTable.size();
    int c = parseTable[0].size();
    cout << "\nParsing Table:\n";
    cout << "St.\t\tAction & Goto\n";

    cout << "  \t";
    for(auto ch : terminals)
        cout << ch << '\t';
    for(auto ch : nonTerminals)
        cout << ch << '\t';
    cout << '\n';

    for (int i=0; i < r; i++) {
        cout << "I" << i << '\t';
        for (int j=0; j < c; j++) {
            if (parseTable[i][j] == "") {
                cout << "-\t";
            }
            else {
                cout << parseTable[i][j] << '\t';
            }
        }
        cout << '\n';
    }
}

// Function to parse a string and verify if it is valid according to the given grammar
bool parseString(string _input, vector<vector<string>>& parseTable) {
    stack<char> parserStack;
    deque<char> input;
    for (auto ch : _input)
        input.push_back(ch);

    input.push_back(TERMINAL);

    parserStack.push('0');
    string option;

    while (!input.empty() && !parserStack.empty()) {
        int index = findIdx(input.front());
        
        if (index != -1)
            option = parseTable[parserStack.top() - '0'][index];
        else
            return false;

        if (option == "")
            return false;
        else if (option == "AC")
            return true;
        else if (option[0] == 'S') {
            parserStack.push(input.front());
            input.pop_front();
            parserStack.push(option[1]);
        }
        else if (option[0] == 'r') {
            for (int i=0; i < 2*(ruleNumbers[option[1] - '0'].second); i++) {
                parserStack.pop();
            }
            int newIndex = findIdx(ruleNumbers[option[1] - '0'].first);
            char gotoState;
            if (newIndex < 0)
                return false;
            else
                gotoState = (parseTable[parserStack.top() - '0'][newIndex])[0];
            parserStack.push(ruleNumbers[option[1] - '0'].first);
            parserStack.push(gotoState);
        }
    }

    return true;
}

// Driver Function
int main() {
    string grammarFile = "grammar.txt";
    string inputString = "a+a+a*a";

    inputGrammar(grammarFile);
    
    findFirstSet();

    findFollowSet();

    findStates();

    printProductions();

    // printGraph();
    cout << "Generating parsing table:\n";
    vector<vector<string>> parseTable = generateParseTable();
    cout << "Generated!\n";
    // printParseTable(parseTable);


    cout << "Size is: " << parseTable.size() << "x" << parseTable[0].size() << "\n";
    if (parseString(inputString, parseTable)) {
        cout << "\nGiven string is Valid!\n\n";
    }
    else {
        cout << "\nGiven string is Invalid!\n\n";
    }

    return 0;
}