%{
    /* Preprocessor Directives */
    #define YYSTYPE char
    #include "Q1.c"
    #include <ctype.h>
    #include <stdio.h>

    /* Function Declarations */
    void yyerror(char *);
%}

%token LETTER
%left '+'  '-'
%left '*'  '/'
%right UMINUS
%right '^'

%%

lines: E '\n' { printf("\n");}
	|
	;
E :E'+'E { printf("+ ");}
  |E'-'E { printf("- ");}
  |E'*'E { printf("* ");}
  |E'/'E { printf("/ ");}
  |'('E')' 
  |LETTER { printf("%c ", yylval);}
  ;

%%

// Error Handler
void yyerror(char *s) 
{
    fprintf(stderr, "%s\n", s);
}

// Function called when input is exhausted
int yywrap(void)
{
    return 1;
}

// Driver Function
int main(void) 
{
    printf("\nInput the Infix Arithmetic Expression: ");
    yyparse();

    return 0;
} 