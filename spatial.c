////////////////////////////////////////////////////////////////////////////////
//spatial.c
//
//Drew Logan, Kelvin Reston
//
//CS345
//OPERATING SYSTEMS
//
//VM Project - Spatial Locality Simulation
//10/11/15
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void spatial(int *referenceList, int *index, int size);
int randint(int n);

int main()
{
  srand(time(NULL));

  int size;

  printf("How long would you like to make the reference string: ");
  scanf("%d", &size);
  //printf("New Size: %d\n", size);

  int referenceList[size];
  int simCounter[size];
  int index = 0;
  int refCount = 0;
  int pageNum;

  for(int i = 0; i < size; i++)
  {
    simCounter[i] = 0;
  }

  printf("\n\n");

  //Loop while reference list is not full
  while(index < size)
  {
    //N% chance of showing spatial starting at index
    if(randint(30) <= 1)
    {
      //Adds a marker where spatial simulation begins
      simCounter[index] = 1;
      //begin spatial locality simulation at index
      spatial(referenceList, &index, size);
      //Adds a marker where spatial simulation ends
      simCounter[index-1] = 1;
    }
    //otherwise generate random page number
    else
    {
      //random number in range [1,100]
      pageNum = randint(100) + 1;
      //add this referenced page to reference list
      referenceList[index] = pageNum;
      //increment index of reference list
      index++;
    }
  }

  /*
  //Display the final reference list in list form
  printf("Reference List: \n\n");
  for(int i = 0; i < 250; i++)
  {
    printf("Index[%d]: %d\n", i, referenceList[i]);
  }
  */

  FILE *f = fopen("aRef.txt", "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  //Append size of array to beginning of output file
  fprintf(f, "%d, ", size);

  //Display the final reference list in array form
  printf("Reference List: (");
  
  for(int i = 0; i < size-1; i++)
  {
    if(simCounter[i] == 1 && refCount % 2 == 1)
    {
      //fprintf(f, "E*");
      printf("E*");
      fprintf(f, "%d, ", referenceList[i]);
      printf("%d, ", referenceList[i]);
      refCount++;
    }
    else if(simCounter[i] == 1 && refCount % 2 == 0)
    {
      //fprintf(f, "S*");
      printf("S*");
      fprintf(f, "%d, ", referenceList[i]);
      printf("%d, ", referenceList[i]);
      refCount++;
    }
    else
    {
      fprintf(f, "%d, ", referenceList[i]);
      printf("%d, ", referenceList[i]);
    }
  }
  fprintf(f, "%d", referenceList[size-1]);
  printf("%d)\n\n", referenceList[size-1]);

  fclose(f);

  return  0;
}

void spatial(int *referenceList, int *index, int size)
{
  //Determines the length of indices to simulate through [1/40:Size, 3/40:Size]
  int tempSize = randint((int)(size*3/40)) + (int)(size/40);
  int tempIndex = *index;
  if((tempIndex + tempSize) > size - 1)
  {
    tempSize = size - 1 - tempIndex;
  }
  referenceList += tempIndex - 1;
  int newPage = *referenceList;
  int newPageRange;

  //50% chance to simulate spatial locality over small region of memory
  if(randint(100) < 50)
  {
    //Range in memory for next page is set to a max of 3
    newPageRange = randint(3) + 1;
  }
  //50% chance to simulate spatial locality over large region of memory
  else
  {
    //Range in memory for next page is set between 4 and 6
    newPageRange = randint(3) + 4;
  }

  for(int i = 0; i < tempSize; i++)
  {
    do
    {
      //50% chance that new page is to left of current page
      if(randint(100) < 50)
      {
        newPage = newPage - randint(newPageRange + 1); 
      }
      //50% chance that new page is to right of current page
      else
      {
        newPage = newPage + randint(newPageRange + 1);
      }
    } while((newPage < 1) || (newPage > 100));

    *referenceList = newPage;
    referenceList++;
    tempIndex++;
  }

  *index = tempIndex - 1;

  return;
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

