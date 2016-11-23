#include <unistd.h>
#include <stdio.h>
#include <string.h>
//#include "exec.h"
//#include "redir.h"
//#include "pipes.h"

void showPrompt() {
  char prompt[100];

  char username[20];
  getlogin_r(username, 20);
  char hostname[20];
  gethostname(hostname, 20);
  char cwd[60];
  getcwd(cwd, 60);

  strcat(prompt, username);
  strcat(prompt, "@");
  strcat(prompt, hostname);
  strcat(prompt, ":");
  strcat(prompt, cwd);
  strcat(prompt, "$ ");
  printf("%s", prompt);
}

int main() {
  showPrompt();
  return 0;
}
