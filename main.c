#include <unistd.h>
#include <stdio.h>
#include <string.h>
//#include "exec.h"
//#include "redir.h"
//#include "pipes.h"

void showPrompt() {
  char prompt[50];
  gethostname(prompt, 50);
  char *cwd;
  getcwd(cwd, 50);
  strcat(prompt, cwd);
  strcat(prompt, "$ ");
  printf("%s", prompt);
}

int main() {
  showPrompt();
}
