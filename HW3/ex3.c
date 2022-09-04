//
// Created by start on 16.03.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "unistd.h"

void DataInitialization(int *x, char* argv[], int ProcNum){
    int i = 0;
    for (i; i < ProcNum; i++){
        x[i] = atoi(argv[i+1]);
    }
}
int main(int argc, char* argv[]){
    int ProcRank = 0, ProcNum = 0;

    MPI_Init(&argc, &argv);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Request request;
    int *x = (int*) malloc((ProcNum)*sizeof(int));
    int *r = (int*) malloc((ProcNum)*sizeof(int));
    if ( ProcRank == 0 ){
        DataInitialization(x, argv, ProcNum);
        for (int i = 0; i < ProcNum; i++){
            printf("x[%d] = %d\n", i, x[i]);
        }
        MPI_Isend(x, ProcNum, MPI_INT, 1%ProcNum, 0, MPI_COMM_WORLD, &request);
    } 

    for (int i = 1; i < ProcNum*3; i++){
        if (ProcRank == i%ProcNum){
            //MPI_Barrier(MPI_COMM_WORLD);
            MPI_Recv(r, ProcNum, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("procRank %d, out:%d\n", ProcRank, r[i%ProcNum]);
            for (int j = 0; j < ProcNum; j++){
                x[j]=r[j];
            }
            MPI_Isend(x, ProcNum, MPI_INT, (i+1)%ProcNum, 0, MPI_COMM_WORLD, &request);
            sleep(1);
        }
    }
    free(x);
    free(r);
    MPI_Finalize();
    return 0;
}