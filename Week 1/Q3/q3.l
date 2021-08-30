%%
. fprintf(yyout, "%s", yytext);
%%
int yywrap(){}

int main(void) {
    yylex();
    return 0;
}