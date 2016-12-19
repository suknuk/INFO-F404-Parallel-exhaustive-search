//#include <stdio.h>		// CL output
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>		// MPI methods
#include <openssl/sha.h>	// SHA1()
#include "word_check.h"		// is_equal_word()
#include "worker.h"

int main(int argc, char* argv[])
{
	// MPI vars
	int id, world_size, len;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (world_size < 2) {
		printf("Must have at least 2 processors\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	// args handiling
	if (argc < 5) {
		printf("5 arguments required\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int search_space_nr = atoi(argv[2]);
	int lsb = atoi(argv[4]);

	printf("search space: %d , lsb: %d\n", search_space_nr, lsb);

	double time_start = MPI_Wtime();

	int status_msg = 0;
	unsigned char search_space[search_space_nr];
	
	MPI_Status status;
	// host	
	if (id == 0) {
		// initialize start and end
		unsigned char start_space[search_space_nr];
		unsigned char end_space[search_space_nr];
		for (int i = 0; i < search_space_nr; i++) {
			start_space[i] = 0;
			end_space[i] = 255;
		}

		// iterate the whole space
		while (!is_word_equal(start_space, end_space, search_space_nr)){
			// wait that workers ask for work
			MPI_Recv(&status_msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);
			// create copy of current space
			unsigned char *tmp = (unsigned char*)malloc(search_space_nr);
			memcpy(tmp, start_space, search_space_nr);
			
			increment_search_space(start_space, search_space_nr);
		
			// send next space to idle worker
			MPI_Send(&tmp, search_space_nr, MPI_UNSIGNED_CHAR, 
					status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			MPI_Send(&start_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			
			/*
			printf("send space to %d\nstart :", status.MPI_SOURCE);
			print_word(tmp, search_space_nr);
			printf("end : ");
			print_word(start_space, search_space_nr);
			*/
		}

		// finished searching the space - now wait for all workers to finish

	}
	else {
		unsigned char start_space[search_space_nr];
		unsigned char end_space[search_space_nr];
		// Get work request
		while(true) {
			// send 'i am ready'
			MPI_Send(&status_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			
			//receive search space	
			MPI_Recv(start_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(end_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
			printf("%d received search space\n", id);
			print_word(start_space, search_space_nr);
			print_word(end_space, search_space_nr);
		}
	}	

	double time_end;

	unsigned char word[] = "Hello";
	int nr_bits = 16;
	int lsb_length = 0;
	if (nr_bits % 8 == 0) {
		lsb_length = nr_bits/8;
	} else {
		lsb_length = nr_bits/8 + 1;
	}
	
	unsigned char LSB[lsb_length];
	get_LSB(word, LSB, lsb_length, 5, nr_bits);

	unsigned char start[] = "aaa";
	unsigned char end[] = "dcc";

//	do_compare(LSB, lsb_length, nr_bits, start, end, 3);

	MPI_Finalize();

	return 0;
}
