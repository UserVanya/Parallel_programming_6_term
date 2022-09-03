#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char* argv[]){
 int ProcRank, ProcNum;
 MPI_Status Status;
 MPI_Init(&argc,&argv); 

 MPI_Comm_size(MPI_COMM_WORLD,&ProcNum);
 MPI_Comm_rank(MPI_COMM_WORLD,&ProcRank);
 printf("%d of %d\n", ProcRank+1, ProcNum);

 MPI_Finalize();
} 