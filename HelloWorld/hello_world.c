#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int id, nb_instance, len;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_instance);
	MPI_Get_processor_name(processor_name, &len);

	printf("Hello world! I'm %d of %d on %s\n", id, nb_instance, processor_name);

	MPI_Finalize();

	return 0;
}
