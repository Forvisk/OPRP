#include <stdio.h>
#include <mpi.h>
#include <time.h> /* for the work function only */
#include <unistd.h>

int main (int argc, char *argv []) {
       char hostname[257];
       int size, rank;
       int i, pid;
       int bcast_value = 1;

       gethostname (hostname, sizeof hostname);
       MPI_Init (&argc, &argv);
       MPI_Comm_rank (MPI_COMM_WORLD, &rank);
       MPI_Comm_size (MPI_COMM_WORLD, &size);
       if (!rank) {
            bcast_value = 42;
       }
       MPI_Bcast (&bcast_value,1 ,MPI_INT, 0, MPI_COMM_WORLD );
       printf("%s\t- %d - %d - %d\n", hostname, rank, size, bcast_value);
       fflush(stdout);

       MPI_Barrier (MPI_COMM_WORLD);
       MPI_Finalize ();
       return 0;
}
