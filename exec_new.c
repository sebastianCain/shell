#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int split_semis(char dest[], char * command[]){
  char *p = dest;
    
  int i=0;
  while(p){
    command[i]=strsep(&p, ";");
    i++;
  }
  command[i]=0;

  return i;
}

void split_spaces(char dest[], char * local_command[]){
  char *s=dest;
  int k = 0;
  while (s) {
    local_command[k] = strsep(&s, " ");
    k++;
  }
  local_command[k] = 0;
}

int exec(){
  while(1){
    printf("write here:");
    
    char dest[100];
    fgets(dest, 100, stdin);
    char *newline = strstr(dest, "\n");
    *newline = 0;
    
    char * command[30];//an array of 10 ptrs
    int size_comm=split_semis(dest, command);
    
    int j=0;
    while(command[j]){
      char * local_command[20];
      split_spaces(command[j], local_command);
      
      int f;
      f=fork();
      if(f==0){//child
	if(strcmp(local_command[0],"exit") == 0){
	  int ppid = getppid();
	  kill(ppid, 9);// kill the parent
	  return 0;//kill itself
	}
	if(strcmp(local_command[0], "cd") == 0){//this is dealt in parent
	  return 0;
	}
	execvp(local_command[0], local_command);
      }
      else{
	wait(NULL);
	if((strcmp(local_command[0], "cd") == 0) && (local_command[1] != 0)){
	  chdir(local_command[1]);
	}
      }
      j++;
    }
  }
  return 0;
}

int main(){
  exec();
}
