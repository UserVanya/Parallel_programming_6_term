#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define HANDLE_ERROR(er, msg, ret) \
	do {errno = er; perror(msg); exit(ret); } while (0)

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0
struct interval{
    double begin;
    double end;
    double partial_sum;
    double h;
    int is_last;
    double* sum;
};
double func(double x){
    return x*x;
}
void* sum(void* unit)
{
	struct interval* interval = ((struct interval*)unit);
    for (double x = interval->begin; x < interval->end; x += interval->h){
        
        if (interval->is_last && x + interval->h >= interval->end){
            double h = (interval->end - x);
            interval->partial_sum += h * (func(x) + func(interval->end))/2;
        }
        else{
            interval->partial_sum += interval->h * (func(x) + func(x + interval->h)) / 2;
        } 
    }
    (*interval->sum) += interval->partial_sum;
    return (void*)0;
}

int main(int argc, char* argv[])
{
    struct timespec begin_t, end_t;
    double* sum_val = (double*)malloc(sizeof(double));
    *sum_val = 0;
    double elapsed = 0;
    double begin = (double)(atof(argv[1])), end = (double)(atof(argv[2]));
    double h = (double)(atof(argv[3]));
	unsigned NP = (unsigned)(atoi(argv[4]));
	pthread_t* threads = (pthread_t*)malloc(NP * sizeof(pthread_t));
	struct interval* unit = (struct interval*)malloc(NP * sizeof(struct interval));
    clock_gettime(CLOCK_REALTIME, &begin_t);
    double k = (end - begin)/NP;
    unsigned h_number = (unsigned)k/h;    
    for (unsigned i = 0; i < NP; i++)
	{
        unit[i].begin = begin + h_number * h * i;
        unit[i].end = begin + h_number * h * (i + 1);
        unit[i].h = h;
        unit[i].sum = sum_val;
        if (i == NP - 1){
            unit[i].end = end; 
            unit[i].is_last = TRUE;
        }
        else{
            unit[i].is_last = FALSE;
        }
        //printf("i = %d, begin = %f, end = %f, h=%f is_last=%d\n", i, unit[i].begin, unit[i].end, unit[i].h, unit[i].is_last);
        unit[i].partial_sum = 0;
        int status = pthread_create(&(threads[i]), NULL, sum, (void*)&unit[i]);
		if (status != 0)
			HANDLE_ERROR(status, "main error: can't create thread", ERROR_CREATE_THREAD);
	}
    for (unsigned i = 0; i < NP; ++i)
	{
        unsigned return_addr;
		int status = pthread_join(threads[i], (void**)&return_addr);
        if (status != SUCCESS)
			HANDLE_ERROR(status, "main error: can't join thread", ERROR_JOIN_THREAD);
        printf("thread %d joined with return value %d\n", i, return_addr);
    }
    printf("sum = %f\n", *(unit[0].sum));
	clock_gettime(CLOCK_REALTIME, &end_t);
    elapsed = end_t.tv_sec - begin_t.tv_sec;
    elapsed += (end_t.tv_nsec - begin_t.tv_nsec) * 1.0/1e9;
    printf("elapsed time: %f\n", elapsed);
    free(threads);
	free(unit);
	return 0;
}