#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *taxi_thread(void *arg);

typedef struct {
	char* passenger;
	int order_time;
	int duration;
} TaxiOrder;

