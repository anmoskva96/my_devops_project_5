#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    output_err();
  } else {
    start_grep(argc, argv);
  }
  return 0;
}

void output_err() {
  fprintf(
      stderr,
      "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
      "[-C[num]]\n"
      "        [-e pattern] [-f file] [--binary-files=value] [--color=when]\n"
      "        [--context[=num]] [--directories=action] [--label] "
      "[--line-buffered]\n"
      "        [--null] [pattern] [file ...]\n");
  exit(1);
}

void start_grep(int argc, char **argv) {
  grepFlag *flags = init_flag();
  list_node *word_head = NULL;
  list_node *file_head = NULL;
  search_all_flag(argc, argv, flags);
  get_word_file(argc, argv, &word_head, &file_head, flags);
  watching_file(file_head, word_head, flags);
  free(flags);
  free_node(file_head);
  free_node(word_head);
}

grepFlag *init_flag() {
  grepFlag *flags = malloc(sizeof(grepFlag));
  flags->e = 0;
  flags->i = 0;
  flags->v = 0;
  flags->c = 0;
  flags->l = 0;
  flags->n = 0;
  flags->h = 0;
  flags->s = 0;
  flags->f = 0;
  flags->o = 0;
  flags->status = 0;
  return flags;
}

void search_all_flag(int argc, char **argv, grepFlag *flags) {
  for (int i = 1; i < argc; i++) {
    if (search_flags(argv[i]) && !search_e_flag(i - 1, argv)) {
      flags->status = search_flag(argv[i], flags);
      if (!flags->status) {
        output_err();
      }
    }
  }
}

int search_flags(const char *arg_str) {
  int fix_value = 0;
  if (*arg_str == '-') {
    fix_value = 1;
  }
  return fix_value;
}

int search_flag(char *flags_str, grepFlag *flags) {
  int return_value = 1;
  if (*flags_str == '-') {
    flags_str++;
  }
  while (*flags_str) {
    if (*flags_str == 'e') {
      flags->e = 1;
      break;
    } else if (*flags_str == 'i') {
      flags->i = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'v') {
      flags->v = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'c') {
      flags->c = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'l') {
      flags->l = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'n') {
      flags->n = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'h') {
      flags->h = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 's') {
      flags->s = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'o') {
      flags->o = 1;
      flags_str++;
      continue;
    } else if (*flags_str == 'f') {
      flags->f = 1;
      break;
    } else {
      fprintf(stderr, "grep: invalid option -- %c\n", *flags_str);
      return_value = 0;
      break;
    }
  }
  return return_value;
}

int search_l_flag(grepFlag *flags, int find, int *n_l_print) {
  int result = 0;
  if (((find && !flags->v) || (!find && flags->v)) && flags->l) {
    result = 1;
    *n_l_print = 1;
  }
  return result;
}

void search_c_flag(grepFlag *flags, int match, int n_match, int f_count,
                   char *file_name) {
  if (flags->c) {
    if (f_count > 1 && !flags->h) {
      printf("%s:", file_name);
    }
    if (flags->v) {
      printf("%d\n", n_match);
    } else {
      printf("%d\n", match);
    }
  }
}

int search_e_flag(int arg_idx, char **argv) {
  int fix_value = 0;
  if (arg_idx >= 1) {
    int len = strlen(argv[arg_idx]);
    if (*(argv[arg_idx]) == '-' && *(argv[arg_idx] + 1) == 'e' &&
        *(argv[arg_idx] + 2) == 0) {
      fix_value = 1;
    } else if (argv[arg_idx][len - 1] == 'e' && argv[arg_idx][0] == '-' &&
               argv[arg_idx][1] != 'e') {
      fix_value = 1;
    }
  }
  return fix_value;
}

int search_e_mid(char *arg_str, list_node **word_head, grepFlag *flags) {
  int return_value = 0;
  if (flags->e) {
    while (*arg_str) {
      if (*arg_str == 'e' && *(arg_str + 1) != 0) {
        if (*word_head == NULL) {
          *word_head = create_node((arg_str + 1));
        } else {
          add_elem_node(*word_head, (arg_str + 1));
          return_value = 1;
          break;
        }
      }
      arg_str++;
    }
  }
  return return_value;
}

