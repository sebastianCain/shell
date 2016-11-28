#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

char * promptString() {
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
  return prompt;
}

//to split funcs w semicolons
char*[] splitCommands() {
  char commands*[20] = malloc(1000);
  while(1) {
    if(strstr(dest, ";")){
      hold=strsep(&p, ";");
      commands[i] = hold;
    } else {
      break;
    }
}

char*[] commandArrayFromString() {
  
}

int prompt() {

  char dest[20];
  printf("%s", promptString());
  fgets(dest, 20, stdin);
  char *newline = strstr(dest, "\n");
  *newline = 0;

  char *hold;
  char *p = dest;
  int runs = 1;
  int was_affected = 0;

    
    else{
      if(was_affected){
        hold=strsep(&p, ";");
        printf("p%sp\n", p);
      }
      else{
        hold=dest;
      }
    }
      
    printf("[%s]\n", hold);//debuging
      
    char *s = hold;
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
	
      if(strcmp(command[0],"cd")==0){//cd cause
        return 0;//handeled in the main
      }
	
      execvp(command[0], command);
      return 0;
    }
      
    //the parent 
    else{
      if(strcmp(command[0],"cd")==0){//cd cause
        execvp(command[0], command);
      }
      sleep(1);
      printf("debug\n");
    }
    runs--;
    }
  return 0;
}


