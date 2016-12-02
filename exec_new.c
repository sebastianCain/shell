#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

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
	  //int status;

	  char * hold[20];
	  int e=0;
	  while(local_command[e+pos+1]){
	    hold[e]=local_command[pos+1];
	    e++;
	  }
	  hold[e]=0;
	  
	  int fdx[2];
	  pipe(fdx);

	  int hold_in = dup(0);
	  int hold_out = dup(1);	  

	  dup2(1,hold_in);//redirect out to in
	  //dup2(1, fd[1]);
	  //dup2(0, fd[0]);

	  //printf("[%s]\n", hold[0]);
	  
	  //execvp(hold[0], hold);
	  
	  int f;
	  f=fork();

	  if(f == 0){
	    local_command[pos]=0;//cleave it off before the |
	    printf("one\n");

	    close(fdx[0]);//closes the read side
	    close(1);
	    printf("bump\n");
	    dup2(fdx[1], 1);//puting fd[1] in place of 1

	    execvp(local_command[0], local_command);
	    return 0;
	  }
	  else{
	    wait(&f);
	    printf("heyo\n");
	    close(fdx[1]);
	    close(0);
	    dup2(fdx[0], 0);
	    execvp(hold[0], hold);
	  }

	  
	  
	  return 0;
	  /*
	  int status;

	  char * hold_2[20];
	  int e=0;
	  while(local_command[e+pos+1]){
	    hold_2[e]=local_command[pos+1];
	    e++;
	  }
	  
	  //int fd[2];
	  //pipe(fd);

	  int hold_in = dup(0);
	  int hold_out = dup(1);

	  int child;

	  dup2(1,hold_in);//redirect out to in
	  //dup2(1, fd[1]);
	  //dup2(0, fd[0]);
	  
	  child=fork();

	  if(child == 0){
	    local_command[pos]=0;//cleave it off before the |
	    printf("one\n");
	    execvp(local_command[0], local_command);
	    return 0;
	  }
	  else{
	    wait(&status);
	    char dest_a[100];
	    fgets(dest_a, 100, stdin);
	    char *newline = strstr(dest, "\n");
	    *newline = 0;
	    printf("[%s]\n", dest_a);
	  }

	  child = fork();

	  if(child == 0){
	    printf("two\n");
	    execvp(hold_2[0], hold_2); 
	    return 0;
	  }
	  else{
	    wait(&status);
	    dup2(hold_in, 0);
	    dup2(hold_out, 1);
	    //close(fd[0]);
	    //close(fd[1]);
	    return 0;
	  }
	   	  
	  return 0;
	  */
	  /*
	  int status;

	  //moving the conetents;
	  char * hold_2[20];
	  int e=0;
	  while(local_command[e+pos+1]){
	    hold_2[e]=local_command[pos+1];
	    e++;
	  }

	  int f1;
	  int f2;

	  int hold_in = dup(0);
	  int hold_out = dup(1);

	  int fd;
	  
	  f1=fork();
	  if(f1 != 0){//child doesnt do this
	    f2=fork();
	  }
	  
	  
	  if(f1==0){
	    fd = getppid();
	    dup2(1, fd);//f2);//put f2 in place of 1 or STD_OUT
	    local_command[pos]=0;//cleave it off before the |

	    printf("one\n");
	    
	    execvp(local_command[0], local_command);
	    
	  }
	  else{
	    wait(&status);
	    //return the STD_OUT to its spot
	    printf("what dup\n");
	    dup2(hold_out, 1);
	    
	    if(f2==0){
	      //set the STD_IN to pid f1
	      //execvp(hold_2[0], hold_2);

	      fd = getppid(); 
	      dup2(0,fd);//f1);

	      printf("two\n");
	      printf("[%s]\n", hold_2[0]);
	      
	      execvp(hold_2[0], hold_2);

	    }
	    else{//where the process goes
	      wait(&status);

	      printf("three\n");
	      
	      //return  STD_IN to its place
	      dup2(hold_in, 0);
	      return 0;
	    }
	  }

	  return 0;
	  */
	  /*
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
	  */
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