int search_f_flag(int argc, char **argv, int *init, list_node **word_head) {
  int return_values = 0;
  int len = strlen(argv[*init]);
  char *pattern_f;
  if (argv[*init][0] == '-' &&
      ((argv[*init][1] == 'f' && argv[*init][2] == '\0') ||
       argv[*init][len - 1] == 'f')) {
    if (*(init + 1) < argc) {
      add_word_f(argv[*init + 1], word_head);
      *init += 1;
      return_values = 1;
    } else {
      output_err();
    }
  } else if (argv[*init][0] == '-' && (pattern_f = strchr(argv[*init], 'f')) &&
             *(pattern_f + 1) != '\0') {
    add_word_f(pattern_f + 1, word_head);
  }
  return return_values;
}

void get_word_file(int argc, char **argv, list_node **word_head,
                   list_node **file_head, grepFlag *flags) {
  int m_word = 0;
  for (int i = 1; i < argc; i++) {
    if (flags->f) {
      if (search_f_flag(argc, argv, &i, word_head)) {
        continue;
      }
    }
    if (search_flags(argv[i]) && search_e_mid(argv[i], word_head, flags)) {
      continue;
    }
    if ((flags->e && search_e_flag(i - 1, argv)) ||
        (!flags->e && !search_flags(argv[i]) && !m_word && !flags->f)) {
      if (*word_head == NULL) {
        *word_head = create_node(argv[i]);
      } else {
        add_elem_node(*word_head, argv[i]);
      }
      m_word = 1;
    } else if (!search_flags(argv[i])) {
      if (*file_head == NULL) {
        *file_head = create_node(argv[i]);
      } else {
        add_elem_node(*file_head, argv[i]);
      }
    }
  }
}

list_node *create_node(char *str) {
  list_node *word_node = malloc(sizeof(list_node));
  if (word_node == NULL) {
    exit(1);
  }
  strcpy(word_node->name, str);
  word_node->next = NULL;
  return word_node;
}

void free_node(list_node *head) {
  while (head) {
    list_node *temp = head;
    head = head->next;
    free(temp);
  }
}

void add_elem_node(list_node *head, char *word_str) {
  while (head) {
    if (!head->next) {
      head->next = create_node(word_str);
      break;
    } else {
      head = head->next;
    }
  }
}

void add_word_f(char *file_n, list_node **word_head) {
  FILE *file = fopen(file_n, "r");
  if (!file) {
    printf("grep: %s: %s\n", file_n, strerror(errno));
    exit(1);
  }
  char buff[BUFSIZ];
  while (fgets(buff, sizeof(buff), file)) {
    int len = strlen(buff);
    if (buff[len - 1] == '\n' && len != 1) {
      buff[len - 1] = 0;
    }
    if (*word_head == NULL) {
      *word_head = create_node(buff);
    } else {
      add_elem_node(*word_head, buff);
    }
  }
  fclose(file);
}

void watching_file(list_node *files, list_node *words, grepFlag *flags) {
  int file_numb = count_file(files);
  while (files) {
    FILE *file = fopen(files->name, "r+");
    if (!file) {
      if (!flags->s) {
        printf("grep: %s: %s\n", files->name, strerror(errno));  //
      }
    } else {
      if (flags->o && !flags->c && !flags->l && !flags->v) {
        file_process_o(file, words, flags, file_numb, files->name);
      } else {
        file_process(file, words, flags, file_numb, files->name);
      }
      fclose(file);
    }
    files = files->next;
  }
}

int count_file(list_node *file_head) {
  int result = 0;
  while (file_head) {
    result++;
    file_head = file_head->next;
  }
  return result;
}

