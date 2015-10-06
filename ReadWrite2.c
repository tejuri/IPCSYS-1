#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include "mysem.c"
#define MAX_C 5
int sem_ID,key=41,shmid;
//char *buff;
int count,*c;
void shminit()
{
    if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((c = (shmat(shmid, NULL, 0))) ==  (char *) -1) {
        perror("shmat");
        exit(1);
    }
}
void read1()
{
    while(1)
    {
        printf("read1\n");
        sem_change(sem_ID,0,-1);
        count++;
        if(count==1)
        sem_change(sem_ID,1,-1);
        sem_change(sem_ID,0,1);
        int x=*c;
        x++;
        *c=x;
        printf("read1ing %d\n",x);
        sem_change(sem_ID,0,-1);
        count--;
        if(count==0)
        sem_change(sem_ID,1,1);
        sem_change(sem_ID,0,1);
        sleep(3+rand()%2);
    }
}
void write1()
{
    while(1)
    {
        printf("Write\n");
        sem_change(sem_ID,1,-1);
        int x=*c;
        x++;
        *c=x;
        printf("Wrote %d\n",x);
        sem_change(sem_ID,1,1);
        sleep(5);
        printf("K");
    }
}
int main()
{
    c=malloc(sizeof(int));
    int f=5,i;
    //printf("%d",f);
    //scanf("%d",&f);
    f=8;
    shminit();
    //printf("l");
    //init();
    sem_ID=sem_init(2, 8); //initialize with 1, 0, 0
    //read1();
    for(i=0;i<5;i++)
    {
        int x=fork();
        if(x==0&&i==0)
        {
            write1();
        }
        else if(x==0)
        {
            shminit();
            read1();
        }
        sleep(rand()%2);
    }
    wait(NULL);
 //pthread_create(&pro, NULL, (void*)&producer, NULL);
 //pthread_create(&con, NULL, (void*)&consumer, NULL);

 //pthread_join(pro,NULL);

 return 0;
 }
