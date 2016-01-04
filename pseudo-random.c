////////////////////////////////////////////////////////////////////////////////
//pseudo-random.c
//
//Drew Logan, Kelvin Reston
//
//CS345
//OPERATING SYSTEMS
//
//VM Project - Pseudo-Random Reference String Simulation
//10/19/15
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randint(int n);

int main()
{
  srand(time(NULL));

  int size;

  printf("How long would you like to make the reference string: ");
  scanf("%d", &size);
  //printf("New Size: %d\n", size);

  int referenceList[size];
  int index = 0;
  int pageNum;

  //Loop while reference list is not full
  for(int i = 0; i < size; i++)
  {
    pageNum = randint(100) + 1;
    referenceList[i] = pageNum;
  }

  FILE *f = fopen("dRef.txt", "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  //Append size of array to beginning of output file
  fprintf(f, "%d, ", size);

  printf("Reference List: (");
  for(int i = 0; i < size-1; i++)
  {   
      fprintf(f, "%d, ", referenceList[i]);
      printf("%d, ", referenceList[i]);
  }
  fprintf(f, "%d", referenceList[size-1]);
  printf("%d)\n\n", referenceList[size-1]);

  fclose(f);

  return  0;
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

