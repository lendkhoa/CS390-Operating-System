#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 1;
int c = 1;  
int main(){
  pid_t pid;
  for (int i = 1; i < 4; i++){
    pid = fork();
    if (pid == 0) {
      value += 1;
      printf("pid == 0\n");
    }else if (pid > 0){
      value --;
      printf("pid > 0\n");
    }
  }
}
