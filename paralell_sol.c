#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
     
void sum1b(); //sum 1 to 1 billion
void sum2b(); //sum 1 billion to 2 billion
void sum3b(); //sum 1 to 1 billion
void sum4b(); //sum 1 billion to 2 billion

int main(){
  int status;
  pid_t pid1 = fork();

  //timer
  struct timeval start, end;
  long mtime, seconds, useconds;  
  gettimeofday(&start, NULL); //timer
   
  if(pid1 < 0){         //fork failed
    fprintf(stderr, "Fork Failed!");
    return 1;
  }else if(pid1 == 0){  //child process
    pid_t pid2 = fork();
    if(pid2==0){
        pid_t pid3 = fork();
        if(pid3==0){
            sum4b();
        }else{
            sum3b();
            wait(NULL);
        }
    }else{
      sum2b();
      wait(NULL);
    }
  }else{               //parent process
    sum1b();
    wait(NULL);
	
	gettimeofday(&end, NULL); //timer
    //timer
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("Elapsed time: %ld milliseconds on pid=%d\n", mtime, pid1); 
  }

  return 0;
}

void sum1b(){
  long sum =0;
  for(int i=1;i<500000000;i++){
    sum += i;
  }
  printf("The sum of 1 to 0.5b is: %ld\n", sum);
}

void sum2b(){
  long sum =0;
  for(int i=500000000;i<1000000000;i++){
    sum += i;
  }
  printf("The sum of 0.5b to 1b is: %ld\n", sum);
}
void sum3b(){
  long sum =0;
  for(int i=1000000000;i<1500000000;i++){
    sum += i;
  }
  printf("The sum of 1 to 1.5b is: %ld\n", sum);
}

void sum4b(){
  long sum =0;
  for(int i=1500000000;i<2000000000;i++){
    sum += i;
  }
  printf("The sum of 1.5b to 2b is: %ld\n", sum);
}