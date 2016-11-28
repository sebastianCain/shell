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
char*[] splitCommands(char *p) {
  char commands*[20] = malloc(1000);
  int i = 0;
  while(p) {
    commands[i] = strsep(&p, ";");
    i++;
  }
  commands[i] = 0;
  return commands;
}

char*[] commandArrayFromString(char *p) {
  char commands*[20] = malloc(1000);
  int i = 0;
  while(p) {
    commands[i] = strsep(&p, " ");
    i++;
  }
  commands[i] = 0;
  return commands;
}

char *strip(char[] p) {
  char final[20];
  int i = 0;
  while(p[i] == " ") {
    p = p[i+1];
    i++;
  }
  int j = 20;
  while(p[j] == 0 || p[j] == " ") {
    p[j] = 0;
    j--;
  }
  return final;
}
