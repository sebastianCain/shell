#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <string.h>


char * promptString() {
  char *prompt = malloc(400);
  strcat(prompt, "(in_project)");
  
  char username[100];
  getlogin_r(username, 100);
  char hostname[100];
  gethostname(hostname, 100);
  char cwd[100];
  getcwd(cwd, 100);

  strcat(prompt, username);
  strcat(prompt, "@");
  strcat(prompt, hostname);
  strcat(prompt, ":");
  strcat(prompt, cwd);
  strcat(prompt, "$ ");

  //char *globalprompt = malloc(400);
  //*globalprompt = prompt;
  return prompt;
}


int split_semis(char dest[], char * command[]) {
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
