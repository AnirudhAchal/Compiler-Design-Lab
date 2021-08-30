%%
([ \t])+ {fprintf(yyout, " ");}
. fprintf(yyout, "%s", yytext);
%%

int yywrap(){}

int main(void) {
    extern FILE *yyin, *yyout;

    yyin = fopen("input.txt", "r");
    yyout = fopen("output.txt", "w");

    yylex();
    return 0;
}