// Program to parse the given list of tokens using the rules of a given grammar using LL(1) parser and print intermediate productions
#include <bits/stdc++.h>

using namespace std;

// Map to store First Sets
unordered_map<char, set<char>> firstMap;

// Map to store Follow Sets
unordered_map<char, set<char>> followMap;

// Map to store Production Rules
unordered_map<char, unordered_set<string>> prodRules;

// Map to store non terminals in the grammar
unordered_map<char, int> nonTerminals;

// Map to store terminals in the grammar
unordered_map<char, int> terminals;

// Start Character of the Grammar
char startChar;

// Function to split the production rule into individual productions
void tokenize(string s, char nonTerminal) {
    int start = 0;
    int end = s.find('|');
    while (end != -1) {
        prodRules[nonTerminal].insert(s.substr(start, end - start));
        start = end + 1;
        end = s.find('|', start);
    }
    prodRules[nonTerminal].insert(s.substr(start, end - start));
}

// Function to find First Set of a symbol
void findFirstSet(char c) {
	// If c is not a non terminal, add c to its own First Set
	if (!isupper(c))
		firstMap[c].insert(c);

	// Loop through production rules of c
    for (auto rule : prodRules[c]) {
		// If c -> epsilon, add epsilon to the First Set
        if (rule == "#")
            firstMap[c].insert('#');
        else {
			// Count number of epsilons derived from a single production
            int epCount = 0;

			// Loop over all symbols in the production
            for (auto ch : rule) {
				// If First Set for a particular symbol has not already been calculated, call the same function on that symbol
                if (firstMap[ch].size() == 0)
                    findFirstSet(ch);
                
				// Perform Set Union of First Set of c and First Set of current symbol
                set_union(firstMap[c].begin(), firstMap[c].end(), firstMap[ch].begin(), firstMap[ch].end(), inserter(firstMap[c], firstMap[c].begin()));

				// If the current symbol can derive epsilon, check next symbol
                if (firstMap[ch].find('#') != firstMap[ch].end())
                    epCount++;
                else
                    break;
            }

			// If all symbols in the rule derive epsilon, add epsilon to the First Set of c
            if (epCount == rule.size())
                firstMap[c].insert('#');
        }
    }
}

// Function to derive the Follow Set of a symbol
void findFollowSet(char c) {
	// If c is the start character, add $ to the Follow Set of c
	if (c == startChar) {
		followMap[c].insert('$');
	}

	// Loop through all Production Rules
	for (auto ele : prodRules) {
		// Loop through each Production
		for (auto rule : ele.second) {
			// Loop through each symbol in the Production
			for (int i=0; i < rule.size(); i++) {
				if (rule[i] == c) {
					// If c is the last symbol in the production, Follow(c) = Follow(ele.first)
					if (i == rule.size() - 1) {
						if (followMap[ele.first].size() == 0 && ele.first != c)
							findFollowSet(ele.first);

						set_union(followMap[c].begin(), followMap[c].end(), followMap[ele.first].begin(), followMap[ele.first].end(), inserter(followMap[c], followMap[c].begin()));
					}

					// If c is not the last symbol, find the First Set for all symbols after c
					int j = i+1;
					for (; j < rule.size(); j++) {
						if (firstMap[rule[j]].size() == 0)
							findFirstSet(rule[j]);

						// Create a temporary set to remove epsilon
						set<char> tempSet = firstMap[rule[j]];
						tempSet.erase('#');

						// Set Union of First Set of the symbols after c and the Follow Set of c
						set_union(tempSet.begin(), tempSet.end(), followMap[c].begin(), followMap[c].end(), inserter(followMap[c], followMap[c].begin()));

						if (firstMap[rule[j]].find('#') == firstMap[rule[j]].end())
							break;
					}
					// If all symbols after c can derive epsilon, Follow Set of c will also contain Follow Set of ele.first
					if (j == rule.size() && firstMap[rule.back()].find('#') != firstMap[rule.back()].end()) {
						if (followMap[ele.first].size() == 0 && ele.first != c)
							findFollowSet(ele.first);
						
						set_union(followMap[c].begin(), followMap[c].end(), followMap[ele.first].begin(), followMap[ele.first].end(), inserter(followMap[c], followMap[c].begin()));
					}
				}
			}
		}
	}
}

// Function to generate the Parse Table
void generateParseTable(vector<vector<string>>& parseTable) {
    // Loop through all production rules
	for (auto prodRule : prodRules) {
        // Loop through each production
		for (auto prod : prodRule.second) {
			int epCount = 0;
            // If the non terminal directly derives epsilon
			for (auto c : prod) {
                if (c == '#') {
                    epCount++;
                    continue;
                }

                // Find the First Set of the production
				for (auto symbol : firstMap[c]) {
					if (symbol == '#')
						epCount++;
					else {
                        string temp;
                        temp += prodRule.first;
                        temp += " => ";
                        temp += prod;
						parseTable[nonTerminals[prodRule.first]][terminals[symbol]] = temp;
					}
				}

                // If the current character does not derive epsilon, the First Set cannot be extended
                if (firstMap[c].find('#') == firstMap[c].end())
                    break;
			}

            // If First Set of production can derive epsilon
			if (epCount == prod.size()) {
                // Loop over the Follow Set for the non terminal
				for (auto symbol : followMap[prodRule.first]) {
                    string temp;
                    temp += prodRule.first;
                    temp += " => ";
                    temp += '#';
					parseTable[nonTerminals[prodRule.first]][terminals[symbol]] = temp;
				}
			}
		}
	}
}

