#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define HANDLE_ERROR(er, msg, ret) \
	do {errno = er; perror(msg); exit(ret); } while (0)

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

void* hello_world(void* ids)
{
	unsigned id = *((unsigned*)ids);
	printf("Hello world from thread %d with thread_id: %ld\n", id, pthread_self());
	return (void*)42;
}

int main(int argc, char* argv[])
{
	unsigned N = (unsigned)(atoi(argv[1]));
	pthread_t* threads = (pthread_t*)malloc(N * sizeof(pthread_t));
	unsigned*  ids       = (unsigned*)malloc(N * sizeof(unsigned));
	for (unsigned i = 0; i < N; i++)
	{
		ids[i] = i;
		int status = pthread_create(&(threads[i]), NULL, hello_world, (void*)&(ids[i]));
		if (status != 0)
			HANDLE_ERROR(status, "main error: can't create thread", ERROR_CREATE_THREAD);
	}
	printf("Hello from main!\n");
	for (unsigned i = 0; i < N; ++i)
	{
        unsigned return_addr;
		int status = pthread_join(threads[i], (void**)&return_addr);
		if (status != SUCCESS)
			HANDLE_ERROR(status, "main error: can't join thread", ERROR_JOIN_THREAD);
        printf("thread %d joined with return value %d\n", i, return_addr);
	}
	
	free(threads);
	free(ids);
	return 0;
}