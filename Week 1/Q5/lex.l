%%
([ \t])+ {fprintf(yyout, " ");}
. fprintf(yyout, "%s", yytext);
%%

int yywrap(){}

int main(void) {
    yylex();
    printf("\n");
    return 0;
}