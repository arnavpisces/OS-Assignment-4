//Arnav Kumar - 2016017
//Vaasu Gupta - 2016275
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define null NULL
#define true 1
#define false 0
#define read 2
#define write 3
static int incre=1;
struct node{
  sem_t mutex;
  // int which;
  int locktype;
  int value;
  struct node* next;
};

struct queue{
  struct node *front, *rear;
};

struct node* newNode(int v){
  struct node *temp=(struct node*)malloc(sizeof(struct node));
  sem_init(&(temp->mutex),0,1);
  // sem_wait(&(temp->mutex));

  incre++;
  temp->value=v;
  temp->next=null;
  // sem_post(&(temp->mutex));
  return temp;
}

struct queue *createQueue(){
  struct queue *q=(struct queue*)malloc(sizeof(struct queue));
  q->front=q->rear=null;
  return q;
}

void enQueue(struct queue *q, int k){

  struct node *temp=newNode(k);

  printf(CYAN "Trying to Acquire EnQueue LOCK" RESET "\n");
  sem_wait(&(temp->mutex));
  temp->locktype=write;
  printf(CYAN "Acquired EnQueue LOCK" RESET "\n");
  if(q->rear==null){
    q->front=q->rear=temp;
    printf(MAGENTA "Value of %d was EnQueued" RESET "\n", temp->value);
    sleep(2);
    printf(CYAN "Releasing EnQueue LOCK" RESET "\n");
    sem_post(&(temp->mutex));
    return;
  }
  q->rear->next=temp;
  q->rear=temp;
  printf(MAGENTA "Value of %d was EnQueued" RESET "\n", temp->value);
  sleep(2);
  temp->locktype=false;
  printf(CYAN "Releasing EnQueue LOCK" RESET "\n");
  sem_post(&(temp->mutex));

}

struct node *deQueue(struct queue *q){
  // printf("BITCH\n", );
  if(q->front==null){
    return null;
  }
  struct node *temp=q->front;
  printf(CYAN "Trying to Acquire DeQueue LOCK" RESET "\n");
  sem_wait(&(temp->mutex));
  temp->locktype=read;
  printf(CYAN "Acquired DeQueue LOCK" RESET "\n");
  // find++;
  printf(GREEN "Value %d was DeQueued" RESET "\n" ,temp->value);
  q->front=q->front->next;

  // if(q->front==null){
  //   q->rear=null;
  // }
  sleep(1);
  printf(CYAN "Releasing DeQueue LOCK" RESET "\n");
  temp->locktype=false;
  sem_post(&(temp->mutex));

  return temp;
}

void* writerThread(void *var){
  struct queue *q=(struct queue*)var;
  static int counter=10;
  while(true){
    enQueue(q, counter);
    counter+=10;
    // enQueue(q, 20);
    // deQueue(q);
    // deQueue(q);
    // enQueue(q, 30);
    // enQueue(q, 40);
    // enQueue(q, 50);
    // deQueue(q);
    // struct node *n=deQueue(q);
    // if(n!=null)
    // printf("dequeue item is %d\n",n->value );
    // printf("WRITER\n");

}
  return null;
}


void readQueue(struct node* n){
  pid_t x = syscall(__NR_gettid);
  printf(YELLOW "(%d) : Trying to Acquire Read LOCK" RESET "\n",x);
  if(n->locktype==read){ //This is basically if another reader comes to read the element while one reader already has the lock
    printf(YELLOW "(%d) : Read LOCK Acquired" RESET "\n",x);
    printf("(%d) : The Value is %d\n", x, n->value);
    printf(YELLOW "(%d) : Read LOCK Released" RESET "\n",x);
  }
  sem_wait(&(n->mutex));
  n->locktype=read;
  printf(YELLOW "(%d) : Read LOCK Acquired" RESET "\n",x);
  printf("(%d) : The Value is %d\n", x, n->value);
  sleep(4);
  printf(YELLOW "(%d) : Read LOCK Released" RESET "\n",x);
  n->locktype=false;
  sem_post(&(n->mutex));
}

void* readerThread(void *var){

  struct queue *q=(struct queue*)var;
  while(true){
    struct queue *temp=(struct queue*)q;
    if(temp->front!=null){
    do{

      readQueue(temp->front);
      // if(temp->front->next==null)break;
      temp->front=temp->front->next;
      if(incre%4==0){
        deQueue((struct queue*)var);
        // continue;
      }

    }while(temp->front!=null);
    }
  }
}

int main(int argc, char const *argv[]) {

  struct queue *q=createQueue();

  printf("Creating Writer thread\n");
  pthread_t writer;
  pthread_t reader;
  pthread_t reader1;
  pthread_create(&writer,NULL,writerThread,(void *)q);
  pthread_create(&reader,NULL,readerThread,(void *)q);
  pthread_create(&reader1,NULL,readerThread,(void *)q);
  pthread_join(writer,null);
  pthread_join(reader,null);
  pthread_join(reader1,null);
  // sem_destroy(&mutex);
  printf("After Thread\n" );
  exit(0);

  return 0;
}
