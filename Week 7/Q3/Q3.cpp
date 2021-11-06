// C++ Program to optimize a given piece of code
#include <bits/stdc++.h>

using namespace std;

// Global vector to store the code
vector<pair<char, string>> code;

// Helper function the replace a variable throughout the code
void replaceVar(char var, char toReplace)
{
    int pos;
    for (int i = 0; i < code.size(); i++)
    {
        while ((pos = code[i].second.find(toReplace)) != string::npos)
            code[i].second[pos] = var;
    }
}

// Helper function to print the code
void printCode()
{
    for (auto line : code)
        cout << line.first << " = " << line.second << '\n';
}

// Function to read code from the specified file
void readCodeFile(string fileName)
{
    ifstream CodeFile(fileName);

    // Error Handling
    if (CodeFile.fail())
    {
        cout << "\nFile not found!\nExiting...\n\n";
        exit(1);
    }

    string fileLine;
    int idx;

    while (getline(CodeFile, fileLine))
        code.push_back({fileLine[0], fileLine.substr(2, fileLine.size() - 2)});

    CodeFile.close();
}

// Function to check for variable propogation
void checkVarPropogation()
{
    for (int i = 0; i < code.size(); i++)
    {
        if (code[i].second.size() == 1 && isalpha(code[i].second[0]))
        {
            bool flag = false;
            for (int j = i + 1; j < code.size(); j++)
            {
                if (code[j].first == code[i].first)
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
                replaceVar(code[i].second[0], code[i].first);
        }
    }
}

// Function to remove dead lines of code
void removeDeadCode()
{
    for (int i = 0; i < code.size() - 1; i++)
    {
        char temp = code[i].first;
        bool flag = false;
        for (auto line : code)
        {
            if (line.second.find(temp) != string::npos)
            {
                flag = true;
                break;
            }
        }
        if (!flag)
            code.erase(code.begin() + i--);
    }
}

// Function to remove common expressions
void removeCommonExpressions()
{
    int pos;
    for (int i = 0; i < code.size(); i++)
    {
        for (int j = i + 1; j < code.size(); j++)
        {
            if (code[i].second == code[j].second)
            {
                char toReplace = code[j].first;
                code.erase(code.begin() + j--);
                replaceVar(code[i].first, toReplace);
            }
        }
    }
}

// Driver function
int main(int argc, char *argv[])
{
    string fileName = "code.py";
    if (argc == 2)
    {
        fileName = argv[1];
    }
    readCodeFile(fileName);

    checkVarPropogation();
    cout << "\nAfter checking variable propogation:\n";
    printCode();

    removeDeadCode();
    cout << "\nAfter removing dead code:\n";
    printCode();

    removeCommonExpressions();
    cout << "\nAfter removing common expressions:\n";
    printCode();

    cout << endl;
    return 0;
}
