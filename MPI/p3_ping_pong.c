#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
	const int PING_PONG_LIMIT = 20;

	int world_rank, world_size, len;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_size < 2) {
		std::cerr << "Must have at least 2 processors" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int ping_pong_counter = 0;
	int partner_rank = (world_rank + 1) % 2;
	while (ping_pong_counter < PING_PONG_LIMIT) {
		if (world_rank == ping_pong_counter % 2) {
			ping_pong_counter++;
			MPI_Send(&ping_pong_counter, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
			std::cout << world_rank << " send " << ping_pong_counter << " to " << partner_rank
				<< std::endl;
		} else {
			MPI_Recv(&ping_pong_counter, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);
			std::cout << world_rank << " received " << ping_pong_counter << " from " 
				<< partner_rank << std::endl;
		}	
	}

	MPI_Finalize();

	return 0;
}
