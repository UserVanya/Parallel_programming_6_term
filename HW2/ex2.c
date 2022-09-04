#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "time.h"

void DataInitialization(int *x, int N){
    int i = 0;
    for (i; i < N; i++){
        x[i] = i;
    }
}
int main(int argc, char* argv[]){
    clock_t t;

    t = clock();
    int ProcRank, ProcNum, N = atoi(argv[1])+1, ProcSum = 0, TotalSum = 0;
    double time_start, time_finish, time;
    int *x = (int*) malloc((N)*sizeof(int));


    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    time_start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);


    if ( ProcRank == 0 ) DataInitialization(x, N);
    int i = 0;
    MPI_Bcast(x, N, MPI_INT, 0, MPI_COMM_WORLD);

    int k = N / ProcNum;

    int i1 = k * ProcRank;
    int i2 = k * ( ProcRank + 1 );
    if ( ProcRank == ProcNum-1 )
        i2 = N;
    i = i1;
    for (i1; i < i2; i++ )
        ProcSum = ProcSum + x[i];


    if ( ProcRank == 0 ) {
        TotalSum = ProcSum;
        i = 1;
        for ( i; i < ProcNum; i++ ) {
            MPI_Recv(&ProcSum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                     &Status);
            TotalSum = TotalSum + ProcSum;
        }
    }
    else {
        MPI_Send(&ProcSum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if ( ProcRank == 0 )
        printf("Total Sum = %d, procNum = %d\n", TotalSum, ProcNum);
    free(x);

    time_finish = MPI_Wtime();
    time = (time_finish-time_start);
    printf("process %d finished. Time passed: %f\n", ProcRank, time);
    MPI_Finalize();
    t = clock() - t;
    if (ProcRank == 0){
        printf ("It took me %f seconds.\n", ((double)t)/CLOCKS_PER_SEC);
    }
    return 0;

}