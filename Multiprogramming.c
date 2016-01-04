////////////////////////////////////////////////////////////////////////////////
//Multiprogramming.c
//
//Drew Logan, Kelvin Reston
//
//CS345
//OPERATING SYSTEMS
//
//VM Project - Multiprogramming Simulation with Round Robin Scheduler
//10/27/15
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

// Process struct containing all information on a process
struct Process 
{
    int numPages;
    int referenceList[500];
    int index;
    int faults; 
    int timeSpent;
    int done;
};

void FIFO(struct Process ProcessArray[], int pageFrames, int numOfProcess);

int main()
{
  struct Process ProcessArray[8];
  int numOfProcess = 8; //defines total number of process
  int pageMax; // pages allocad for shared memory resource
  int temp;
  
  printf("How many pages would you like to allocate for shared memory resource: ");
  scanf("%d", &pageMax);
  
  //First Process

  FILE *aRef;
  aRef = fopen("aRef.txt", "r");

  fscanf(aRef, "%d, ", &temp);
  ProcessArray[0].numPages = temp;

  for(int i = 0; i < ProcessArray[0].numPages; i++)
  {
    fscanf(aRef, "%d, ", &temp);
    ProcessArray[0].referenceList[i] = temp;
  }

  ProcessArray[0].index = 0;
  ProcessArray[0].faults = 0;
  ProcessArray[0].timeSpent = 0;
  ProcessArray[0].done = 0;

  fclose(aRef);
  
  //Second Process
  
  FILE *bRef;
  bRef = fopen("bRef.txt", "r");

  fscanf(bRef, "%d, ", &temp);
  ProcessArray[1].numPages = temp;

  for(int i = 0; i < ProcessArray[1].numPages; i++)
  {
    fscanf(bRef, "%d, ", &temp);
    ProcessArray[1].referenceList[i] = temp;
  }

  ProcessArray[1].index = 0;
  ProcessArray[1].faults = 0;
  ProcessArray[1].timeSpent = 0;
  ProcessArray[1].done = 0;

  fclose(bRef);
  
  //Third Process
  
  FILE *cRef;
  cRef = fopen("cRef.txt", "r");

  fscanf(cRef, "%d, ", &temp);
  ProcessArray[2].numPages = temp;

  for(int i = 0; i < ProcessArray[2].numPages; i++)
  {
    fscanf(cRef, "%d, ", &temp);
    ProcessArray[2].referenceList[i] = temp;
  }

  ProcessArray[2].index = 0;
  ProcessArray[2].faults = 0;
  ProcessArray[2].timeSpent = 0;
  ProcessArray[2].done = 0;

  fclose(cRef);

  //Fourth Process
  
  FILE *dRef;
  dRef = fopen("dRef.txt", "r");

  fscanf(dRef, "%d, ", &temp);
  ProcessArray[3].numPages = temp;

  for(int i = 0; i < ProcessArray[3].numPages; i++)
  {
    fscanf(dRef, "%d, ", &temp);
    ProcessArray[3].referenceList[i] = temp;
  }

  ProcessArray[3].index = 0;
  ProcessArray[3].faults = 0;
  ProcessArray[3].timeSpent = 0;
  ProcessArray[3].done = 0;

  fclose(dRef);

  //Fifth Process

  FILE *aRef2;
  aRef2 = fopen("aRef2.txt", "r");

  fscanf(aRef2, "%d, ", &temp);
  ProcessArray[4].numPages = temp;

  for(int i = 0; i < ProcessArray[4].numPages; i++)
  {
    fscanf(aRef2, "%d, ", &temp);
    ProcessArray[4].referenceList[i] = temp;
  }

  ProcessArray[4].index = 0;
  ProcessArray[4].faults = 0;
  ProcessArray[4].timeSpent = 0;
  ProcessArray[4].done = 0;

  fclose(aRef2);
  
  //Sixth Process
  
  FILE *bRef2;
  bRef2 = fopen("bRef2.txt", "r");

  fscanf(bRef2, "%d, ", &temp);
  ProcessArray[5].numPages = temp;

  for(int i = 0; i < ProcessArray[5].numPages; i++)
  {
    fscanf(bRef2, "%d, ", &temp);
    ProcessArray[5].referenceList[i] = temp;
  }

  ProcessArray[5].index = 0;
  ProcessArray[5].faults = 0;
  ProcessArray[5].timeSpent = 0;
  ProcessArray[5].done = 0;

  fclose(bRef2);
  
  //Seventh Process
  
  FILE *cRef2;
  cRef2 = fopen("cRef2.txt", "r");

  fscanf(cRef2, "%d, ", &temp);
  ProcessArray[6].numPages = temp;

  for(int i = 0; i < ProcessArray[6].numPages; i++)
  {
    fscanf(cRef2, "%d, ", &temp);
    ProcessArray[6].referenceList[i] = temp;
  }

  ProcessArray[6].index = 0;
  ProcessArray[6].faults = 0;
  ProcessArray[6].timeSpent = 0;
  ProcessArray[6].done = 0;

  fclose(cRef2);

  //Eighth Process
  
  FILE *dRef2;
  dRef2 = fopen("dRef2.txt", "r");

  fscanf(dRef2, "%d, ", &temp);
  ProcessArray[7].numPages = temp;

  for(int i = 0; i < ProcessArray[7].numPages; i++)
  {
    fscanf(dRef2, "%d, ", &temp);
    ProcessArray[7].referenceList[i] = temp;
  }

  ProcessArray[7].index = 0;
  ProcessArray[7].faults = 0;
  ProcessArray[7].timeSpent = 0;
  ProcessArray[7].done = 0;

  fclose(dRef2);
  
  FIFO(ProcessArray, pageMax, numOfProcess);

  for(int i = 0; i < numOfProcess; i++)
  {
    printf("Size of Process %d: %d\n", i+1, ProcessArray[i].numPages);
    printf("Page faults for Process %d: %d\n", i+1, ProcessArray[i].faults);
    printf("Time Spent on Process %d: %d\n\n", i+1, ProcessArray[i].timeSpent);
  }

  return  0;
}


