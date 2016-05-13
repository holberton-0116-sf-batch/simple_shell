CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
EXEC_FILE = shell
C_FILES = main.c print_char.c
OBJECTS	:= $(C_FILES:.c=.o)

$(EXEC_FILE): $(OBJECTS) libshell/libshell.a
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(C_FILES)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC_FILE)
