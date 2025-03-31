# Makefile

# ============ Variables ============

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Iinclude -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib
LDLIBS = -lm -lncurses -lreadline

# Flex and Bison commands
FLEX = flex
BISON = bison

# Output program name
EXEC = ippp

# Source files (non-generated)
SRCS = src/main.c \
       src/error_handler.c \
       src/hash_table.c \
       src/symbol_table.c \
       src/functions.c \
       src/stack.c

# Generated files from Flex/Bison
LEX_SRC = src/lex.yy.c
BISON_SRC = src/syn.tab.c

# ============ Rules ============

# Default target
all: $(EXEC)

# 1) Generate lex.yy.c and lex.yy.h from lex.l
$(LEX_SRC): src/lex.l
	$(FLEX) --header-file=include/lex.yy.h -o $@ $<

# 2) Generate syn.tab.c and syn.tab.h from syn.y
$(BISON_SRC): src/syn.y
	$(BISON) -d -o $@ --defines=include/syn.tab.h $<

# 3) Compile everything into the final executable
$(EXEC): $(SRCS) $(LEX_SRC) $(BISON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# 4) Cleanup
clean:
	rm -f \
	$(EXEC) \
	$(LEX_SRC) \
	include/lex.yy.h \
	$(BISON_SRC) \
	include/syn.tab.h

.PHONY: all clean