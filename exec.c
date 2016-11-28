#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int exec() {

  char dest[20];
  while (1) {
  printf("type: ");
  fgets(dest, 20, stdin);
  
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
  
  //command has all its info
  
  int f;
  f = fork();
  
  //the child--here the program is run
  if (f==0){
    if(strcmp(command[0],"exit") == 0){
      //return 0;
      //need to kill parent too
      //break;
      int ppid=getppid();
      //printf("%d\n", ppid);
      kill(ppid, 9);
    }
    execvp(command[0], command);
    return 0;
  }
  
  //the parent 
  else{
    sleep(1);
    printf("debug\n");
  }
  }
  return 0;
}


//Testing///

int main(){
  //printf("type: ");
  exec();
}

