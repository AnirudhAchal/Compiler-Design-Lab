#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cout << "Enter the number of variables: ";
    cin >> n;

    bool hasDivide = false;
    bool hasFloat = false;

    vector<char> variable(n), type(n);
    vector<char> input;

    for(int i = 0; i < n; i++)
    {
        cout << "Enter the variable " << i + 1 << ": ";
        cin >> variable[i];

        cout << "Enter the variable-type " << i + 1 << " [float-f/int-i]: ";
        cin >> type[i];

        if(type[i] == 'f')
        {
            hasFloat = true;
        }
    }
    
    cout << "Enter an expression (end with $):";
    char c;
    cin >> c;

    while(c != '$') {
        input.push_back(c);
        cin >> c;
    }

    int k = input.size();
        
    for(int i = 0; i < k; i++)
    {
        if(input[i]=='/')
        {
            hasDivide = true;
            break;  
        }  
    }

    for(int i = 0; i < n; i++)
    {
        if(input[0] == variable[i])
        {
            if(hasDivide || hasFloat)
            {
                if(type[i] == 'f')
                {  
                    cout << "\nThe datatype is correctly defined\n";
                    break;  
                }
                else {  
                    cout << "Identifier " << variable[i] << " must be a float type\n";
                    break;  
                } 
            }
            else {  
                cout << "\nThe datatype is correctly defined\n";
                break;  
            }  
        }
    }

    return 0;
}