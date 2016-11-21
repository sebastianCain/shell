#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {

  printf("run a program here --> ");

  char dest[20];
  //printf("before while");
  while (fgets(dest, 20, stdin)) {
    //printf("%s", dest);
    char *newline = strstr(dest, "\n");
    *newline = 0;
    char *s = dest;
    char *command[20];
    int i = 0;
    while (s) {
      command[i] = strsep(&s, " ");
      i++;
    }
    command[i] = 0;
    //printf("command[0]: %s\n", command[0]);
    //printf("command[1]: %s\n", command[1]);
    execvp(command[0], command);
  }

  return 0;
}
