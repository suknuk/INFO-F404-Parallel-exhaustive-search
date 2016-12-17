#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int id, world_size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int token;

	if (id == 0){
		token = 1;
		MPI_Send(&token, 1, MPI_INT, (id + 1) % world_size, 0, MPI_COMM_WORLD);
		MPI_Recv(&token, 1, MPI_INT, world_size -1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::cout << id << " received " << token << " from " << (world_size - 1) << std::endl;
	} else {
		MPI_Recv(&token, 1, MPI_INT, id - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::cout << id << " received " << token << " from " << (id - 1) << std::endl;
		token++;
		MPI_Send(&token, 1, MPI_INT, (id + 1) % world_size, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
