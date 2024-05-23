#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1)
    cat(argc, argv);
  else
    printf("cat: flags empty and file...");
  return 0;
}

void cat(int argc, char *argv[]) {
  singleflag sflag = {0, 0, 0, 0, 0, 0, 0, 0};
  int error = 0;
  int i = 1;
  for (; i < argc; i++) {
    if (strspn(argv[i], "-") == 0) break;
    if (checkingFlags(argv[i], &sflag) == 1) error = 1;
  }
  if (sflag.b) sflag.n = 0;
  if (!error && i == argc) {
    printf("cat: please input filename...");
  }
  for (; !error && i < argc; i++) {
    FILE *file_name = fopen(argv[i], "r");
    if (file_name) {
      int start_line = 1;
      int count_empty_line = 1;
      int count_all_line = 1;
      char previous_char = '\n';
      char next_char = ' ';
      char ch[1024] = {'\0'};
      while ((*ch = fgetc(file_name)) != EOF) {
        if (sflag.s && use_flag_s(previous_char, next_char, *ch)) continue;
        if (sflag.n) start_line = use_flag_n(start_line, &count_all_line, *ch);
        if (sflag.b) use_flag_b(previous_char, &count_empty_line, *ch);
        if (sflag.E) use_flag_E(*ch);
        if (sflag.v) use_flag_v(ch);
        next_char = previous_char;
        previous_char = *ch;
        if (sflag.T && use_flag_T(*ch)) continue;
        if (*ch == '\0')
          fputc(*ch, stdout);
        else
          fputs(ch, stdout);
        memset(ch, '\0', 1024);
      }
      fclose(file_name);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
    }
  }
  if (error)
    fprintf(stderr,
            "cat: illegal option -- '%s'.\nTry 'man cat' for more information.",
            argv[i - 1]);
}

int use_flag_b(char previous_char, int *count_empty_line, char ch) {
  if (previous_char == '\n' && ch != '\n')
    printf("%6d\t", (*count_empty_line)++);
  return ch == '\n';
}

int use_flag_v(char *ch) {
  unsigned char tmp = *ch;
  if ((tmp <= 8) || (tmp >= 11 && tmp <= 31) || (tmp >= 127 && tmp <= 159))
    strcpy(ch, symbols_ASCII[tmp]);
  return tmp;
}

int use_flag_E(char ch) {
  if (ch == '\n') printf("$");
  return ch == '\n';
}

int use_flag_n(int start_line, int *count_all_line, char ch) {
  if (start_line) printf("%6d\t", (*count_all_line)++);
  return ch == '\n';
}

int use_flag_s(char previous_char, char next_char, char ch) {
  return ch == '\n' && previous_char == '\n' && next_char == '\n';
}

int use_flag_T(char ch) {
  int tab = 0;
  if (ch == '\t') tab = printf("^I");
  return tab;
}

int checkingFlags(char *argv, singleflag *sflag) {
  int error = 0;
  if (strlen(argv) == 1 || strlen(argv) != strspn(argv, "-bvEnsTet")) error = 1;
  if (!strcmp(argv, "--number"))
    error = 0, sflag->n = 1;
  else if (!strcmp(argv, "--number-nonblank")) {
    error = 0;
    sflag->b = 1;
  } else if (!strcmp(argv, "--squeeze-blank")) {
    error = 0;
    sflag->s = 1;
  } else {
    if (strchr(argv, 'b')) sflag->b = 1;
    if (strchr(argv, 'e')) sflag->v = 1, sflag->E = 1;
    if (strchr(argv, 'E')) sflag->E = 1;
    if (strchr(argv, 'v')) sflag->v = 1;
    if (strchr(argv, 'n')) sflag->n = 1;
    if (strchr(argv, 's')) sflag->s = 1;
    if (strchr(argv, 't')) {
      sflag->v = 1;
      sflag->T = 1;
    }
    if (strchr(argv, 'T')) sflag->T = 1;
  }
  return error;
}
