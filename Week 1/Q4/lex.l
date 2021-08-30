%{
    #include <stdio.h>
    int lineCount = 0, charCount = 0;
%}

%%
\n          lineCount++;
[ \t]+      ;
.           charCount++;
%%

int yywrap(void) {
    return 1;
}

int main() {
    yylex();
    printf("\nTotal number of lines: %d\nTotal number of characters: %d\n\n", lineCount, charCount);

    return 0;
}