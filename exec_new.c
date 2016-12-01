#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#include "stringext.h"

//To comile use the gcc -c <filenm> comamnd to get .o files then combine those


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
  } else {
    wait(&status);
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
  } else {
    wait(&status);
    int b = dup2(new_home, 0); 
    close(fd);
  }
  return 0;
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
    int status;
    
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

	//the stdout case
	int pos;
	pos=find(local_command, ">");
	if(pos != -1){// if ">" is in loc_comm
	  std_out(local_command, pos);
	  return 0;
	}

	//the stdin case
	pos=find(local_command, "<");
	if(pos != -1){// if ">" is in loc_comm
	  std_in(local_command, pos);
	  //THIS WILL USE THE SAME METHOS AS STD_OUT (SEE ABOVE)

	  return 0;
	}

	pos = find(local_command, "|");
	if(pos != -1){

	  int status;
	  int fd = open("no_touch", O_WRONLY | O_CREAT | O_RDONLY, 0644);
	  
	  //the shuffle//	  
	  int new_home = dup(1);
	  dup2(fd, 1);
	  
	  //local_command[pos]=0;

	  char * hold [20];

	  int r=0;
	  while(r<pos && local_command[r]){
	    hold[r]=local_command[r];
	    r++;
	  }
	  hold[r]=0;
	  
	  int a_fork;//needed to return the process table back to normal
	  a_fork = fork();
	  
	  if(a_fork == 0){
	    execvp(hold[0], hold);
	  } else {
	    wait(&status);
	    close(fd);
	    int b = dup2(new_home, 1);
	  }

	  //part 2
	  
	  fd = open("no_touch", O_RDONLY, 0644);

	  
	  int a=0;
	  while(local_command[pos+a+1]){
	    hold[a]=local_command[pos+a+1];
	    a++;
	  }
	  hold[a]=0;
	  
	  //Now hold only holds the stuff after the |, 
	  //close(fd);
	  
	  int new_home_again = dup(0);
	  dup2(fd, 0);
	  
	  int b_fork;
	  b_fork=fork();
	  
	  if(b_fork == 0){
	    
	    execvp(hold[0], hold);
	  }
	  else{
	    wait(&status);
	    
	    dup2(new_home_again, 0);
	    close(fd);

	  }
	  
	  return 0;
	    
	}
	
	//all other cases
	execvp(local_command[0], local_command);
      }
      else{
	wait(&status);
	//wait(NULL);
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
