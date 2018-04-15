#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
int arr[10]={0};
#define null NULL
#define true 1
#define false 0
sem_t mutex;

void* writerThread(void *var){
  sleep(2);
  sem_wait(&mutex);
  sleep(1);

  arr[1]=123123;
  printf("%d\n",arr[1] );
  // sleep(5);
  sem_post(&mutex);
}

void* readerThread(void *var){
  printf("Trying to acquire lock\n");
  sem_wait(&mutex);
  printf("LOCK ACQUIRED\n" );
  while(true){
  printf("Element is %d\n",arr[0]);
  sleep(2);
}
  // sleep(3);
  sem_post(&mutex);

}


int main(int argc, char const *argv[]) {
  sem_init(&mutex,0,1);
  pthread_t writer;
  pthread_t reader;
  pthread_create(&writer,NULL,writerThread,null);
  pthread_create(&reader,NULL,readerThread,null);
  pthread_join(reader,null);
  pthread_join(writer,null);

   return 0;
 }
