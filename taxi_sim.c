#include "taxi_sim.h"

void *taxi_thread_function(void *arg)
{
	fprintf(stdout, "Taxi starting!\n");

	pthread_exit("Taxi shutting down.\n");
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Incorrect number of arguments\n");
		return EXIT_FAILURE;
	}

	int n_taxis = atoi(argv[1]);
	if(n_taxis <= 0)
	{
		fprintf(stderr, "Must specify more than zero taxis\n");
		return EXIT_FAILURE;
	}

	fprintf(stdout, "Using %d taxi(s)\n", n_taxis);

	FILE *queue_file = fopen("taxi_queue.txt", "r+");
	if(!queue_file)
	{
		fprintf(stderr, "An error occurred closing the queue file\n");
		return EXIT_FAILURE;
	}

	// TODO: read file into queue

	int close_status = fclose(queue_file);
	if(0 != close_status)
	{
		fprintf(stderr, "An error occurred closing the queue file\n");
		return EXIT_FAILURE;
	}

	int i;
	for(i = 0; i < n_taxis; i++)
	{
		pthread_t taxi_thread;
		int thread_create_result = pthread_create(&taxi_thread, NULL,
			taxi_thread_function, NULL);

		if(thread_create_result != 0)
		{
			fprintf(stderr, "An error occurred while creating a thread\n");
			return EXIT_FAILURE;
		}
	}

	fprintf(stdout, "Done!\n");

	return EXIT_SUCCESS;
}
