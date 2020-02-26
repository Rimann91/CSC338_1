#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 64
     
int sum1(); //sum 1 to 1 billion
int sum2(); //sum 1 billion to 2 billion
int sum3(); //sum 1 billion to 2 billion
int sum4(); //sum 1 billion to 2 billion
void putmem(long sum, int pos, int shmid);

int main(){

  long total;
  long* results;

  int status;
  pid_t pid;
  pid_t pid2;
  pid_t pid3;

  key_t key;
  int shmid;

  //timer
  struct timeval start, end;
  long mtime, seconds, useconds;  

  /*make the key*/
  if((key = ftok("paralell.c",'R')) == -1){
    perror("shmget");
    exit(1);
  }

  /*create the segment*/
  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1){
    perror("shmget");
    exit(1);
  }


  pid = fork();

  gettimeofday(&start, NULL); //timer
 
  if(pid < 0){         //fork failed
    fprintf(stderr, "Fork Failed!");
    return 1;
  }else if(pid == 0){  //child process
    pid2 = fork();

    if(pid2 < 0){         //fork failed
        fprintf(stderr, "Fork Failed!");
        return 1;
      }
      else if(pid2 == 0){  //child process
        pid3 = fork();

        if(pid3 < 0){         //fork failed
            fprintf(stderr, "Fork Failed!");
            return 1;
          }
          else if(pid3 == 0){  //child process
            putmem(sum1(), 1,shmid);
          }
          else{               //parent process
            putmem(sum2(),2,shmid);
            wait(NULL);
          }

      }
      else{               //parent process
        putmem(sum3(),3,shmid);
        wait(NULL);
      }

  }else{               //parent process
    putmem(sum4(),4,shmid);
    wait(NULL);
  }

  gettimeofday(&end, NULL); //timer
  
  if(pid > 0){
    //timer
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("Elapsed time: %ld milliseconds on pid=%d\n", mtime, pid); 
    printf("value at register 1 is: %ld \n", *results);
    printf("value at register 2 is: %ld \n", *(results+8));
  }


  //results = shmat(shmid, (void *)0, 0);
  //total = *results + (*results+8) + (*results+16) + (*results+24);

  //printf("Total sum of 1-2billion is: %ld\n",total);
  return 0;
}

int sum1(){
  long sum =0;
  for(int i=1;i<500000000;i++){
    sum += i;
  }
  printf("The sum of 1 to 0.5b is: %ld\n", sum);
  return sum;
}

int sum2(){
  long sum =0;
  for(int i=500000000;i<1000000000;i++){
    sum += i;
  }
  printf("The sum of 0.5b to 1b is: %ld\n", sum);
  return sum;
}

int sum3(){
  long sum =0;
  for(int i=1000000000; i<1500000000;i++){
    sum += i;
  }
  printf("The sum of 1b to 1.5b is: %ld\n", sum);
  return sum;
}

int sum4(){
  long sum =0;
  for(int i=1500000000; i<2000000000;i++){
    sum += i;
  }
  printf("The sum of 1.5b to 2b is: %ld\n", sum);
  return sum;
}

void putmem(long sum, int pos, int shmid){
  long* data;
  long* container;


  /* attach to the segment to get a pointer to it: */
  data = shmat(shmid, (void *)0, 0);
  container = data+(pos*8);
  if (data == (long *)(-1)) {
      perror("shmat");
      exit(1);
  }

  /* read or modify the segment*/
  printf("writing to segment: \"%ld\"\n", sum);
  //memcpy(data, sum, 64);
  container = &sum;


    /* detach from the segment: */
  if (shmdt(data) == -1) {
      perror("shmdt");
      exit(1);
  }
}
