#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int id, world_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_size < 2) {
		std::cout << "At least 2 processes required" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int number_amount;

	if (id == 0){
		const int MAX_NUMBERS = 100;
		int numbers[MAX_NUMBERS];
		
		srand(time(NULL));
		number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
		
		MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
		std::cout << id << " send " << number_amount << " to 1" << std::endl;
	} else if (id == 1) {
		MPI_Status status;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

		MPI_Get_count(&status, MPI_INT, &number_amount);
		
		int* number_buf = (int*)malloc(sizeof(int) * number_amount);
		
		MPI_Recv(number_buf, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		
		std::cout << id << " received " << number_amount << " from 0."
			<< "Message source = " << status.MPI_SOURCE
			<< ", tag = " << status.MPI_TAG
			<< std::endl;
	}

	MPI_Finalize();

	return 0;
}
