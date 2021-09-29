# Define the C Compiler Used
CC = g++

# Define the name of the created C file
CPPFILE = Q4.cpp

# Create the executable file
main: $(CPPFILE)
	@$(CC) -o $@ $^

# Clean up intermediate files
.PHONY: clean
clean:
	@rm main