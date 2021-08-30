# Define the C Compiler Used
CC = gcc

# Define the lexical analyser used
LEX = flex

# Define the name of the created C file
CFILE = lex.c

# Convert the Lex file into C code
%.c: %.l
	@$(LEX) -o $@ $<

# Create the executable file
main: $(CFILE)
	@$(CC) -o $@ $^

# Clean up intermediate files
.PHONY: clean
clean:
	@rm main *.c output.txt