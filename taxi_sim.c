#include "taxi_sim.h"

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Incorrect number of arguments\n");
		return EXIT_SUCCESS;
	}

	int n_taxis = atoi(argv[1]);
	if(n_taxis <= 0)
	{
		fprintf(stderr, "Must specify more than zero taxis\n");
		return EXIT_SUCCESS;
	}

	fprintf(stdout, "Using %d taxi(s)\n", n_taxis);

	FILE *queue_file = fopen("taxi_queue.txt", "r+");
	if(!queue_file)
	{
		fprintf(stderr, "An error occurred closing the queue file\n");
		return EXIT_SUCCESS;
	}

	// TODO: read file

	int close_status = fclose(queue_file);
	if(0 != close_status)
	{
		fprintf(stderr, "An error occurred closing the queue file\n");
		return EXIT_SUCCESS;
	}

	// TODO: start processing queue until no more taxis, then wait

	printf("Done!\n");

	return EXIT_SUCCESS;
}
