# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic

# Detect OS and set commands
ifeq ($(OS),Windows_NT)
    RM = cmd /c del /f /q
    EXE = .exe
else
    RM = rm -f
    EXE = 
endif

# Targets
TARGET1 = a3q1$(EXE)
TARGET2 = a3q2$(EXE)

# Source files
SRC1 = a3q1_main.c a3q1_functions.c
SRC2 = a3q2_main.c a3q2_functions.c

# Header files
HEADERS = a3q1_header.h a3q2_header.h

# Default target
all: $(TARGET1) $(TARGET2)

# Build a3q1
$(TARGET1): $(SRC1) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SRC1)

# Build a3q2
$(TARGET2): $(SRC2) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SRC2)

# Clean up
clean:
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)

.PHONY: all clean