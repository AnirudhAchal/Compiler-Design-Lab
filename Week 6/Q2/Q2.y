%{
	/* Preprocessor Directives */
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
    #include <ctype.h>

	/* Function Declarations */
	void printTriplesTable();
	char addToTriplesTable(char,char,char);
    void yyerror(char *s);
    int yylex();

	/* Global Variables */
	int idx = 0;
	char memAddr = 'A';
	struct TripleEntry
	{
		char arg1, arg2, op;
	} inputCode[20];
%}

%token LETTER NUMBER
%type expr
%left '+''-'
%right '/''*'

%%

statement: LETTER '=' expr ';' {$$ = addToTriplesTable((char)$1, (char)$3, '=');}
	|expr ';' ;
expr:	expr '+' expr {$$ = addToTriplesTable((char)$1, (char)$3, '+');}
	|expr '-' expr {$$ = addToTriplesTable((char)$1, (char)$3, '-');}
	|expr '*' expr {$$ = addToTriplesTable((char)$1, (char)$3, '*');}
	|expr '/' expr {$$ = addToTriplesTable((char)$1, (char)$3, '/');}
	|'('expr')'{$$ = (char)$2;}
	|LETTER {$$ = (char)$1;}
	|NUMBER {$$ = (char)$1;}
;

%%

// Add an instruction to the triples table
char addToTriplesTable(char arg1, char arg2, char op)
{
	// Create a new entry in the input code array
	inputCode[idx].arg1 = arg1;
	inputCode[idx].arg2 = arg2;
	inputCode[idx].op = op;

	// Increment the array pointer and memory location
	idx++;
	memAddr++;

	// Return the currently used memory location
	return memAddr - 1;
}

// Print the result triples table
void printTriplesTable()
{
	int i = 0;
	memAddr = 'A';
	printf("\nInstruction Triples:\n");
	for (i=0; i < idx; i++)
	{
		printf("%c =\t", memAddr);
		if(isalpha(inputCode[i].arg1))
			printf("%c \t", inputCode[i].arg1);
		else
			printf("%c \t", memAddr);

		printf("%c \t",inputCode[i].op);

		if(isalpha(inputCode[i].arg2))
			printf("%c \t",inputCode[i].arg2);
		else
			printf("%c \t",memAddr);

		printf("\n");
		memAddr++;
	}
	printf("\n");
}

// Error handler
void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
	exit(0);
}

// Function called when input is exhausted
int yywrap(void)
{
	return 1;
}

// Driver Function
int main(void)
{
	printf("\nEnter the Arithmetic Expression: ");
	yyparse();
	printTriplesTable();

	return 0;
}
