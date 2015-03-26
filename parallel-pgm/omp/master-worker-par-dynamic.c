/* 
 *  Generic example of master/worker program.  "Tasks" just consist of
 *  sleeping for a specified time; times to wait are generatedly
 *  randomly from 1 to a specified maximum.
 *
 *  Command-line arguments:  total tasks, maximum task time,
 *    (optional) "--verbose" for verbose output
 * 
 *  Parallel version using OpenMP and dynamic assignment of tasks to workers.
 */

/* ugly nonportable hack to make millisleep() function compile with c99 */
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "timer.h"
#include "cmdline.h"
#include "master-worker-functions.h"

int main(int argc, char *argv[]) {

  int num_tasks;
  int max_task_time;
  double start_time, end_time;
  int task_time;
  int total_task_time = 0;
  int max_thread_total_task_time;
  int min_thread_total_task_time;
  bool verbose = false;
  int nthreads;

  char* usage_msg = "usage is %s num_tasks max_task_time [--verbose]\n";

  num_tasks = get_integer_arg(argc, argv, 1, 1, "num_tasks", usage_msg);
  max_task_time = get_integer_arg(argc, argv, 2, 1, "max_task_time", usage_msg);
  int required_parms = 3;
  if (argc > required_parms) {
      if (strcmp(argv[required_parms], "--verbose") == 0) 
          verbose = true;
      else {
          fprintf(stderr, usage_msg, argv[0]);
          return EXIT_FAILURE;
      }
  }

  max_thread_total_task_time = 0;
  min_thread_total_task_time = max_task_time * num_tasks;

  /* could use omp_get_wtime but get_time is what sequential code uses */
  start_time = get_time();

  #pragma omp parallel private(task_time)
  {
      int thread_total_task_time = 0;
      int thread_num_tasks = 0;
      int thread_ID = omp_get_thread_num();

      #pragma omp single
      {
          nthreads = omp_get_num_threads();
      }
      #pragma omp for schedule(dynamic)
      for (int i = 0; i < num_tasks; ++i) {

          #pragma omp critical
          {
              task_time = random_in_range(1, max_task_time);
          }
          thread_total_task_time += task_time;
          ++thread_num_tasks;
          if (verbose)
              printf("(thread %d) task time = %d\n", thread_ID, task_time);
          millisleep(task_time);
      }
      printf("thread %d number of tasks = %d, total time = %d\n", thread_ID,
              thread_num_tasks, thread_total_task_time);
      #pragma omp critical
      {
          if (thread_total_task_time > max_thread_total_task_time)
              max_thread_total_task_time = thread_total_task_time;
          if (thread_total_task_time < min_thread_total_task_time)
              min_thread_total_task_time = thread_total_task_time;
          total_task_time += thread_total_task_time;
      }
  }

  end_time = get_time();

  printf("\nOpenMP parallel version with %d threads, dynamic scheduling\n", 
          nthreads);
  printf("number of tasks = %d\n", num_tasks);
  printf("max task time = %d\n", max_task_time);
  printf("total task time = %d\n", total_task_time);
  printf("total task time in threads ranges from %d to %d\n",
          min_thread_total_task_time, max_thread_total_task_time);
  printf("running time = %g\n", end_time - start_time);

  return EXIT_SUCCESS;
}
