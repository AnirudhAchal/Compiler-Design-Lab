#include <string.h>
#include <ctype.h>
#include <stdio.h>

int isOperator(char c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '=':
    case '>':
    case '<':
    case '!':
        return 1;
    }

    return 0;
}

void isKeywordOrIdentifier(char str[10], FILE *keywordFile, FILE *identifierFile)
{
    if (
        strcmp("int", str) == 0 ||
        strcmp("if", str) == 0 ||
        strcmp("then", str) == 0 ||
        strcmp("else", str) == 0 ||
        strcmp("endif", str) == 0 ||
        strcmp("while", str) == 0 ||
        strcmp("do", str) == 0 ||
        strcmp("enddo", str) == 0 ||
        strcmp("print", str) == 0)
    {
        fputs(str, keywordFile);
        putc(' ', keywordFile);
    }
    else
    {
        fputs(str, identifierFile);
        putc(' ', identifierFile);
    }
}

int main()
{
    // Define file pointers for the input program, words(keywords & variables), symbols and comments
    FILE *inputFile, *wordFile, *symbolFile, *operatorFile, *commentFile;
    char c, str[32];
    int num[100];
    int num_count = 0, line_count = 0;

    // Open the various files using file pointers
    inputFile = fopen("input.txt", "r");
    wordFile = fopen("words.txt", "w");
    symbolFile = fopen("symbols.txt", "w");
    operatorFile = fopen("operators.txt", "w");
    commentFile = fopen("comments.txt", "w");

    // Traverse through the entire input program
    while ((c = getc(inputFile)) != EOF)
    {
        // Check for opening of comment
        if (c == '/')
        {
            c = getc(inputFile);

            // Check if start of a block comment
            if (c == '*')
            {
                while (1)
                {
                    // Add to comments file till end of comment
                    while ((c = getc(inputFile)) != '*')
                        putc(c, commentFile);

                    // Check for end of comment
                    if ((c = getc(inputFile)) == '/')
                        break;
                    else
                    {
                        putc('*', commentFile);
                        ungetc(c, inputFile);
                    }
                }
                line_count++;
            }
            // If not a comment, return back to the previous character
            else
                ungetc(c, inputFile);
        }

        // Check for start of a constant
        if (isdigit(c))
        {
            // Compute the full value of the constant and add to number array
            int value = c - '0';
            while (isdigit(c = getc(inputFile)))
                value = value * 10 + (c - '0');

            num[num_count++] = value;
        }
        else
        {
            // Check for start of a word
            if (isalpha(c))
            {
                putc(c, wordFile);
                c = getc(inputFile);
                while (isdigit(c) || isalpha(c))
                {
                    putc(c, wordFile);
                    c = getc(inputFile);
                }
                putc(' ', wordFile);
                ungetc(c, inputFile);
            }

            // Ignore whitespaces
            else if (c == ' ' || c == '\t')
                continue;

            // Check for new line
            else if (c == '\n')
                line_count++;

            // Check for operator
            else if (isOperator(c))
            {
                putc(c, operatorFile);
                if (isOperator(c = getc(inputFile)))
                    putc(c, operatorFile);
                else
                    ungetc(c, inputFile);

                putc(' ', operatorFile);
            }

            // If nothing else is true, it is a special character
            else
            {
                putc(c, symbolFile);
                putc(' ', symbolFile);
            }
        }
    }

    fclose(inputFile);
    fclose(wordFile);
    fclose(symbolFile);
    fclose(operatorFile);
    fclose(commentFile);

    FILE *keywordFile, *identifierFile;
    int i = 0;

    wordFile = fopen("words.txt", "r");
    keywordFile = fopen("keywords.txt", "w");
    identifierFile = fopen("identifiers.txt", "w");

    while ((c = getc(wordFile)) != EOF)
    {
        if (c != ' ')
        {
            str[i++] = c;
        }
        else
        {
            str[i] = '\0';
            isKeywordOrIdentifier(str, keywordFile, identifierFile);
            i = 0;
        }
    }
    fclose(wordFile);
    fclose(keywordFile);
    fclose(identifierFile);

    // Print the constants
    printf("\nConstants: ");
    for (i = 0; i < num_count; i++)
        printf("%d ", num[i]);

    keywordFile = fopen("keywords.txt", "r");
    identifierFile = fopen("identifiers.txt", "r");
    commentFile = fopen("comments.txt", "r");
    operatorFile = fopen("operators.txt", "r");
    symbolFile = fopen("symbols.txt", "r");

    printf("\n\nKeywords: ");
    while ((c = getc(keywordFile)) != EOF)
        printf("%c", c);

    printf("\n\nIdentifiers: ");
    while ((c = getc(identifierFile)) != EOF)
        printf("%c", c);

    printf("\n\nOperators: ");
    while ((c = getc(operatorFile)) != EOF)
        printf("%c", c);

    printf("\n\nComments: ");
    while ((c = getc(commentFile)) != EOF)
        printf("%c", c);

    printf("\n\nSymbols: ");
    while ((c = getc(symbolFile)) != EOF)
        printf("%c", c);

    printf("\n\nTotal no. of lines are: %d\n", line_count);

    fclose(keywordFile);
    fclose(identifierFile);
    fclose(commentFile);
    fclose(operatorFile);
    fclose(symbolFile);

    return 0;
}
