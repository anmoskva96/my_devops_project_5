CC = gcc

CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	rm -rf s21_cat

valgrind: s21_cat
	valgrind --leak-check=full --tool=memcheck --log-file=log.txt ./s21_cat s21_cat.c

cppcheck: s21_cat
	cppcheck --enable=all --std=c11 --inconclusive --suppress=missingIncludeSystem *.c

rebuild: clean s21_cat
