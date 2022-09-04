


#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include "time.h"
#include <math.h>
#include <stdbool.h>
#include <math.h>
//Функция, вычисляющая интеграл
double factorial(int n){
    if (n != 1 && n != 0)
        return n*(factorial(n-1));
    else {
        if (n == 0 || n == 1)
            return 1;
        else
            printf("wrong argument: negative value\n");
            exit(0);
    }
}
//Функция вычисляющая слагаемое ряда
double part(int n){
    return 1/factorial(n);
}
// Вычислим ряд -1^(n+1)/n
double more_difficult_part(int n){
    if (n == 0)
        return 0;
    else
        return pow(-1, n+1) / n;
}
int main(int argc, char* argv[]){
    int ProcRank, ProcNum, N = atoi(argv[1]);
    double time_start, time_finish, time, ProcSum = 0, TotalSum = 0, RecvBuf = 0;
    int flag = 0;

    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    //Засекаем время
    time_start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    //Выделяем интервал от i1 до i2 для каждого из потоков
    int k = N / ProcNum;
    int i1 = k * ProcRank;
    int i2 = k * ( ProcRank + 1 );
    if ( ProcRank == ProcNum-1 )
        i2 = N;
    
    //производим вычисление своей частичной суммы ряда
    for (int i = i1; i < i2; i++ ){
        ProcSum = ProcSum + more_difficult_part(i);
    }
    MPI_Request request;
    MPI_Status status;
    
    //Отправляем результаты со всех процессов на нулевой и суммируем их с помощью MPI_Reduce 
    MPI_Reduce(&ProcSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);        
    
    if ( ProcRank == 0 )
        printf("e = %f, procNum = %d\n", TotalSum, ProcNum);

    //Вычисляем время для каждого процесса
    time_finish = MPI_Wtime();
    time = (time_finish-time_start);
    printf("process %d finished. Time passed: %f\n", ProcRank, time);
    //Ждем пока все процессы дойдут до этой строки и считаем время выполнения всей программы
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0){
        time_finish = MPI_Wtime();
        time = (time_finish-time_start);
        printf("Time passed: %f\n", time);
    }
    MPI_Finalize();
    return 0;

}