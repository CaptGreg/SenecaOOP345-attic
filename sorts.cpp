// http://stackoverflow.com/questions/244252/a-good-reference-card-cheat-sheet-with-the-basic-sort-algorithms-in-c

#include <stdlib.h>
#include <string.h>
#include <memory.h> // malloc

static void swap(int *a, int *b) {
    if (a != b) {
        int c = *a;
        *a = *b;
        *b = c;
    }
}

void bubblesort(int *a, int l) {
    int i, j;

    for (i = l - 2; i >= 0; i--)
        for (j = i; j < l - 1 && a[j] > a[j + 1]; j++)
            swap(a + j, a + j + 1);
}

void selectionsort(int *a, int l) {
    int i, j, k;
    for (i = 0; i < l; i++) {
        for (j = (k = i) + 1; j < l; j++)
            if (a[j] < a[k])
                k = j;
        swap(a + i, a + k);
    }
}

static void hsort_helper(int *a, int i, int l) {
    int j;

    for (j = 2 * i + 1; j < l; i = j, j = 2 * j + 1)
        if (a[i] < a[j])
            if (j + 1 < l && a[j] < a[j + 1])
                swap(a + i, a + ++j);
            else
                swap(a + i, a + j);
        else if (j + 1 < l && a[i] < a[j + 1])
            swap(a + i, a + ++j);
        else
            break;
}

void heapsort(int *a, int l) {
    int i;

    for (i = (l - 2) / 2; i >= 0; i--)
        hsort_helper(a, i, l);

    while (l-- > 0) {
        swap(a, a + l);
        hsort_helper(a, 0, l);
    }
}

static void msort_helper(int *a, int *b, int l) {
    int i, j, k, m;

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

void mergesort(int *a, int l) {
    int *b;

    if (l < 0)
        return;

    b = (int*) malloc(l * sizeof(int));
    memcpy(b, a, l * sizeof(int));
    msort_helper(a, b, l);
    free((void*) b);
}

static int pivot(int *a, int l) {
    int i, j;

    for (i = j = 1; i < l; i++)
        if (a[i] <= a[0])
            swap(a + i, a + j++);

    swap(a, a + j - 1);

    return j;
}

void quicksort(int *a, int l) {
    int m;

    if (l <= 1)
        return;

    m = pivot(a, l);
    quicksort(a, m - 1);
    quicksort(a + m, l - m);
}

struct node {
    int value;
    struct node *left, *right;
};

void btreesort(int *a, int l) {
    int i;
    struct node *root = NULL, **ptr;

    for (i = 0; i < l; i++) {
        for (ptr = &root; *ptr;)
            ptr = a[i] < (*ptr)->value ? &(*ptr)->left : &(*ptr)->right;
        *ptr = (struct node*) malloc(sizeof(struct node));
        **ptr = (struct node){.value = a[i]};
    }

    for (i = 0; i < l; i++) {
        struct node *node;
        for (ptr = &root; (*ptr)->left; ptr = &(*ptr)->left);
        a[i] = (*ptr)->value;
        node = (*ptr)->right;
        free((void*) *ptr);
        (*ptr) = node;
    }
}


int main(int argc, char**argv)
{
  int *a;
  int l;

  bubblesort(a, l);
  selectionsort(a, l);
  heapsort(a, l);
  mergesort(a, l);
  quicksort(a, l);
  btreesort(a, l);
}
