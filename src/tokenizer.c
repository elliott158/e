#include <stdio.h>
#include <string.h>

void split(char str[], char delim[]) {
  char* token;
  token = strtok(str, delim);
  while (token) {
    printf("%s\n", token);
    token = strtok(NULL, delim);
  }
}

int main() {
  char str[] = "hello hello";
  char delim[] = " ";
  split(str, delim);
  return 0;
}