void file_process(FILE *file, list_node *words, grepFlag *flags, int file_numb,
                  char *file_name) {
  char buff[BUFSIZ];
  int l_counter = 0, m_count = 0, n_m_count = 0, n_l_print = 0;
  while (fgets(buff, sizeof(buff), file)) {
    l_counter++;
    int find = find_word_str(buff, words, flags);
    int len = strlen(buff);
    if (buff[len - 1] != '\n') {
      strcpy(&buff[len], "\n");
    }
    if (flags->c && find) {
      m_count++;
    } else {
      m_count += find;
    }
    if (find == 0) {
      n_m_count++;
    }
    if (search_l_flag(flags, find, &n_l_print)) {
      break;
    }
    if (((find && !flags->v) || (!find && flags->v)) && !flags->c) {
      if (file_numb > 1 && !flags->h) {
        printf("%s:", file_name);
      }
      if (flags->n) {
        printf("%d:", l_counter);
      }
      printf("%s", buff);
    }
  }
  search_c_flag(flags, m_count, n_m_count, file_numb, file_name);
  if (n_l_print) {
    printf("%s\n", file_name);
  }
}

int find_word_str(char *str, list_node *words, grepFlag *flags) {
  int return_value = 0, find, regcomp_res;
  regex_t regex;
  while (words) {
    if (flags->i) {
      regcomp_res = regcomp(&regex, words->name, REG_ICASE);
    } else {
      regcomp_res = regcomp(&regex, words->name, 0);
    }
    if (regcomp_res != 0) {
      words = words->next;
      regfree(&regex);
      continue;
    }
    find = regexec(&regex, str, 0, NULL, 0);
    if (find == 0) {
      return_value++;
    }
    regfree(&regex);
    words = words->next;
  }
  return return_value;
}

void file_process_o(FILE *file, list_node *words, grepFlag *flags,
                    int file_numb, char *file_name) {
  char buff[BUFSIZ];
  int line_numb = 0;
  while (fgets(buff, sizeof(buff), file)) {
    line_numb++;
    int line_print = 0;
    search_o_pattern(buff, words, file_name, file_numb, flags, line_numb,
                     &line_print);
  }
}

void trim_n(char *line) {
  if (strlen(line) == 1 && line[0] == '\n') {
    line[0] = '.';
  }
  if (line[strlen(line) - 1] == '\n') {
    line[strlen(line) - 1] = '\0';
  }
}

int make_reg_compare(regex_t *regex, grepFlag *flags, char *keyword) {
  int compare_val;
  if (flags->i) {
    compare_val = regcomp(regex, keyword, REG_ICASE);
  } else {
    compare_val = regcomp(regex, keyword, 0);
  }
  return compare_val;
}

void print_o_match(size_t offset, regmatch_t match, char *buff, grepFlag *flags,
                   int line_numb, int *line_print, int f_count,
                   char *file_name) {
  if (!flags->c && !flags->l) {
    if (f_count > 1 && !flags->h) {
      printf("%s:", file_name);
    }
    if (flags->n && !(*line_print)) {
      printf("%d:", line_numb);
      *line_print = 1;
    }
  }
  for (size_t i = offset + match.rm_so; i < offset + match.rm_eo; i++) {
    printf("%c", buff[i]);
  }
  printf("\n");
}

void search_o_pattern(char *str, list_node *words, char *file_name, int f_count,
                      grepFlag *flags, int line_numb, int *line_print) {
  regex_t regex;
  trim_n(str);
  while (words) {
    int compare_res = make_reg_compare(&regex, flags, words->name);
    if (!compare_res) {
      regmatch_t match;
      size_t offset = 0, len = strlen(str);
      int e_flag = 0;
      while (regexec(&regex, str + offset, 1, &match, e_flag) == 0) {
        e_flag = REG_NOTBOL;
        print_o_match(offset, match, str, flags, line_numb, line_print, f_count,
                      file_name);
        offset += match.rm_eo;
        if (match.rm_so == match.rm_eo) {
          offset++;
        }
        if (offset > len) {
          break;
        }
      }
    }
    regfree(&regex);
    words = words->next;
  }
}
