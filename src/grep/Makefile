CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: s21_grep

s21_grep:
	${CC} ${CFLAGS} s21_grep.c -o s21_grep

clean:
	rm -rf s21_grep.d* s21_grep test_sys_grep.log test_s21_grep.log

test: test_func_grep.sh test_6_grep.txt test_5_grep.txt s21_grep
	sh test_func_grep.sh
test_leak: test_leak_grep.sh test_6_grep.txt
	sh test_leak_grep.sh

valgrind: s21_grep
	valgrind --leak-check=full --tool=memcheck --log-file=log.txt ./s21_grep s21_grep.c

cppcheck: s21_grep
	cppcheck --enable=all --std=c11 --inconclusive --suppress=missingIncludeSystem *.c

rebuild: clean all
