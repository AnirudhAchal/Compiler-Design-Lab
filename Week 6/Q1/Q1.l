%{
    #include "Q1.tab.h"
%}

LETTER [a-zA-Z]+

%%

{LETTER} {yylval = (char)yytext[0]; return LETTER;}
.|\n     {return *yytext;}

%%
