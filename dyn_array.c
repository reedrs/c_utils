#include "dyn_array.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Initialize a NAME object
void
CONCAT(init, NAME)(NAME* new_arr)
{
  const size_t initSize = 2;
  new_arr->d = malloc(initSize * sizeof(DTYPE));
  new_arr->allocated = initSize;
  new_arr->used = 0;
}

// Free all memory allocated in a NAME
void
CONCAT(dealloc, NAME)(NAME* arr)
{
  #ifdef C_STRING_MODE
  size_t i;

  for (i = 0; i < arr->used; i++)
  {
    free(arr->d[i]);
  }
  #endif

  free(arr->d);
}

// Double size of allocated memory
void
CONCAT(_grow, NAME)(NAME* arr)
{
  arr->d = realloc(arr->d, arr->allocated*2*sizeof(DTYPE));
  arr->allocated *= 2;
}

// Add a new element to the end of a NAME array
void
CONCAT(push, NAME)(NAME* arr, DTYPE new_element)
{
  if (!(arr->used < arr->allocated))
  {
    CONCAT(_grow, NAME)(arr);
  }

  arr->d[arr->used] = new_element;
  arr->used++;
}

// Find the first occurence of search in arr, optionally return its index to an argument
int
CONCAT(contains, NAME)(NAME* arr, DTYPE search, size_t *idx_ret)
{
  for (size_t i = 0; i < arr->used; i++)
  {
    #ifdef C_STRING_MODE
    if (strcmp(search, arr->d[i]) == 0)
    #else
    if (search == arr->d[i])
    #endif
    {
      if (idx_ret)
      {
        *idx_ret = i;
      }

      return 1;
    }
  }
  
  return 0;
}

// Remove all elements of a NAME array at an index greater than or equal to idx
void
CONCAT(truncate, NAME)(NAME* arr, size_t idx)
{
  #ifdef C_STRING_MODE
  size_t i;

  for (i = idx; i < arr->used; i++)
  {
    free(arr->d[i]);
    arr->d[i] = NULL;
  }
  #endif

  arr->used = idx;
}

// Remove one element from a NAME array, shifting over other elements to fill its place
void
CONCAT(removeIdx, NAME)(NAME* arr, size_t idx)
{
  size_t i;

  for (i = idx; i < arr->used - 1; i++)
  {
    arr->d[i] = arr->d[i + 1];
  }

  CONCAT(truncate, NAME)(arr, arr->used - 1);
}

// Remove all elements in a NAME array with elements equal to element, shifting over
// other elements to fill their places.
void
CONCAT(removeVals, NAME)(NAME* arr, DTYPE element)
{
  size_t i;

  while (CONCAT(contains, NAME)(arr, element, &i))
  {
    CONCAT(removeIdx, NAME)(arr, i);
  }
}
