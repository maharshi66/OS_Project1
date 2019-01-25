// Programmer: Maharshi Shah    September 9, 2018
// Operating Systems COP 4600
// Shared Memory - Project 1     
// U13381853                   

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define SHMKEY ((key_t) 2700)   

typedef struct {
  int value; 
} shared_mem;

shared_mem *total;                               //shared variable for processes

void process1()
{
    int i = 0;
    while (i < 100000)                          //Increases value of total by 1 100,000 times
    {
        i++;
        total->value = total->value + 1;
    }
    printf ("\nFrom Process 1:counter %d\n", total->value);
}

void process2()
{
    int i = 0;
    while (i < 200000)                          //Increases value of total by 1 200,000 times
    {
        i++;
        total->value = total->value + 1;
    }

    printf ("\nFrom Process 2:counter %d\n", total->value);
}


void process3()
{
    int i = 0;
    while (i < 300000)                          //Increases value of total by 1 300000 times
    {
        i++;
        total->value = total->value + 1;
    }
    printf ("\nFrom Process 3:counter %d\n", total->value);
}

void process4()
{
    int i = 0;
    while (i < 500000)                          //Increases value of total by 1 500,000 times
    {
        i++;
        total->value = total->value + 1;
    }
    printf ("\nFrom Process 4:counter %d\n", total->value);
}


int main(void)
{
    //required variable declaration
    int shmid,ID, status, pid1, pid2, pid3,pid4;		
    char *shmadd;
    shmadd = (char *) 0;

//Shared memory segment connection

    if ((shmid = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    if ((total = (shared_mem *) shmat(shmid, shmadd, 0)) == (shared_mem*) - 1)
    {
        perror("shmat");
        exit(0);
    }

    total->value = 0;

// First Child creation and initiation
    if ((pid1 = fork()) == 0)                               
    {
        process1();                                         
    }

//Second Child process creation and initiation
    if ((pid1 != 0) && (pid2 = fork()) == 0)               
    {
        process2();                                         
    }

//Third Child process creation and initiation
    if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork()) == 0) 
    {
        process3();        
    }

//Fourth Child process creation and initiation   
    if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 = fork()) == 0) 
    {
        process4();        
    }

    //Loop until child processes finish
    for(;(ID = wait(&status)) != -1;)                       
    {
        printf("Child with ID: %d has just exited\n", ID);     
    }

    //Detaching shared memory 
    if(shmdt (total) == -1){
        perror("shmdt");
        exit(-1);
    }
    
    //Removing shared memory
    if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 != 0))
    {
        if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
        {
            perror("shmctl");
            exit(-1);
        }
    //After all children have finished, print result
        printf("\n\tEnd of Simulation\n");                    
    }

    return 0;
}