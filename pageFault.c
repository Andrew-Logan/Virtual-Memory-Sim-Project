////////////////////////////////////////////////////////////////////////////////
//pageFault.c
//
//Drew Logan, Kelvin Reston
//
//CS345
//OPERATING SYSTEMS
//
//VM Project - FIFO, LRU, and Second Chance Page Replacement Simulation
//10/22/15
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int FIFO(int *referenceList, int size, int pageMax);
int trueLRU(int *referenceList, int size, int pageMax);
int secondChance(int *referenceList, int size, int pageMax);
int randint(int n);

int main()
{
  srand(time(NULL));

  int size;
  int pageNum;
  int pageMax;

  printf("How many pages would you like to allocate to this process: ");
  scanf("%d", &pageMax);
  //printf("Number of allocated pages: %d\n\n", pageMax);

  printf("How long would you like to make the reference string: ");
  scanf("%d", &size);
  printf("\n");

  int referenceList[size];

  //Loop while reference list is not full
  for(int i = 0; i < size; i++)
  {
    pageNum = randint(100) + 1;
    referenceList[i] = pageNum;
  }

  int fifoCount = FIFO(referenceList, size, pageMax);
  int lruCount = trueLRU(referenceList, size, pageMax);
  int secondCount = secondChance(referenceList, size, pageMax);

  printf("FIFO: Number of page faults is: %d\n\n",fifoCount);
  printf("True LRU: Number of page faults is: %d\n\n",lruCount);
  printf("Second Chance: Number of page faults is: %d\n\n",secondCount);

  return  0;
}

int FIFO(int *referenceList, int size, int pageMax)
{ 
  int i,j,a[size],frame[pageMax],k,avail,count=0;
  
  for(i=1;i<=size;i++)
  {
    a[i] = *referenceList;
    referenceList++;
  }
  
  for(i=0;i<pageMax;i++)
  {
    frame[i]= -1;
  }
  j=0;

  //printf("------------------------------------------------\n");
  //printf("Ref String\t Page Frames\n");
  //printf("------------------------------------------------\n");

  for(i=1;i<=size;i++)
  {
    //printf("%d\t\t ",a[i]);
    avail=0;
    for(k=0;k<pageMax;k++)
    {
      if(frame[k]==a[i])
        avail=1;
    }
    if(avail==0)
    {
      frame[j]=a[i];
      j=(j+1)%pageMax;
      count++;
      //for(k=0;k<pageMax;k++)
        //printf("%d\t",frame[k]);
      }
      //printf("\n");
  } 

  //printf("\n");

  return count;
}

int trueLRU(int *referenceList, int size, int pageMax)
{
  int frame[pageMax],a[size],count=0,count1,d,i,j,k=0,r,t,b[pageMax],count2[pageMax];
  for(i=0;i<=size;i++)
  {
    a[i] = *referenceList;
    referenceList++;
  }
  frame[k]=a[k];

  //printf("------------------------------------------------\n");
  //printf("Ref String\t Page Frame\n");
  //printf("------------------------------------------------\n");
  //printf("%d\t\t %d\n", frame[k], frame[k]);

  count++;
  k++;

  for(i=1;i<size;i++)
  {
    //printf("%d\t ",a[i]);
    count1=0;
    for(j=0;j<pageMax;j++)
    {
      if(a[i]!=frame[j])
      {
        count1++;
      }
    }
    if(count1==pageMax)
    {
      count++;
      if(k<pageMax)
      {
        frame[k]=a[i];
        k++;
        //for(j=0;j<k;j++)
        //{
         //printf("\t %d",frame[j]);
        //}
      }
      else
      {
        for(r=0;r<pageMax;r++)
        {
          count2[r]=0;
          for(j=i-1;j<size;j--)
          {
            if(frame[r]!=a[j])
            {
              count2[r]++;
            }
            else
            {
              break;
            }
          }
        }
        for(r=0;r<pageMax;r++)
        {
          b[r]=count2[r];
        }
        for(r=0;r<pageMax;r++)
        {
          for(j=r;j<pageMax;j++)
          {
            if(b[r]<b[j])
            {
              t=b[r];
              b[r]=b[j];
              b[j]=t;
            }
          }
        }
        for(r=0;r<pageMax;r++)
        {
          if(count2[r]==b[0])
          {
            frame[r]=a[i];
          }
          //printf("\t %d ",frame[r]);
        }
      }
    }
    //printf("\n");
  }

  return count;
}

int secondChance(int *referenceList, int size, int pageMax)
{
  int i,j,x,a[size],frame[2][pageMax],k,avail,count=0;
  
  for(i=1;i<=size;i++)
  {
    a[i] = *referenceList;
    referenceList++;
  }
  for(i=0;i<pageMax;i++)
  {
    frame[0][i]= -1;
  }
  for(i=0;i<pageMax;i++)
  {
    frame[1][i]= 0;
  }

  j=0;
  //printf("------------------------------------------------\n");
  //printf("Ref String\t Page Frames|Second Chance Bit\n");
  //printf("------------------------------------------------\n");
  for(i=1;i<=size;i++)
  {
    //printf("%d\t\t ",a[i]);
    avail=0;
    for(k=0;k<pageMax;k++)
      if(frame[0][k]==a[i])
      {
        avail=1;
        frame[1][k] = 1;
      }
    if(avail == 0)
    {
      x = j;
      do
      {
        if(frame[1][j] == 1)
        {
           frame[1][j] = 0;
           j = (j+1)%pageMax;
        }
        else if (frame[1][j] == 0)
        {
          frame[0][j]=a[i];
          frame[1][j] = 0;
          j=(j+1)%pageMax;
          count++;
          //for(k=0;k<pageMax;k++)
            //printf("%d|%d\t",frame[0][k], frame[1][k]);
          break;
        }
      }while(x != j);
    }
    //printf("\n");
  }
  //printf("\n");

  return count;
}

/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 */
int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    long end = RAND_MAX / n; // truncate skew
    //assert(end > 0L);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}



