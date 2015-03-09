#include "taxi_sim.h"

void *taxi_thread_function(void *arg)
{
	int taxi_id = (int) arg;
	fprintf(stdout, "Taxi %d starting!\n", taxi_id);

	while(NULL != request_queue)
	{
		// Get request details and then free it
		pthread_mutex_lock(&queue_mutex);
		TaxiRequest *request = request_queue;
		char *name = request -> requester;
		int drive_time = request -> duration;
		request_queue = request -> next_request;
		pthread_mutex_unlock(&queue_mutex);
		free(request);

		fprintf(stdout, "Taxi %d picking up %s...\n", taxi_id, name);
		sleep(drive_time);
		fprintf(stdout, "Taxi %d dropping off %s...\n", taxi_id, name);
	}

	fprintf(stdout, "Taxi %d shutting down.\n", taxi_id);
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	EXITWITHFAILUREIF(argc != 3, "Incorrect number of arguments\n");

	int n_taxis = atoi(argv[1]);
	EXITWITHFAILUREIF(0 >= n_taxis, "Must specify more than zero taxis\n");

	fprintf(stdout, "Using %d taxi(s)\n", n_taxis);

	FILE *queue_file = fopen(argv[2], "r+");
	EXITWITHFAILUREIF(!queue_file, 
		"An error occurred closing the queue file\n");

	// Read file into queue
	char name[BUFFER_SIZE];
	int time, duration;
	TaxiRequest *request = request_queue;
	while(3 == fscanf(queue_file, "%d\t%s\t%d", &time, name, &duration))
	{
		//fprintf(stdout, "Time: %d, Name: %s, Duration: %d\n", time, name, duration);

		TaxiRequest *prev_request = request;
		request = malloc(sizeof(TaxiRequest));
		request -> order_time = time;
		request -> duration = duration;
		strcpy(request -> requester, name);
		request -> next_request = NULL;

		if(NULL == request_queue)
		{
			request_queue = request;
		}
		else
		{
			prev_request -> next_request = request;
		}

		prev_request = request;
	}

	EXITWITHFAILUREIF(0 != fclose(queue_file), 
		"An error occurred closing the queue file\n");

	EXITWITHFAILUREIF(0 != pthread_mutex_init(&queue_mutex, NULL),
		"An error occurred on mutex init\n");
		
	pthread_t taxi_thread[n_taxis];
	int i;
	for(i = 0; i < n_taxis; i++)
	{
		int thread_create_result = pthread_create(&taxi_thread[i], NULL,
			taxi_thread_function, (void *) i + 1);

		EXITWITHFAILUREIF(thread_create_result != 0, 
			"An error occurred while creating a thread\n");
	}

	fprintf(stdout, "Taxis dispatched; waiting...\n");

	for(i = 0; i < n_taxis; i++)
	{
		int thread_join_result = pthread_join(taxi_thread[i], NULL);

		EXITWITHFAILUREIF(0 != thread_join_result, 
			"An error occurred while joining a thread\n");
	}

	pthread_mutex_destroy(&queue_mutex);

	fprintf(stdout, "Done!\n");

	return EXIT_SUCCESS;
}
