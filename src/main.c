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
	int lsb_nr = atoi(argv[4]);

	printf("search space: %d , lsb: %d\n", search_space_nr, lsb_nr);

	double time_start = MPI_Wtime();

	// char length of the LSB char array
	int lsb_length = 0;
	if (lsb_nr % 8 == 0) {
		lsb_length = lsb_nr/8;
	} else {
		lsb_length = lsb_nr/8 + 1;
	}
	
	// 0 = finished, 1 = work 
	int status_msg = 1;
	unsigned char search_space[search_space_nr];
	
	MPI_Status status;
	// host	
	if (id == 0) {
		// select a word in the space
		unsigned char word[search_space_nr];
		for (int i = 0; i < search_space_nr; i++) {
			word[i] = 61;
		}

		unsigned char LSB[lsb_length];
		get_LSB(word, LSB, lsb_length, search_space_nr, lsb_nr);

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
			
			// Send that there is work to do
			MPI_Send(&status_msg, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			
			// send next space to idle worker
			MPI_Send(tmp, search_space_nr, MPI_UNSIGNED_CHAR, 
					status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			MPI_Send(start_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			// Send the LSB	
			MPI_Send(LSB, lsb_length, MPI_UNSIGNED_CHAR, 
					status.MPI_SOURCE, 0, MPI_COMM_WORLD);
		
			print_word(start_space, search_space_nr);

			free(tmp);
		}
		printf("There is no more work to do\n");
		// finished searching the space - now wait for all workers to finish

		int finished_counter = 1;
		while(finished_counter != world_size){
			MPI_Recv(&status_msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);
			status_msg = 0; // the no more work flag
			// Send that work is finished
			MPI_Send(&status_msg, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			finished_counter++;
		}
		printf("everyone finished\n");

	} else {
		// Get work request
		while(true) {
			// send 'i am ready'
			MPI_Send(&status_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			
			// receive anser from host, either there is work or no work
			MPI_Recv(&status_msg, 1, MPI_INT, 0, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);
			if (status_msg == 0) {
				break;
			}

			unsigned char *start_space = (unsigned char*)malloc(search_space_nr);
			unsigned char *end_space = (unsigned char*)malloc(search_space_nr);
			unsigned char *LSB = (unsigned char*)malloc(lsb_length);
			
			//receive search space	
			MPI_Recv(start_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(end_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// receive LSB
			MPI_Recv(LSB, lsb_length, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
			free(LSB);
			free(start_space);
			free(end_space);
		}
		printf("%d has finished working\n", id);
	}	

	double time_end;


	unsigned char start[] = "aaa";
	unsigned char end[] = "dcc";

//	do_compare(LSB, lsb_length, nr_bits, start, end, 3);

	MPI_Finalize();

	return 0;
}
