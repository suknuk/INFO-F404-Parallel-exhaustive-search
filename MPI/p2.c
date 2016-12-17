#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int world_rank, world_size, len;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_size < 2) {
		std::cerr << "Must have at least 2 processors" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int number;
	if (world_rank == 0) {
		number = -1;
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else if (world_rank == 1) {
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::cout << "Process 1 received number " << number << " from process 0" << std::endl;
	}

	MPI_Finalize();

	return 0;
}
