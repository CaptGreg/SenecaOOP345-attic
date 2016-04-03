// best reference:
// https://en.wikibooks.org/wiki/Special:Search/Algorithm_Implementation/Sorting/

// some sorts are also in see Numerical Recipes in C

// GB original code hacked from
// http://stackoverflow.com/questions/244252/a-good-reference-card-cheat-sheet-with-the-basic-sort-algorithms-in-c

// GB ported C-code to C++ function templates, new/delete


#include <cstring>     // memcpy
#include <iostream>
#include <algorithm>   // for std::make_heap, std::sort_heap

using namespace std;

template <typename T>   // function template
static void swap(T* a, T* b) {
    if (a != b) {
        T c = *a;
        *a = *b;
        *b = c;
    }
}

template <typename Iterator>
void heap_sort(Iterator begin, Iterator end)
{
    std::make_heap(begin, end);
    std::sort_heap(begin, end);
}

template <typename T>   // function template
void heapsortCPP(T* a, int l) 
{
  heap_sort(a, a+l);
}

template <typename T>   // function template
void heapsortC(T arr[], unsigned int N) 
{
    // https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Heapsort#C
    // This is a fast implementation of heapsort in C, adapted from Numerical Recipes in C 
    // but designed to be slightly more readable and to index from 0.

    if(N==0) // check if heap is empty
      return;

    T t; /* the temporary value */
    unsigned int n = N, parent = N/2, index, child; /* heap indexes */
    /* loop until array is sorted */
    while (1) { 
        if (parent > 0) { 
            /* first stage - Sorting the heap */
            t = arr[--parent];  /* save old value to t */
        } else {
            /* second stage - Extracting elements in-place */
            n--;                /* make the heap smaller */
            if (n == 0) {
                return; /* When the heap is empty, we are done */
            }
            t = arr[n];         /* save lost heap entry to temporary */
            arr[n] = arr[0];    /* save root entry beyond heap */
        }
        /* insert operation - pushing t down the heap to replace the parent */
        index = parent; /* start at the parent index */
        child = index * 2 + 1; /* get its left child index */
        while (child < n) {
            /* choose the largest child */
            if (child + 1 < n  &&  arr[child + 1] > arr[child]) {
                child++; /* right child exists and is bigger */
            }
            /* is the largest child larger than the entry? */
            if (arr[child] > t) {
                arr[index] = arr[child]; /* overwrite entry with child */
                index = child; /* move index to the child */
                child = index * 2 + 1; /* get the left child and go around again */
            } else {
                break; /* t's place is found */
            }
        }
        /* store the temporary value at its new location */
        arr[index] = t; 
    }
}

template <typename T>   // function template
void bubblesort(T* a, int l) 
{
    int i, j;  // GB cannot be unsigned (or size_t)

    for (i = l - 2; i >= 0; i--)  // GB NOTE >=0 terminate condition
        for (j = i; j < l - 1 && a[j] > a[j + 1]; j++)
            swap(a + j, a + j + 1);
}

template <typename T>   // function template
void selectionsort(T* a, size_t l) 
{
    size_t i, j, k;
    for (i = 0; i < l; i++) {
        for (j = (k = i) + 1; j < l; j++) {
            if (a[j] < a[k])
                k = j;
        }
        swap(a + i, a + k);
    }
}

template <typename T>   // function template
static void hsort_helper(T* a, int i, int l) 
{
    int j;

    for (j = 2 * i + 1; j < l; i = j, j = 2 * j + 1) {
        if (a[i] < a[j]) {
            if (j + 1 < l && a[j] < a[j + 1]) {
                swap(a + i, a + ++j);
            } else {
                swap(a + i, a + j);
            }
        } else if (j + 1 < l && a[i] < a[j + 1]) {
            swap(a + i, a + ++j);
        } else {
            break;
        }
    }
}

template <typename T>   // function template
void heapsort(T* a, int l) 
{ // see Numerical Recipes in C
    int i;

    for (i = (l - 2) / 2; i >= 0; i--)
        hsort_helper(a, i, l);

    while (l-- > 0) {
        swap(a, a + l);
        hsort_helper(a, 0, l);
    }
}

