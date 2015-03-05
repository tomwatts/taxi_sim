all: default

default: taxi_sim.o
	gcc -o taxi_sim taxi_sim.o

taxi_sim.o: taxi_sim.c
	gcc -c taxi_sim.c

clean:
	rm -f *.o taxi_sim

