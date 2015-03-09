#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 80

#define EXITWITHFAILUREIF(condition, message) \
	if(condition) \
	{ \
		fprintf(stderr, message); \
		return EXIT_FAILURE; \
	}

void *taxi_thread(void *arg);

typedef struct taxirequest
{ 
	char requester[BUFFER_SIZE];
	int order_time;
	int duration;
	struct taxirequest *next_request;
} TaxiRequest;

TaxiRequest* request_queue;

pthread_mutex_t queue_mutex;

