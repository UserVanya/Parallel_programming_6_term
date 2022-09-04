#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>




#define HANDLE_ERROR(er, msg, ret) \
	do {errno = er; perror(msg); exit(ret); } while (0)

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0
//Структура для отправки интервала для суммирования в нить
struct interval{
    unsigned begin;
    unsigned end;
    unsigned partial_sum;
};
//Функция выполняющая нахождение частичной суммы (каждая нить в ней выполняется отдельно(асинхронно и параллельно))
void* sum(void* unit)
{
	struct interval* interval = ((struct interval*)unit);
    for (unsigned i = interval->begin; i < interval->end; i++){
        interval->partial_sum += i;
    }
}

int main(int argc, char* argv[])
{
    struct timespec begin, end;
    double elapsed = 0;
    unsigned N = (unsigned)(atoi(argv[1]) + 1);
	unsigned NP = (unsigned)(atoi(argv[2]));
	//Выделяем память для нитей и для интервалов
    pthread_t* threads = (pthread_t*)malloc(NP * sizeof(pthread_t));
	struct interval* unit = (struct interval*)malloc(NP * sizeof(struct interval));
    //Замер времени
    clock_gettime(CLOCK_REALTIME, &begin);
    
    for (unsigned i = 0; i < NP; i++)
	{
        //выделеление интервала нити i
        double k = 1.0 * N / NP;
        unit[i].begin = (unsigned)(k * i);
        unit[i].end = (unsigned)(k * (i + 1));
        if (i == NP - 1)
            unit[i].end = (unsigned)(N);
        unit[i].partial_sum = 0;
        //Отправка нити к выполнению функции sum
        int status = pthread_create(&(threads[i]), NULL, sum, (void*)&unit[i]);
		if (status != 0)
			HANDLE_ERROR(status, "main error: can't create thread", ERROR_CREATE_THREAD);
	}
    unsigned total_sum = 0;
    for (unsigned i = 0; i < NP; ++i)
	{
        unsigned return_addr;
        //Ожидание выполнения нити 
		int partial_sum = pthread_join(threads[i], (void**)&return_addr);
        //Суммирования частичных сумм
        total_sum += unit[i].partial_sum;
    }
    printf("total_sum = %d\n", total_sum);
	//Замер времени выполнения
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = end.tv_sec - begin.tv_sec;
    elapsed += (end.tv_nsec - begin.tv_nsec) * 1.0/1e9;
    printf("elapsed time: %f\n", elapsed);
    free(threads);
	free(unit);
	return 0;
}