template <typename T>   // function template
static void msort_helper(T* a, T* b, size_t l) 
{
    size_t i, j, k, m;

    switch (l) {
        case 1:
            a[0] = b[0];
        case 0:
            return;
    }

    m = l / 2;
    msort_helper(b, a, m);
    msort_helper(b + m, a + m, l - m);
    for (i = 0, j = 0, k = m; i < l; i++)
        a[i] = b[j < m && !(k < l && b[j] > b[k]) ? j++ : k++];
}

template <typename T>   // function template
void mergesort(T* a, size_t l) 
{
    T *b;

    if (l < 0)
        return;

    // b = (T*) malloc(l * sizeof(T));
    b = new T[l];
    memcpy(b, a, l * sizeof(T));
    msort_helper(a, b, l);
    // free((void*) b);
    delete [] b;
}

template <typename T>   // function template
static int pivot(T* a, size_t l) 
{
    size_t i, j;

    for (i = j = 1; i < l; i++)
        if (a[i] <= a[0])
            swap(a + i, a + j++);

    swap(a, a + j - 1);

    return j;
}

template <typename T>   // function template
void quicksort(T* a, size_t l) 
{
    if (l <= 1)
        return;

    size_t m = pivot(a, l);
    quicksort(a, m - 1);
    quicksort(a + m, l - m);
}

template <typename T>   // function template
void btreesort(T* a, size_t l) 
{
    size_t i;
    struct node {
        T value;
        struct node *left, *right;
    };
    struct node *root = NULL, **ptr;

    for (i = 0; i < l; i++) {
        for (ptr = &root; *ptr;)
            ptr = a[i] < (*ptr)->value ? &(*ptr)->left : &(*ptr)->right;

        *ptr = (struct node*) malloc(sizeof(struct node));
        // *ptr = new node;

        **ptr = (struct node){.value = a[i]};
    }

    for (i = 0; i < l; i++) {
        struct node *node;
        for (ptr = &root; (*ptr)->left; ptr = &(*ptr)->left);
        a[i] = (*ptr)->value;
        node = (*ptr)->right;

        free((void*) *ptr);
        // delete ptr;

        (*ptr) = node;  // GB: Delete node then store something at that address?
        // aborts with new/delete.  Runs with malloc/free.
        // munmap_chunk(): invalid pointer: 0x0000000000f49c98 ***
        // /bin/bash: line 1:  7236 Aborted
    }
}

template <typename T>   // function template
void shellsort(T a[], const int size)// Nothing to do with shells.  Named after D.H. Shell
{
  int i, j, inc;
  T tmp;
 
  inc = 3;
  while (inc > 0) {
    for (i=0; i < size; i++) {
      j = i;
      tmp = a[i];
      while ((j >= inc) && (a[j-inc] > tmp)) {
        a[j] = a[j - inc];
        j = j - inc;
      }
      a[j] = tmp;
    }
    if (inc/2 != 0)
      inc = inc/2;
    else if (inc == 1)
      inc = 0;
    else
      inc = 1;
  }
}

template <typename T>   // function template
void insertionsort(T a[], const int length)
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Insertion_sort
{
    int i, j;
    T value;

    for(i = 1 ; i < length ; i++)
    {
        value = a[i];
        for (j = i - 1; j >= 0 && a[j] > value; j--)
            a[j + 1] = a[j];
        a[j + 1] = value;
    }
}

int main(int argc, char**argv)
{
  int a[] = {42,19,2,66,1,33,8,5,19};
  int l = sizeof(a) / sizeof(a[0]);
  int b[l];
  memcpy(b,a,sizeof(a));

  cout << "input data "; for(auto e: a) cout << e << " "; cout << "\n\n";

  bubblesort(a, l);
  cout << "bubblesort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  selectionsort(a, l);
  cout << "selectionsort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  heapsort(a, l);
  cout << "heapsort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  heapsortC(a, l);
  cout << "heapsortC "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  heapsortCPP(a, l);
  cout << "heapsortCPP "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  mergesort(a, l);
  cout << "mergesort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  quicksort(a, l);
  cout << "quicksort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  btreesort(a, l);
  cout << "btreesort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  shellsort(a, l);
  cout << "shellsort "; for(auto e: a) cout << e << " "; cout << "\n";

  memcpy(a,b,sizeof(a));
  insertionsort(a, l);
  cout << "insertionsort "; for(auto e: a) cout << e << " "; cout << "\n";
}
