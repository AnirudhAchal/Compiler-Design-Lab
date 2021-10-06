// C++ Program to find the LR(0) items for a given grammar
#include <bits/stdc++.h>

using namespace std;

// Map to store Production Rules
unordered_map<char, unordered_set<string>> prodRules;

// Map to store the LR(0) items
unordered_map<char, vector<string>> LR0Items;

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

// Function to derive LR(0) items
void findLR0Items() {
    for (auto rule : prodRules) {
        for (auto str : rule.second) {
            for (int i=0; i <= str.size(); i++) {
				string temp = str;
				temp.insert(i, ".");
                LR0Items[rule.first].push_back(temp);
            }

            if (str == "#") {
                LR0Items[rule.first].pop_back();
            }
        }
    }
}

// Function to print the LR(0) items
void printRules() {
    for (auto item : LR0Items) {
        cout << "\n" << item.first << " => ";
        for (auto str : item.second) {
            cout << str << " | ";
        }
        cout << "\b\b ";
    }
}

// Driver Function
int main()
{
	// Open the file containing the grammar
	string fileLine;
	ifstream GrammarFile("grammar.txt");

	// Parse the production rules and store it in the prodRules map
	while (getline(GrammarFile, fileLine))
		tokenize(fileLine.substr(5, fileLine.size()-5), fileLine[0]);
	
	// Create a list of non terminal characters
    vector<char> nonTerminals;
    for (auto ele : prodRules)
        nonTerminals.push_back(ele.first);

    // Find the LR(0) items
    findLR0Items();

    // Print the LR(0) items
    printRules();

    GrammarFile.close();
	return 0;
}