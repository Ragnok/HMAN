CFLAGS+=-std=c11 -Wall -Werror -Wno-deprecated -Wextra -Wstack-usage=1024 -pedantic -D _XOPEN_SOURCE=800

.PHONY: clean debug

TARGET=hangman

$(TARGET): $(OBJS)

debug: CFLAGS+=-g
debug: hangman

clean:
	rm hangman *.o
