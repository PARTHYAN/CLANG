#include <stdio.h>
#include <stdlib.h>

enum sort {DESCEND, ASCEND};
enum TYPE {CHAR, INT};

int swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
  return 1;
}

void print_list(void *arr, int len, enum TYPE type)
{
  for(int i=0; i<len; i++)
    if (type == CHAR)
      printf("%d ", ((char *)arr)[i]);
    else if (type == INT)
      printf("%d ", ((int *)arr)[i]);
  printf("\n");  
}

int* bsort(int* arr, int len, enum sort order)
{
  int i, res=1;
  int *iorder = (int *)malloc(len * sizeof(int));

  for (i=0; i<len; i++)
    iorder[i] = i;

  while(res)
    for (i=0, res=0; i<len-1; i++)
      if ((order == ASCEND && arr[i] > arr[i+1]) || (order == DESCEND && arr[i] < arr[i+1]))
      {
          res = swap(&arr[i], &arr[i+1]);
          swap(&iorder[i], &iorder[i+1]);
      }
  return iorder;
}

int main(void)
{
  int arr[] = {1, 4, 6, 2, 7, 8, 5, 3, 5};
  int len = sizeof(arr)/sizeof(int);
  enum sort order = ASCEND;

  int *iorder = bsort(arr, len, order);
  print_list(arr, len, INT);
  print_list(iorder, len, INT);

  return 0;
}
