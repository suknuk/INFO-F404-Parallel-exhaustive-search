#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int world_rank, world_size, len;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Get_processor_name(processor_name, &len);

	std::cout << "Hello from " << world_rank << " of " << world_size << " on " << processor_name << std::endl;

	MPI_Finalize();

	return 0;
}
