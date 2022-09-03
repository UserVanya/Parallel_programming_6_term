#include <mpi/mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define MAT MPI_ANY_TAG
#define MCW MPI_COMM_WORLD
#define MSI MPI_STATUS_IGNORE
#define X 1.0
#define T 1.0
double phi(unsigned m, double x_step);
double psi(unsigned p, double t_step);
#define VERBOSE 0
double f(unsigned p, unsigned m, double x_step, double t_step);
void step(double** u, unsigned m, unsigned p, double x_step, double t_step);
void print_u(double **u, unsigned M, unsigned P);

int main(int argc, char* argv[]){
    double time_start;
    unsigned proc_rank, proc_num;
    unsigned P = (unsigned)atoi(argv[1]), M = (unsigned)atoi(argv[2]);
    double t_step = T / P; 
    double x_step = X / M;
    double **u = (double**)malloc(M * sizeof(double*));
    for (unsigned i = 0; i <= M; ++i)
		u[i] = (double*)malloc(P * sizeof(double));
    MPI_Init(&argc,&argv);
    time_start = MPI_Wtime(); 
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    assert(proc_num <= M);
    time_start = MPI_Wtime();
    if (proc_num == 1){
        for (unsigned p = 0; p < P; p++){
            for (unsigned m = 0; m < M; m++){
                step(u, m, p, x_step, t_step);
            }
        }
        print_u(u, M, P);
    }
    else{
        double k = (double)M / proc_num;
        unsigned m_begin = (unsigned)(k * proc_rank);
        unsigned m_end = (unsigned)(k * (proc_rank + 1));
        MPI_Request request_to, request_from;
        if (proc_rank == proc_num - 1)
            m_end = M;
        if (proc_rank == 0){
            for(unsigned p = 0; p < P; p++){
                for (unsigned m = m_begin; m < m_end; m++){
                    step(u, m, p, x_step, t_step);    
                }
                MPI_Isend(&u[m_end-1][p], 1, MPI_DOUBLE, proc_rank+1, 0, MCW, &request_to);
            }
        } 
        else if (proc_rank != proc_num-1){
            for(unsigned p = 0; p < P; p++){
                MPI_Irecv(&u[m_begin-1][p], 1, MPI_DOUBLE, proc_rank-1, MAT, MCW, &request_from);
                for (unsigned m = m_begin; m < m_end; m++){
                    step(u, m, p, x_step, t_step);
                }
                MPI_Isend(&u[m_end-1][p], 1, MPI_DOUBLE, proc_rank+1, 0, MCW, &request_to);
                MPI_Wait(&request_from, MSI);
            }
        }
        else{
            for(unsigned p = 0; p < P; p++){
                MPI_Irecv(&u[m_begin-1][p], 1, MPI_DOUBLE, proc_rank-1, MAT, MCW, &request_from);
                for (unsigned m = m_begin; m < m_end; m++){
                    step(u, m, p, x_step, t_step);
                }
                MPI_Wait(&request_from, MSI);
            }
        }
        //print_u(u, M, P);
        
    }
    MPI_Barrier(MCW);
    if (proc_rank == 0)
        printf("%d\t%d\t%d\t%f\n", M, P, proc_num, MPI_Wtime()-time_start);
    MPI_Finalize();    
}
void print_u(double **u, unsigned M, unsigned P){
    for (unsigned p = 0; p < P; p++){
        for (unsigned m = 0; m < M; m++)
            printf("%.4f ", u[m][p]);
        printf("\n");
    }
    printf("\n");
}
void step(double **u, unsigned m, unsigned p, double x_step, double t_step){
    if (m == 0){
        u[0][p] = psi(p, t_step);
    }
    else if (p == 0){
        u[m][0] = phi(m, x_step);
    }
    else 
        u[m][p] = f(m, p-1, x_step, t_step) * t_step + u[m][p-1] - (u[m][p-1]-u[m-1][p-1]) * t_step / x_step;
}

double phi(unsigned m, double x_step){
    return pow(m * x_step, 2);
}
double psi(unsigned p, double t_step){
    return 2 * sin(2 * p * t_step);
}
double f(unsigned m, unsigned p, double x_step, double t_step){
    return p * t_step * cos(m * x_step);
}