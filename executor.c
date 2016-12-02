#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "stringext.h"

//To compile without make use the gcc -c <filenm> comamnd to get .o files then combine those


int std_out(char * command[], int pos){
  int status;
  int fd = open(command[pos+1], O_CREAT | O_WRONLY, 0644);
  
  //the shuffle//	  
  int new_home = dup(1);
  dup2(fd, 1);
  
  command[pos]=0;

  int nother_fork;//needed to return the process table back to normal
  nother_fork = fork();

  if (nother_fork == 0){
    execvp(command[0], command);
  }
  else {
    //wait(&status);
    wait(&nother_fork);
    int b = dup2(new_home, 1); 
    close(fd);
  }
  return 0;
}

int std_in(char * command[], int pos){
  int status;
  int fd = open(command[pos+1], O_RDONLY, 0644);
  
  //the shuffle//	  
  int new_home = dup(0);
  dup2(fd, 0);
  
  command[pos]=0;

  int nother_fork;//needed to return the process table back to normal
  nother_fork = fork();

  if(nother_fork == 0){
    execvp(command[0], command);
  }
  else {
    //wait(&status);
    wait(&nother_fork);
    int b = dup2(new_home, 0); 
    close(fd);
  }
  return 0;
}

void mykill() {
  int ppid = getppid();
  kill(ppid, 9); // kill the parent
}

int mypipe(char *command[], int pos) {
  //hold is where we store the part of local_command after the |
  char * hold[20];
  int e = 0;
  while(command[e+pos+1]){
    hold[e] = command[pos+1];
    e++;
  }
  hold[e]=0;
  
  int fdx[2];//making the array
  pipe(fdx);//the pipe
  
  int hold_in = dup(0);//back ups
  int hold_out = dup(1);//back ups	  
  
  dup2(1, hold_in);//redirect out to in
  
  int f;
  f = fork();
  
  if(f == 0){
    command[pos] = 0;//cleave it off before the |
    
    close(fdx[0]);//closes the read side
    close(1);//close the output so it doenst leave
    
    dup2(fdx[1], 1);//puting fd[1] in place of 1
    
    execvp(command[0], command);//the exec
    return 0;
  } else {
    wait(&f);
    close(fdx[1]);//close the write side
    close(0);//close STD_IN
    dup2(fdx[0], 0);//switch em
    execvp(hold[0], hold);
  }
  
  dup2(hold_in, 0);
  dup2(hold_out, 1);
  
  return 0;
}

int exec() {
  while(1) {
    char *prompt = promptString();
    printf("%s", prompt);
    free(prompt);
    
    char dest[100];
    fgets(dest, 100, stdin);
    char *newline = strstr(dest, "\n");
    *newline = 0;
    
    char * command[30];//an array of 10 ptrs
    int size_comm=split_semis(dest, command);
    
    int j=0;
    int status;
    
    while(command[j]){
      char * local_command[20];
      split_spaces(command[j], local_command);
      
      printf("about to check for nothingness\n");
      printf("loc_com[0]: %s\n", local_command[0]);
      printf("about to check for nothingness pt 2\n");

      
      
      int f;
      f = fork();
      if (f == 0) {//child

        if (local_command[0] == NULL) {
          //either spaces or nothing
          return 0;
        }
        
        if (strcmp(local_command[0],"exit") == 0){
          mykill();
          return 0; //kill child
        }

        if (strcmp(local_command[0], "cd") == 0){//this is dealt in parent
          return 0;
        }
	
        //the stdout case
        int pos;
        pos = find(local_command, ">");
        if(pos != -1){// if ">" is in local_command
          std_out(local_command, pos);
          return 0;
        }
        
        //the stdin case
        pos = find(local_command, "<");
        if(pos != -1){// if ">" is in local_command
          std_in(local_command, pos);
          return 0;
        }
        
        pos = find(local_command, "|");
        if(pos != -1){
          return mypipe(local_command, pos);
        }
        
        //all other cases
        execvp(local_command[0], local_command);
      }
      else{
        //wait(&status);
        wait(&f);
        if((strcmp(local_command[0], "cd") == 0) && (local_command[1] != 0)){
          chdir(local_command[1]);
        }
      }
      j++;
    }
  }
  return 0;
}
