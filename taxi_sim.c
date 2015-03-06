#include "taxi_sim.h"

void *taxi_thread_function(void *arg)
{
	int taxi_id = (int) arg;
	fprintf(stdout, "Taxi %d starting!\n", taxi_id);

	fprintf(stdout, "Taxi shutting down.\n");

	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	EXITWITHFAILUREIF(argc < 2, "Incorrect number of arguments\n");

	int n_taxis = atoi(argv[1]);
	EXITWITHFAILUREIF(0 >= n_taxis, "Must specify more than zero taxis\n");

	fprintf(stdout, "Using %d taxi(s)\n", n_taxis);

	FILE *queue_file = fopen("taxi_queue.txt", "r+");
	EXITWITHFAILUREIF(!queue_file, 
		"An error occurred closing the queue file\n");

	// Read file into queue
	char name[BUFFER_SIZE];
	int time, duration;
	while(3 == fscanf(queue_file, "%d\t%s\t%d", &time, name, &duration))
	{
		fprintf(stdout, "Time: %d, Name: %s, Duration: %d\n", time, name, duration);

		TaxiRequest request;
		request.order_time = time;
		request.duration = duration;
		strcpy(request.requester, name);

		// TODO: populate queue
	}


	EXITWITHFAILUREIF(0 != fclose(queue_file), 
		"An error occurred closing the queue file\n");

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

	// TODO: destroy queue

	fprintf(stdout, "Done!\n");

	return EXIT_SUCCESS;
}
