#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
     
void sum1b(); //sum 1 to 1 billion
void sum2b(); //sum 1 billion to 2 billion

int main(){
  int status;
  pid_t pid;

  //timer
  struct timeval start, end;
  long mtime, seconds, useconds;  

  pid = fork();

  gettimeofday(&start, NULL); //timer
 
  if(pid < 0){         //fork failed
    fprintf(stderr, "Fork Failed!");
    return 1;
  }else if(pid == 0){  //child process
    sum1b();
  }else{               //parent process
    sum2b();
    wait(NULL);
  }

  gettimeofday(&end, NULL); //timer
  
  if(pid > 0){
    //timer
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("Elapsed time: %ld milliseconds on pid=%d\n", mtime, pid); 
  }
  return 0;
}

void sum1b(){
  long sum =0;
  for(int i=1;i<1000000000;i++){
    sum += i;
  }
  printf("The sum of 1 to 1b is: %ld\n", sum);
}

void sum2b(){
  long sum =0;
  for(int i=1000000000;i<2000000000;i++){
    sum += i;
  }
  printf("The sum of 1b to 2b is: %ld\n", sum);
}