// Function to print the parse table
void printParseTable(vector<vector<string>>& parseTable) {
    cout << "\nParse Table\n\n";

    vector<char> terminalSorted(terminals.size());

    for (auto symbol : terminals) {
        terminalSorted[symbol.second] = symbol.first;
    }

    for (auto c : terminalSorted) {
        cout << "\t" << c;
    }
    
    cout << "\n";
    for (auto symbol : nonTerminals) {
        cout << symbol.first << "\t";
        for (auto entry : parseTable[symbol.second]) {
            cout << entry << "\t";
        }
        cout << "\n";
    }
}

// Fuction to parse the list of tokens 
// Returns true if successfully parsed
// Else returns false
bool parseTokens(vector<char> &tokens, vector<vector<string>> &parseTable) {
    char start_symbol = 'S';
    stack<char> st;
    st.push('$');
    st.push(start_symbol);
    int look_ahead = 0;

    while(st.size() && look_ahead < tokens.size())
    {
        char st_top = st.top();
        st.pop();

        if(st_top == tokens[look_ahead])
        {
            look_ahead++;
        }
        else {
            int i_index = nonTerminals[st_top];
            int j_index = terminals[tokens[look_ahead]];
            
            if(parseTable[i_index][j_index].size() == 0)
            {
                return false;
            }
            else {
                for(int k = parseTable[i_index][j_index].size() - 1; k > 4; k--)
                {
                    st.push(parseTable[i_index][j_index][k]);
                }
            }
        }
    }

    if(st.size() > 0)
    {
        return false;
    }
    else {
        return true;
    }
}

// Function to print the productions
void printProductions(vector<char> tokens, int look_ahead, stack<char> st)
{
    for(int i = 0; i < look_ahead && tokens[i] != '$'; i++)
    {
        cout << tokens[i] << " ";
    }

    stack<char>t_st;
    
    while(st.size() > 1)
    {
        cout << st.top() << " ";
        t_st.push(st.top());
        st.pop();
    }

    cout << "\n";

    while(t_st.size() > 0)
    {
        st.push(t_st.top());
        t_st.pop();
    }
}

// Function to parse the list of tokens and prints the intermediate productions
// Returns true if successfully parsed
// Else returns false
bool parseTokensAndPrintProductions(vector<char> &tokens, vector<vector<string>> &parseTable) {
    char start_symbol = 'S';
    stack<char> st;
    st.push('$');
    st.push(start_symbol);
    int look_ahead = 0;

    cout << "\nProductions\n";
    printProductions(tokens, look_ahead, st);

    while(st.size() && look_ahead < tokens.size())
    {
        char st_top = st.top();
        st.pop();

        // If top is epsilon the pop from stack
        if(st_top == '#') 
        {
            printProductions(tokens, look_ahead, st);
            continue;
        }

        if(st_top == tokens[look_ahead])
        {
            look_ahead++;
        }
        else {
            int i_index = nonTerminals[st_top];
            int j_index = terminals[tokens[look_ahead]];
            
            if(parseTable[i_index][j_index].size() == 0)
            {
                return false;
            }
            else {
                for(int k = parseTable[i_index][j_index].size() - 1; k > 4; k--)
                {
                    st.push(parseTable[i_index][j_index][k]);
                }
            }
        }
        
        printProductions(tokens, look_ahead, st);
    }

    if(st.size() > 0)
    {
        return false;
    }
    else {
        return true;
    } 
}

// Driver Function
int main()
{
	// Open the file containing the grammar
	string fileLine;
	ifstream GrammarFile("grammar.txt");

	// Find the starting character (first character in the file)
	GrammarFile.get(startChar);
	GrammarFile.seekg(0);

	// Parse the production rules and store it in the prodRules map
	while (getline(GrammarFile, fileLine))
		tokenize(fileLine.substr(5, fileLine.size()-5), fileLine[0]);
	
	// Create a list of non terminal characters
    
	int i = 0;
    for (auto ele : prodRules)
        nonTerminals.insert({ele.first, i++});

	// Compute the First Sets for all non terminals
	for (auto ele : nonTerminals) {
		if (firstMap[ele.first].size() > 0)
			continue;

		findFirstSet(ele.first);
	}

	// Compute the Follow Sets for all non terminals
	for (auto ele : nonTerminals) {
		if (followMap[ele.first].size() > 0)
			continue;
		
		findFollowSet(ele.first);
	}

	i = 0;
	for (auto ele : firstMap) {
		if (!isupper(ele.first))
			terminals.insert({ele.first, i++});
	}
	terminals.insert({'$', i++});

    // Create a 3D Matix to store the parse table
	vector<vector<string>> parseTable(nonTerminals.size(), vector<string>(terminals.size()));

    // Generate the parse table
	generateParseTable(parseTable);

    // Print the parse table
    printParseTable(parseTable);

    // Input tokens from tokens.txt
    vector<char> tokens;
    ifstream TokensFile("tokens.txt");

    while(getline(TokensFile, fileLine)) {
        tokens.push_back(fileLine[0]); 
    }

    // Parse the tokens using the parse table
    if(parseTokensAndPrintProductions(tokens, parseTable)) {
        cout << "\nSuccessfully parsed\n\n"; 
    }
    else {
        cout << "\nThere is a syntax error\n\n";
    }

    GrammarFile.close();
    TokensFile.close();
	return 0;
}
