#include<string.h>
#include<ctype.h>
#include<stdio.h>

void keyword(char str[10])
{
    if(
        strcmp("int", str) == 0 ||
        strcmp("if", str) == 0 ||
        strcmp("then", str) == 0 ||
        strcmp("else", str) == 0 ||
        strcmp("endif", str) == 0 ||
        strcmp("while", str) == 0 ||
        strcmp("do", str) == 0 ||
        strcmp("enddo", str) == 0 ||
        strcmp("print", str) == 0
    )
    {
        printf("\n%s is a keyword", str);
    }
    else {
        printf("\n%s is an identifier", str);
    } 
}

int main()
{
    FILE *f1, *f2, *f3;
    char c, str[10], st1[10];
    int num[100];
    int num_count = 0;
    int line_count = 0;
    int value = 0;
    int i = 0;

    // f1 = fopen("input","w");
    // printf("\nEnter the c program");

    // while((c=getchar()) != EOF)
    // {
    //     putc(c,f1);
    // }
    // fclose(f1);

    f1=fopen("input.txt","r");
    f2=fopen("identifier","w");
    f3=fopen("specialchar","w");

    while((c = getc(f1)) != EOF)
    {
        /***/
        // Ignore comments
        if(c == '/')
        {
            c = getc(f1);
            if(c == '*')
            {
                // Ignore till end of comment
                while (1) {
                    while((c = getc(f1)) != '*');
                    if (c == '*' && (c = getc(f1) == '/'))
                        break;
                    else
                        ungetc(c, f1);
                }
                line_count++;
            }
            else {
                ungetc(c, f1);
            }
        }

        if(isdigit(c))
        {
            value = c - '0';
            while(isdigit(c = getc(f1))) 
            {
                value = value * 10 + (c - '0');
            }
            num[num_count++] = value;
        }
        else {
            if(isalpha(c))
            {
                putc(c,f2);
                c = getc(f1);
                while(isdigit(c) || isalpha(c) || c== '_' || c == '$')
                {
                    putc(c, f2);
                    c=getc(f1);
                }
                putc(' ', f2);
                ungetc(c, f1);
            }
            else if(c == ' ' || c == '\t')
            {
                continue;
            }
            else if(c == '\n')
            {
                line_count++;
            }
            else {
                // special character
                putc(c, f3);
            }
        }
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);

    printf("\nThe numbers in the program: ");
    for(i = 0; i < num_count; i++)
    {
        printf("%d ", num[i]);
    }
    printf("\n");

    f2 = fopen("identifier", "r");
    i = 0;

    printf("\nThe keywords and identifiers are: ");
    while((c = getc(f2)) != EOF) 
    {
        if(c != ' ')
        {
            str[i++] = c;
        }
        else
        {
            str[i]='\0';
            keyword(str);
            i = 0; 
        }
    }
    fclose(f2);

    f3 = fopen("specialchar", "r");
    printf("\n\nSpecial characters are: ");
    while((c = getc(f3))!=EOF)
    {
        printf("%c ",c);
    }
    printf("\n");
    fclose(f3);

    printf("\nTotal no. of lines are: %d\n",line_count);
    
    return 0;
    
} 