void FIFO(struct Process  ProcessArray[], int pageFrames, int numOfProcess)
{ 
  
  int processNum = 1; // defines current process
  int finished = 0; // All processes finished indicator
  int clock = 0; // scedualer clock
  int frame[pageFrames];
  int pageHit;
  int j = 0; // frame index
  int avail = 0; //page avalibility indicator
  
  for(int i=0;i<pageFrames;i++)
  {
    frame[i]= 0;
  }
  
  while(finished != 1)
  { 
    
    for(int i = 0; i < 10; i++)//Time slice start
    {
        if( ProcessArray[processNum-1].index > ProcessArray[processNum-1].numPages-1)//Check if process has terminated
        {
            ProcessArray[processNum-1].done = 1;//Turn indicator on to state that process has finished
            break; //Break from loop switch process 
        }   
        
        if(ProcessArray[processNum-1].index <= ProcessArray[processNum-1].numPages-1)// increment time spent on process
        {
            //Increment time spent on process
            for(int i = 0; i < numOfProcess; i++)
            {
              if(ProcessArray[i].done == 0)
              {
                ProcessArray[i].timeSpent++; 
              }
            }
        }
        
        //Start FIFO algorithm
        avail=0;
        
        for(int k=0; k < pageFrames; k++)
        {
            if(frame[k] == ProcessArray[processNum-1].referenceList[ProcessArray[processNum-1].index])
            {
                avail=1;
            }
        }
        
        if(avail==0)
        {
            frame[j]=ProcessArray[processNum-1].referenceList[ProcessArray[processNum-1].index];
            j=(j+1)%pageFrames;
            ProcessArray[processNum-1].faults++;
        }
        // end FIFO algorithm
        
        ProcessArray[processNum-1].index++;
        clock++;
        
    }//End of time slice
    
    //Switch to first process if on last process
    if(processNum == numOfProcess)
    {
        processNum = 1;
    }
    
    //else switch go to next process in Process Struct array
    else
    {
        processNum = processNum + 1;
    }

    // check to see if all processes are completed
    for(int i = 0; i < numOfProcess; i++)
    {
      if(ProcessArray[i].done == 0)
      {
        finished = 0;
        break;
      }
      else
      {
        finished = 1;
      }
    }
  }
  
  //output total time spent to complete all processes 
  printf("\nTotal clock time for all processes: %d\n\n", clock);
  
}