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
  int rank, i, size, N, tag, origin, destination;
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

  int message_out;
  int message_in;
  tag = 99;

  timestamp_type time1, time2;
  get_timestamp(&time1);

  /*Send integer 0 to process 0*/
  if(rank == 0)
    {
      message_out = 0;
      destination = rank + 1;
      MPI_Send(&message_out, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
      printf("rank %d hosted on %s starts with the message %d\n", rank, hostname, message_out);
    }

  for (i = 0; i<N; i++){
  /*Send message in loops*/
  if(rank == 0)
  {
    destination = rank + 1;
    origin = size -1;
  }
  else if (rank == size -1)
  {
    destination = 0;
    origin = rank - 1;
  }
  else
  {
    destination = rank + 1;
    origin = rank - 1;
  }

  MPI_Recv(&message_in,  1, MPI_INT, origin, tag, MPI_COMM_WORLD, &status);
  message_out = message_in + rank;
  MPI_Send(&message_out, 1, MPI_INT, destination, tag, MPI_COMM_WORLD);
  /*
  printf("rank %d hosted on %s received from %d the message %d\n", rank, hostname, origin, message_in);
  */
  }

  MPI_Finalize();

  if (rank == 0)
  {
    printf("The last message from rank 0 is %d \n", message_out);
  }

  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  printf("Time elapsed for %d loops for rank %d is %f seconds.\n", N, rank, elapsed);

  return 0;
}
