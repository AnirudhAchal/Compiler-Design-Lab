%{
    int isInComment = 0;
%}

identifier [a-zA-Z][a-zA-Z0-9]*

%%

"/*" isInComment = 1;
"*/" isInComment = 0;
[0-9]+ {if(!isInComment) printf("\n%s is a Constant",yytext);}
int|if|then|else|endif|while|do|enddo|print {if(!isInComment) printf("\n%s is a Keyword", yytext);}
{identifier}(\[[0-9]*\])? {if(!isInComment) printf("\n%s is an Identifier", yytext);}
= {if(!isInComment)printf("\n%s is an Assignment Operator",yytext);}
\<=|\>=|\<|==|\> {if(!isInComment) printf("\n%s is a RelOp", yytext);}
\+|- {if(!isInComment) printf("\n%s is an AddOp", yytext);}
\*|\/ {if(!isInComment) printf("\n%s is an MultOp", yytext);}
,|;|\(|\)|\{|\} {if(!isInComment) printf("\n%s is a Special Symbol", yytext);}
. ;

%%
int yywrap() {}

int main()
{
    FILE *file;
    file = fopen("input.txt","r");
    yyin = file;

    yylex();
    printf("\n\n");
    return 0;
}
