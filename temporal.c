////////////////////////////////////////////////////////////////////////////////
//temporal.c
//
//Drew Logan, Kelvin Reston
//
//CS345
//OPERATING SYSTEMS
//
//VM Project - Temporal Locality Simulation
//10/13/15
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void temporal(int *referenceList, int *cache, int *index, int cacheSize, int size);
void updateCache(int *cache, int cacheSize, int *page);
int randint(int n);

int main()
{
  srand(time(NULL));

  int size, refCount = 0;

  printf("How long would you like to make the reference string: ");
  scanf("%d", &size);
  //printf("New Size: %d\n", size);

  int cacheSize = randint(5) + 4;
  int referenceList[size];
  int cache[cacheSize];
  int simCounter[size];
  int index = 0;
  int pageNum;

  for(int i = 0; i < size; i++)
  {
    referenceList[i] = 0;
    simCounter[i] = 0;
  }
  for(int i = 0; i < cacheSize; i++)
  {
    cache[i] = 0;
  }

  //Loop while reference list is not full
  while(index < size)
  {
    //N% chance of showing temporal starting at index
    if(((rand() % 50) <= 1 && index >= cacheSize))
    {
      //Adds a marker where temporal simulation begins
      simCounter[index] = 1;
      //begin temporal locality simulation at index
      temporal(referenceList, cache, &index, cacheSize, size);
      //Adds a marker where temporal simulation ends
      simCounter[index-1] = 1;
    }
    //otherwise generate random page number
    else
    {
      //random number in range [1,100]
      pageNum = 1 + (rand() / (RAND_MAX / (100 - 1 + 1) + 1));
      //add this referenced page to reference list
      referenceList[index] = pageNum;
      //Update cache with most recent page reference
      updateCache(cache, cacheSize, &pageNum);
      //increment index of reference list
      index++;
    }
  }
  /*
  //Display the final reference list in list form
  printf("Reference List: \n\n");
  for(int i = 0; i < size; i++)
  {
    printf("Index[%d]: %d\n", i, referenceList[i]);
  }
  */

  FILE *f = fopen("bRef.txt", "w");
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
      printf("E*");
      fprintf(f, "%d, ", referenceList[i]);
      printf("%d, ", referenceList[i]);
      refCount++;
    }
    else if(simCounter[i] == 1 && refCount % 2 == 0)
    {
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

//Function to simulate spatial locality through the next 10-12 indices 
void temporal(int *referenceList, int *cache, int *index, int cacheSize, int size)
{
  int tempSize;

  //50% chance to simulate temporal locality over small region of time
  if(randint(100) < 50)
  {
    //Region of time set between 2% and 4% of reference list
    tempSize = randint((int)(size/25)) + (int)(size/50);
  }
  //50% chance to simulate temporal locality over large region of time
  else
  {
    //Region of time set between 12% and 18% of reference list
    tempSize = randint((int)(size*9/50)) + (int)(size*3/25);
  }

  int tempIndex = *index;
  if((tempIndex + tempSize) > size - 1)
  {
    tempSize = size - 1 - tempIndex;
  }
  //Set position in reference list to current index
  referenceList += tempIndex - 1;
  //Set current page from reference list
  int newPage = *referenceList;

  int num, temp;
  for(int i = 0; i < tempSize; i++)
  {
    for(int j = 0; j < cacheSize; j++)
    {
      num = (rand() / (RAND_MAX / (9+j)+1)+1);
      if(num < 5)
      {
        newPage = cache[j];

        if(j == 0)
          break;
        else
        {
          for(int k = j; k > 0; k--)
          {
            temp = cache[k];
            cache[k] = cache[k-1];
            cache[k-1] = temp;
          }
          break;
        }
      }
    }

    *referenceList = newPage;
    referenceList++;
    tempIndex++;
  }

  *index = tempIndex - 1;

  return;
}

void updateCache(int *cache, int cacheSize, int *page)
{
  int cacheStart = (int)cache;
  int tempPage = *page;
  int temp1 = *(cache);
  int temp2;

  //Add most recently referenced page to cache and append old cache to end
  for(int i = 1; i < cacheSize; i++)
  {
    temp2 = *(cache+i);
    *(cache+i) = temp1;
    temp1 = temp2;
  }

  *cache = tempPage;

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

