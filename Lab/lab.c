#include <mpi/mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define MAT MPI_ANY_TAG
#define MCW MPI_COMM_WORLD
#define MSI MPI_STATUS_IGNORE

#define T_GRID_SIZE 5
#define X_GRID_SIZE 5
#define T_STEP (1.0/T_GRID_SIZE)
#define X_STEP (1.0/X_GRID_SIZE)
double phi(unsigned m);
double psi(unsigned p);
#define VERBOSE 1
double f(unsigned p, unsigned m);
void dump_u(unsigned m_begin, unsigned m_end);
void step(unsigned p, unsigned m);
static double u[X_GRID_SIZE][T_GRID_SIZE] = {0};
static double temp[T_GRID_SIZE]= {0};
int main(int argc, char* argv[]){
    assert(T_STEP!=0);
    assert(X_STEP!=0);
    double time_start;
    int ProcRank, ProcNum;
    MPI_Init(&argc,&argv);
    time_start = MPI_Wtime(); 
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if(ProcNum == 1){
        for (unsigned m = 0; m < X_GRID_SIZE; m++){
            for(unsigned p = 0; p < T_GRID_SIZE; p++){
                step(m, p);
            }
        }
        if (VERBOSE) dump_u(0, X_GRID_SIZE);
    }
    else{
        assert(ProcNum <= X_GRID_SIZE);
        MPI_Request request;
        MPI_Barrier(MCW);
        for (unsigned m = ProcRank; m < X_GRID_SIZE; m+=ProcNum){
            for(unsigned p = 0; p < T_GRID_SIZE; p++){
                if (m != 0)
                    MPI_Recv(&u[m-1][p], 1, MPI_DOUBLE, (m-1)%ProcNum, MAT, MCW, MSI);
                step(m, p);
                if (m != X_GRID_SIZE-1)
                    MPI_Isend(&u[m][p], 1, MPI_DOUBLE, (m+1)%ProcNum, 0, MCW, &request);
            }
        }
        printf("ProcRank=%d\n", ProcRank);
        for (unsigned m = 0; m < X_GRID_SIZE; m++)
            if(m%ProcNum != 0){
                if (ProcRank == m%ProcNum)
                    MPI_Ssend(&u[m], T_GRID_SIZE, MPI_DOUBLE, 0, 0, MCW);
                if (ProcRank == 0 && m%ProcNum != 0)
                    MPI_Recv(&u[m], T_GRID_SIZE, MPI_DOUBLE, m%ProcNum, MAT, MCW, MSI);          
            }
    } 
    MPI_Barrier(MCW);
    if (ProcRank==0)
        printf("%d\t%f\n", ProcNum, MPI_Wtime()-time_start);
    if(ProcRank == 0 && VERBOSE)
        dump_u(0, X_GRID_SIZE);
    MPI_Finalize();    
    
    
}
void step(unsigned m, unsigned p){
    if (m == 0){
        u[0][p] = psi(p);
    }
    else if (p == 0){
        u[m][0] = phi(m);
    }
    else 
        u[m][p] = f(m, p-1)*T_STEP + u[m][p-1] - (u[m][p-1]-u[m-1][p-1])*T_STEP/X_STEP;
}
void dump_u(unsigned m_begin, unsigned m_end) {
    for (unsigned m = m_begin; m < m_end; m++) {
        for (unsigned p = 0; p < T_GRID_SIZE; p++)
            printf("%.4f ", u[m][p]);
        printf("\n");
    }
    printf("\n");
}
double phi(unsigned m){
    return pow(m*X_STEP, 2);
}
double psi(unsigned p){
    return 2*sin(2*p*T_STEP);
}
double f(unsigned m, unsigned p){
    return p*T_STEP*cos(m*X_STEP);
}