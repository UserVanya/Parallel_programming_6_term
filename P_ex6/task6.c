#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct{		//structure for thread data
	int part_t;		//how many numbers a thread should sum
	int start_t;		//starting from which number
	int result_t;		//where the result is stored
	int t_id;			//thread id
} pthrData;

void* threadFunc(void* thread_data){
	struct timespec begin_t, end_t;					//local variables needed only in thread functions to calculate elapsed time of calculations for each thread
	double elapsed_t;
	clock_gettime(CLOCK_REALTIME, &begin_t);		//put this moment of time into begin_t

	pthrData *data = (pthrData*) thread_data;		//bc thread_data is void*
	for(int i = 0; i < data->part_t; i++)
		data->result_t += data->start_t + i;			//sum from start for part numbers

	clock_gettime(CLOCK_REALTIME, &end_t);			//put this moment of time into end_t
	elapsed_t = end_t.tv_sec - begin_t.tv_sec;		//elapsed time is the difference between begin and end
	elapsed_t += (end_t.tv_nsec - begin_t.tv_nsec)/1000000000.0;				//adding time including nanoseconds
	printf("The thread %d was working for %f seconds\n", data->t_id, elapsed_t);		//printing the elapsed time

	pthread_exit(0);
}


int main(int argc, char** argv){
	struct timespec begin, end;	//variables for start and end of calculations
	double elapsed;				//variable for elapsed time 
	if(argc != 3){							
		printf("Write arguments: Number_of_threads N\n");	//check if the correct number of arguments was given
		return 0;
		}
	int num_threads = atoi(argv[1]);				//number of threads
	if (!num_threads){
		printf("Write a valid number of threads (>0)!\n");			//check if number of threads > 0
		return 0;													//(since main is not calculating anything in this case sum will not be correct and will always be equal to 0)
	}
	int N = atoi(argv[2]);						//until which number to sum up

	clock_gettime(CLOCK_REALTIME, &begin);		//putting time at this moment into begin

	pthread_t* tid = (pthread_t*) malloc(num_threads * sizeof(pthread_t));		//allocating memory for array of threads id
	pthrData* threadData = (pthrData*) malloc(num_threads * sizeof(pthrData));	//allocating memory for thread data

	int part = N/num_threads;		//integer division (how many numbers a thread will have to sum)
	int shift = N%num_threads;		//remainder of division (will be calculated by the first thread)
	int res = 0;					//the final result

	for(int i = 0; i < num_threads; i++){	//loop for each 
		threadData[i].t_id = i;				//id for each thread (needed for adequate printf of calculation time)
		threadData[i].result_t = 0;			//result for each thread = 0
		if (!i)
		{
			threadData[i].part_t = part + shift;		//the first thread sums up part + shift numbers
			threadData[i].start_t = 1;					//starting from 1
		}
		else
		{
			threadData[i].part_t = part;						//the rest of the threads sum up part of numbers
			threadData[i].start_t = part * i + shift + 1;		//starting one after the other
		}
		
		pthread_create(tid + i, NULL, threadFunc, threadData + i);		//creating threads
	}

	for (int i = 0; i < N; i++){		//for each thread
		pthread_join(tid[i], NULL);		//waiting for threads to finish
		res += threadData[i].result_t;	//calculating the final result
	}

	printf("==================================RESULT======================================\n");				//For a better view of the final result
	printf("Sum of numbers from 1 to %d, calculated by %d threads, is equal to %d\n", N, num_threads, res); //printing the final result

	clock_gettime(CLOCK_REALTIME, &end);		//putting time at this moment into end
	elapsed = end.tv_sec - begin.tv_sec;		//elapsed time is the difference between begin and end
	elapsed += (end.tv_nsec - begin.tv_nsec)/1000000000.0;				//adding time including nanoseconds
	printf("==================================TIME======================================\n");		//For a better view of the final calculation time
	printf("The threads were working for %f seconds in total\n", elapsed);							//printing the elapsed time

	free(tid);				//free memory for thread ids
	free(threadData);		//and thread data
	return 0;
}