#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include "mysem.c"
#define MAX_C 5
int sem_ID;
int customers_count=5;
void phip(int i)
{
    if(i%2==0)
    while(1)
    {
        printf("Thinking %d\n",i);
        sem_change(sem_ID,i,-1);
        printf("A");
        sem_change(sem_ID,(i+1)%5,-1);
        printf("Eating %d\n",i);
        sem_change(sem_ID,(i+1)%5,1);
        sem_change(sem_ID,i,1);
        printf("Sleeping %d\n",i);
        sleep(3);
    }
    else
    while(1)
    {
        printf("Thinking %d\n",i);
        sem_change(sem_ID,(i+1)%5,-1);
        printf("A");
        sem_change(sem_ID,i,-1);
        printf("Eating %d\n",i);
        sleep(rand()%3);
        sem_change(sem_ID,i,1);
        sem_change(sem_ID,(i+1)%5,1);
        printf("Sleeping %d\n",i);
        sleep(rand()%3);
    }
}
int main()
{
    int i,array[]={1,1,1,1,1};
    sem_ID=sem_init(5,1);
    for(i=0;i<5;i++)
    {
        int x=fork();
        if(x==0)
        {
            phip(i);
        }
        sleep(rand()%2);
    }
    wait(NULL);
}
