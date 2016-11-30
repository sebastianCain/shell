#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <fcntl.h>

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
    //if there are extra spaces, we set the memory there to 0
    //then we ove back one step, there by allowing us to write over the slot
    if(*(local_command[k]) == 0){
      //local_command[k]=0;
      k--;
    }
    k++;
  }
  local_command[k] = 0;
  
  printf("--------\n");
}

//returns the loacation of a ceratin charechter,
int find(char * haystack[], char * needle){
  int ans=0;
  while(haystack[ans]){
    if(strcmp(haystack[ans], needle) == 0){
	return ans;
    }
    ans++;
  }
  return -1;//was not found;
}

//puts vals from s into d , ending before position n,
int repackage(char * dest[], char * src[], int n){
  int x=0;
  while(src[x] && x<n){
    dest[x]=src[x];
    x++;
  }
  return 0;
}

int std_out(char * command[], int pos){
  int status;
  int fd = open(command[pos+1], O_CREAT | O_WRONLY, 0644);
  
  //the shuffle//	  
  int new_home = dup(1);
  dup2(fd, 1);
  
  command[pos]=0;

  int nother_fork;//needed to return the process table back to normal
  nother_fork = fork();

  if(nother_fork == 0){
    execvp(command[0], command);
  }
  else{
    wait(&status);
    int b = dup2(new_home, 1); 
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
	
	int pos;
	pos=find(local_command, ">");
	if(pos != -1){// if ">" is in loc_comm
	  std_out(local_command, pos);
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
