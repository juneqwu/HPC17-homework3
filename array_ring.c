/* Ring Communication:
 * Send messages in a circle with respect to mpirank
 * 0 <-> 1, 2 <-> 3, ....
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include <unistd.h>
#include <mpi.h>

int main( int argc, char *argv[])
{
  int rank, i,j, size, N, tag, origin, destination;
  MPI_Status status;

  char hostname[1024];
  gethostname(hostname, 1024);

  if (argc != 2){
   N = 1;
  }
  else{
    N = atol(argv[1]);
  }


  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  tag = 99;

  int L = 31250;
  double *message = malloc(L* sizeof(double));

  srand(12874);
  for (i = 0; i < L; i++){
    message[i] = rand();
  }
  
  timestamp_type time1, time2;
  get_timestamp(&time1);

 
  printf("rank %d hosted on %s starts with the message\n", rank, hostname);
  
  for (i = 0; i<= N; i++){
  /*Send message in loops*/
  if(rank == 0)
  {
    if (i == 0){
      destination = rank + 1;
      MPI_Send(message, L, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);
    }
    if (i == N){
      origin = size -1;
      MPI_Recv(message, L, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, &status);
    }
    else{
      destination = rank + 1;
      origin = size -1;
      MPI_Recv(message, L, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, &status);
      MPI_Send(message, L, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);
    }
  }
  else if (rank == size -1)
  {
    destination = 0;
    origin = rank - 1;
    MPI_Recv(message, L, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, &status);
    MPI_Send(message, L, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);

  }
  else
  {
    destination = rank + 1;
    origin = rank - 1;
    MPI_Recv(message, L, MPI_DOUBLE, origin, tag, MPI_COMM_WORLD, &status);
    MPI_Send(message, L, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);
  }

  /*
  printf("rank %d hosted on %s received from %d the message %d\n", rank, hostname, origin, message);
  */
  }

  free(message);

  MPI_Finalize();
  
  if (0 == rank){
    get_timestamp(&time2);
    double elapsed = timestamp_diff_in_seconds(time1,time2);
    printf("Time elapsed for %d loops for rank %d is %f seconds.\n", N, rank, elapsed);
  }
  return 0;
}
