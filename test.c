#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define clear() printf("\033[H\033[J")
char c=1;
while(1) {
  printf("%c",c);
  fflush(stdout); 
  sleep(1); 
  clear();
  c++;
}
