#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int status;
} grepFlag;

grepFlag *init_flag();

typedef struct list {
  char name[256];
  struct list *next;
} list_node;

void output_err();
void start_grep(int argc, char **argv);
void search_all_flag(int argc, char **argv, grepFlag *flags);
int search_flags(const char *arg_str);
int search_flag(char *flags_str, grepFlag *flags);
int search_l_flag(grepFlag *flags, int find, int *n_l_print);
void search_c_flag(grepFlag *flags, int match, int n_match, int f_count,
                   char *file_name);
int search_e_flag(int arg_idx, char **argv);
int search_e_mid(char *arg_str, list_node **word_head, grepFlag *flags);
int search_f_flag(int argc, char **argv, int *init, list_node **word_head);
void get_word_file(int argc, char **argv, list_node **word_head,
                   list_node **file_head, grepFlag *flags);
list_node *create_node(char *str);
void free_node(list_node *head);
void add_elem_node(list_node *head, char *word_str);
void add_word_f(char *file_n, list_node **word_head);
void watching_file(list_node *files, list_node *words, grepFlag *flags);
int count_file(list_node *file_head);
void file_process(FILE *file, list_node *words, grepFlag *flags, int file_numb,
                  char *file_name);
int find_word_str(char *str, list_node *words, grepFlag *flags);
void file_process_o(FILE *file, list_node *words, grepFlag *flags,
                    int file_numb, char *file_name);
void trim_n(char *line);
int make_reg_compare(regex_t *regex, grepFlag *flags, char *keyword);
void print_o_match(size_t offset, regmatch_t match, char *buff, grepFlag *flags,
                   int line_numb, int *line_print, int f_count,
                   char *file_name);
void search_o_pattern(char *str, list_node *words, char *file_name, int f_count,
                      grepFlag *flags, int line_numb, int *line_print);

#endif  // SRC_GREP_S21_GREP_H_
