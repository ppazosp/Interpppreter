CC = gcc

CFLAGS = -Wall -Iinclude -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib
LDLIBS = -lm -lncurses -lreadline

FLEX = flex
BISON = bison

EXEC = ippp

SRCS = src/main.c \
       src/error_handler.c \
       src/hash_table.c \
       src/symbol_table.c \
       src/functions.c \
       src/stack.c

LEX_SRC = src/lex.yy.c
BISON_SRC = src/syn.tab.c


all: $(EXEC)

$(LEX_SRC): src/lex.l
	$(FLEX) --header-file=include/lex.yy.h -o $@ $<

$(BISON_SRC): src/syn.y
	$(BISON) -d -o $@ --defines=include/syn.tab.h $<

$(EXEC): $(SRCS) $(LEX_SRC) $(BISON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f \
	$(EXEC) \
	$(LEX_SRC) \
	include/lex.yy.h \
	$(BISON_SRC) \
	include/syn.tab.h

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

.PHONY: all clean valgrind
