#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "mysem.c"

#define MAX_BUF 2
int * buffer;
struct ss{
int    head,tail,item_id;
int    count;
 // 3 semaphores... { Mutex, Empty, Full}
int    fflag,eflag;
}*st;
int    sem_ID;
void init()
{
    st->head=st->tail=st->eflag=st->fflag=st->count=0;
    st->item_id=1;
}
void producer()
{


printf("Producer started\n");

 while(1)
 {
  int v=0;
  sem_change(sem_ID, 0, -1);
  if(st->count==MAX_BUF)
  {
   printf("Producer waiting\n");
   st->fflag=1;
   sem_change(sem_ID, 0, 1);
   sem_change(sem_ID, 2, -1);
   sem_change(sem_ID, 0, -1);    //Waits until a free slot is available
  }

  st->count++;
  buffer[st->head]=st->item_id++;

  if(st->count==1 && st->eflag==1)
  {
  st->eflag=0;
  sem_change(sem_ID, 1, 1);    //To signal consumer thread that buffer is not empty
  }
  printf("produced item:%d \n", buffer[st->head]);
  st->head=(st->head+1)%MAX_BUF;
  sem_change(sem_ID, 0, 1);

  sleep(rand()%3);

 }
}

void consumer()
{
printf("consumer started\n");
 while(1)
 {
  int v=0;
  sem_change(sem_ID, 0, -1);
  if(st->count==0)
  {
   printf("Consumer Waiting\n");
   st->eflag=1;
   sem_change(sem_ID, 0, 1);
   sem_change(sem_ID, 1, -1);
   sem_change(sem_ID, 0, -1);
      //Waits until an item is produced
  }
  st->count--;

  printf("consumed item:%d \n", buffer[st->tail]);
  st->tail=(st->tail+1)%MAX_BUF;
  if(st->count==MAX_BUF-1 && st->fflag==1)
  {
   st->fflag=0;
   sem_change(sem_ID, 2, 1);                //To signal the producer thread that
   }//buffer is not full
   sem_change(sem_ID, 0, 1);
  sleep(rand()%4);
 }
}
key_t key=123;
int shmid;
void seminit()
{
    if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((st = shmat(shmid, NULL, 0)) ==  (char *) -1) {
        perror("shmat");
        exit(1);
    }
    if ((shmid = shmget(key+1, 1, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((buffer = shmat(shmid, NULL, 0)) ==  (char *) -1) {
        perror("shmat");
        exit(1);
    }
}
char *ax,*cc;
int main()
{

seminit();
init();
 buffer=(int*)calloc(MAX_BUF,sizeof(int));
 pthread_t pro,con;
 int values[]={1,0,0};
 int aa=21;
 sem_ID=sem_init_diff_val(3, values); //initialize with 1, 0, 0
    int x=fork();
    if(x==0)
    {
        seminit();
        producer();
    }
    else
    {
    }
    int y=fork();
    if(y==0)
    {

        seminit();
        consumer();
    }
    else
    {
        //wait(NULL);
    }
    int z=fork();
    if(z==0)
    {

        seminit();
        consumer();
    }
    else
    {
        //wait(NULL);
    }
    wait(NULL);
 //pthread_create(&pro, NULL, (void*)&producer, NULL);
 //pthread_create(&con, NULL, (void*)&consumer, NULL);

 //pthread_join(pro,NULL);

 return 0;
}
