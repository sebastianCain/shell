#include <string.h>

char * promptString();
int split_semis(char dest[], char * command[]);
void split_spaces(char dest[], char * local_command[]);
int find(char * haystack[], char * needle);
int repackage(char * dest[], char * src[], int n);
