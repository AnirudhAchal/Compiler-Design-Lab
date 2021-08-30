# Define the C Compiler Used
CC = gcc

# Define the name of the created C file
CFILE = q7.c

# Create the executable file
main: $(CFILE)
	@$(CC) -o $@ $^

# Clean up intermediate files
.PHONY: clean
clean:
	@rm main identifier specialchar