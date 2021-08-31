# Define the C Compiler Used
CC = gcc

# Define the name of the created C file
CFILE = q2.c

# Create the executable file
main: $(CFILE)
	@$(CC) -o $@ $^

# Clean up intermediate files
.PHONY: clean
clean:
	@rm main comments.txt identifiers.txt keywords.txt symbols.txt words.txt operators.txt