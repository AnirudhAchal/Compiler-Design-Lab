// Program to find the First and Follow sets of a given grammar
#include <bits/stdc++.h>

using namespace std;

// Map to store First Sets
unordered_map<char, set<char>> firstMap;

// Map to store Follow Sets
unordered_map<char, set<char>> followMap;

// Map to store Production Rules
unordered_map<char, unordered_set<string>> prodRules;

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

// Function to print the First Sets of all non terminals
void printFirst() {
	cout << "\nFirst Set:\n\n";

	for (auto ele : firstMap) {
		if (isupper(ele.first)) {
			cout << "First(" << ele.first << ") = {";
			for (auto c : ele.second) {
				cout << c << ", ";
			}
			cout << "\b\b}\n";
		}
	}
}

// Function to print the Follow Set of all non terminals
void printFollow() {
	cout << "\nFollow Set:\n\n";

	for (auto ele : followMap) {
		if (isupper(ele.first)) {
			cout << "Follow(" << ele.first << ") = {";
			for (auto c : ele.second) {
				cout << c << ", ";
			}
			cout << "\b\b}\n";
		}
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
    vector<char> nonTerminals;
    for (auto ele : prodRules)
        nonTerminals.push_back(ele.first);

	// Compute the First Sets for all non terminals
	for (auto ele : nonTerminals) {
		if (firstMap[ele].size() > 0)
			continue;

		findFirstSet(ele);
	}

	// Compute the Follow Sets for all non terminals
	for (auto ele : nonTerminals) {
		if (followMap[ele].size() > 0)
			continue;
		
		findFollowSet(ele);
	}

	// Print the First and Follow sets
	printFirst();
	printFollow();

    GrammarFile.close();
	return 0;
}
