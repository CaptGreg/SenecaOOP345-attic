#include <stdio.h>
#include <stdlib.h>  // qsort

int main(int argc, char**argv)
{
  int array2[] = { 4, 3, 1, 2, 5 };
  int array3[] = { 4, 3, 1, 2, 5 };
  // NOTE: nested functions in gcc, not in g++
  // http://gcc.gnu.org/onlinedocs/gcc-4.4.4/gcc/Nested-Functions.html#Nested-Functions
  void dump2 (void)
  {
    int i;
    for (i = 0; i < sizeof (array2) / sizeof (array2[0]); i ++)
      printf ("%d ", array2[i]);
    printf ("\n");
  }
  void dump3 (void)
  {
    int i;
    for (i = 0; i < sizeof (array3) / sizeof (array3[0]); i ++)
      printf ("%d ", array3[i]);
    printf ("\n");
  }


// https://groups.google.com/forum/#!topic/comp.lang.c/_YLcPZfqh_8
#define lambda2(return_type, body_and_args) \
  ({ \
    return_type __fn__ body_and_args \
    __fn__; \
  })

  // Use it like this:
  int (*max)(int, int) = lambda2 (int , (int x, int y) { return x > y ?  x : y; } );
  printf ("max(9,99)=%d\n", max(9,99));


  // Or with qsort:
  printf ("Initial: ");
  dump2 ();
  qsort (array2, 
         sizeof(array2)/sizeof(array2[0]), 
         sizeof (array2[0]), 
         lambda2 (int, (const void * a, const void * b)
             { return *(int*)a - *(int*)b; }) 
  );
  printf ("Sorted: ");
  dump2 ();

  // http://walfield.org/blog/2010/08/25/lambdas-in-c.html
  /* Create a lambda function.  Note: unlike lambdas in functional
     languages, this lambda does not capture the containing
     environment.  Thus, if you access the enclosing environment, you
     must ensure that the lifetime of this lambda is bound by the
     lifetime of the enclosing environment (i.e., until the enclosing
     function returns).  This means that if you access local
     variables, bad things will happen.  If you don't access local
     variables, you're fine.  */

#define lambda3(l_ret_type, l_arguments, l_body)         \
  ({                                                    \
    l_ret_type l_anonymous_functions_name l_arguments   \
      l_body                                            \
    &l_anonymous_functions_name;                        \
  })

  printf ("Initial: ");
  dump3 ();

  /* Ensure that the lambda is a nested function and thus requires a
     trampoline.  */
  int comparison = 0;

  qsort (array3, sizeof (array3) / sizeof (array3[0]), sizeof (array3[0]),
         lambda3 ( int, 
                   (const void *a, const void *b),
                   {
                    dump3 ();
                    printf ("Comparison %d: %d and %d\n",
                            ++ comparison, *(const int *) a, *(const int *) b);
                    return *(const int *) a - *(const int *) b;
                   }
                 )
  );

  printf ("Sorted: ");
  dump3 ();

  return 0;
}
