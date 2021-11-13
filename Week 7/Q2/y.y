%{
#include"y.tab.h"
#include<stdio.h>
char addtotable(char,char,char);
void machine(char,char,char,char);
int a_26[26];
int index1=0;
char temp = 'A'-1;


struct expr{

char operand1;
char operand2;
char operator;
char result;
};



%}

%union{
char symbol;
}




%left '+' '-'
%left '/' '*'

%token <symbol> LETTER NUMBER
%type <symbol> exp
%%

statement: LETTER '=' exp ';' {addtotable((char)$1,(char)$3,'=');};
exp: exp '+' exp {$$ = addtotable((char)$1,(char)$3,'+');}
    |exp '-' exp {$$ = addtotable((char)$1,(char)$3,'-');}
    |exp '/' exp {$$ = addtotable((char)$1,(char)$3,'/');}
    |exp '*' exp {$$ = addtotable((char)$1,(char)$3,'*');}
    |'(' exp ')' {$$= (char)$2;}
    |NUMBER {$$ = (char)$1;}
    |LETTER {(char)$1;};

%%

struct expr arr[20]; 

void yyerror(char *s){
    printf("Errror %s",s);
}

char addtotable(char a, char b, char o){
    temp++;
    machine(a,b,o,temp);
    arr[index1].operand1 =a;
    arr[index1].operand2 = b;
    arr[index1].operator = o;
    arr[index1].result=temp;
    index1++;
    return temp;
}

void machine(char a, char b, char o, char t)
{
    if(a_26[a-'a']==0 && isupper(a)==0)
    {
        printf("LOAD \t$%c, %c, 0\n",a,a);
        a_26[a-'a']=1;
    }
    if(a_26[b-'a']==0 && isupper(b)==0)
    {
        printf("LOAD \t$%c, %c, 0\n",b,b);
        a_26[b-'a']=1;
    }
    switch(o)
    {
        case '+':
        {
            printf("ADD \t$%c, $%c, $%c\n",t,a,b);
            break;
        }
        case '-':
        {
            printf("SUB \t$%c, $%c, $%c\n",t,a,b);
            break;
        }
        case '*':
        {
            printf("MUL \t$%c, $%c, $%c\n",t,a,b);
            break;
        }
        case '/':
        {
            printf("DIV \t$%c, $%c, $%c\n",t,a,b);
            break;
        }
        case '=':
        {
            printf("MOV \t$%c, $%c\n",a,b);
            printf("STORE \t$%c, %c\n",a,a);
            break;
        }
    }
}

int find(char l){
    int i;
    for(i=0;i<index1;i++)
        if(arr[i].result==l) break;
    return i;
}

int yywrap(){
    return 1;
}

int main(){
    printf("It converts to the RISCV code \n");
    printf("Enter the expression(expression must end with ;): ");
    yyparse();
    printf("\nCode Generation finished!!!!\n");
    return 0;
}
