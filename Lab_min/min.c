#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
int main(int argc, char* argv[]){
 int ProcRank, ProcNum;
 int sbuf = 4, rbuf = 0;
 double time_start, time_finish, time;   
 MPI_Status Status;
 MPI_Init(&argc,&argv); 
 MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
 MPI_Comm_rank(MPI_COMM_WORLD,&ProcRank);
 MPI_Request request;
 MPI_Isend(&sbuf, 1, MPI_INT, ProcRank, 0, MPI_COMM_WORLD, &request);
 time_start = MPI_Wtime();
 MPI_Recv(&rbuf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 time_finish = MPI_Wtime();
 time = (time_finish-time_start);      
 printf("rank %d: time = %f, rbuf=%d\n", ProcRank, time, rbuf);     
 MPI_Finalize();
} 