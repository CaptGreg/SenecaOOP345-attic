/* 
 *  Generic example of master/worker program.  "Tasks" just consist of sleeping
 *  for a specified time; times to wait are generatedly randomly from 1 to a
 *  specified maximum.
 *
 *  Command-line arguments:  total tasks, maximum task time,
 *    (optional) "--verbose" for verbose output
 *
 *  Parallel version using MPI and dynamic assignment of tasks to workers.
 */

/* ugly nonportable hack to make millisleep() function compile with c99 */
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "timer.h"
#include "cmdline.h"
#include "utility.h"
#include "master-worker-functions.h"

int main(int argc, char *argv[]) {

  int num_tasks;
  int max_task_time;

  double start_time, end_time;

  int task_time;
  int total_task_time = 0;
  int max_process_total_task_time;
  int min_process_total_task_time;
  int process_total_task_time = 0;
  int process_num_tasks = 0;
  bool verbose = false;
  int nprocs;
  int process_ID;
  MPI_Status status;

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
      fprintf(stderr, "MPI initialization error\n"); 
      exit(EXIT_FAILURE);
  }

  char* usage_msg = "usage is %s num_tasks max_task_time [--verbose]\n";
  num_tasks = get_integer_arg_extended(argc, argv, 1, 1, "num_tasks", 
          usage_msg, true, (void (*)(void))MPI_Finalize);
  max_task_time = get_integer_arg_extended(argc, argv, 2, 1, "max_task_time",
          usage_msg, true, (void (*)(void))MPI_Finalize);
  int required_parms = 3;
  if (argc > required_parms) {
      if (strcmp(argv[required_parms], "--verbose") == 0) 
          verbose = true;
      else {
          fprintf(stderr, usage_msg, argv[0]);
          error_exit(NULL);
      }
  }

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &process_ID);

  if (nprocs < 2) {
      error_exit("number of processes must be at least 2\n");
  }

  MPI_Barrier(MPI_COMM_WORLD);
  /* could use MPI_Wtime but get_time is what sequential code uses */
  start_time = get_time();

  if (process_ID == 0) {
      for (int i = 0; i < num_tasks; ++i) {

          task_time = random_in_range(1, max_task_time);
          total_task_time += task_time;

          MPI_Recv(0, 0, MPI_INT, MPI_ANY_SOURCE, 0, 
                  MPI_COMM_WORLD, &status);

          MPI_Send(&task_time, 1, MPI_INT, status.MPI_SOURCE, 0, 
                  MPI_COMM_WORLD);
      }
      task_time = -1;
      for (int i = 1; i < nprocs; ++i) {
          MPI_Send(&task_time, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      }
  }
  else {

      task_time = 0;
      while (task_time >= 0) {

          MPI_Send(0, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);

          MPI_Recv(&task_time, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

          if (task_time > 0) {
              process_total_task_time += task_time;
              ++process_num_tasks;
              if (verbose)
                  printf("(process %d) task time = %d\n", process_ID, task_time);
              millisleep(task_time);
          }
      }
      printf("process %d number of tasks = %d, total time = %d\n", process_ID,
              process_num_tasks, process_total_task_time);
  }

  MPI_Reduce(&process_total_task_time, &max_process_total_task_time, 1,
          MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  if (process_ID == 0) {
      process_total_task_time = max_process_total_task_time;
  }
  MPI_Reduce(&process_total_task_time, &min_process_total_task_time, 1,
          MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  end_time = get_time();

  if (process_ID == 0) {
      printf("\nMPI parallel version with %d worker processes"
              ", dynamic assignment of tasks\n", 
              nprocs-1);
      printf("number of tasks = %d\n", num_tasks);
      printf("max task time = %d\n", max_task_time);
      printf("total task time = %d\n", total_task_time);
      printf("total task time in processes ranges from %d to %d\n",
              min_process_total_task_time, max_process_total_task_time);
      printf("running time = %g\n", end_time - start_time);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
