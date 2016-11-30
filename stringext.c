
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
