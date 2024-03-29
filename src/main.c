#include <stdio.h>		// CL output
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
		unsigned char start_word[search_space_nr];
		unsigned char end_word[search_space_nr];
		for (int i = 0; i < search_space_nr; i++) {
			start_word[i] = 0;
			end_word[i] = 255;
		}
		//search every possible comination of bits
		while (!is_word_equal(start_word, end_word, search_space_nr)){
			//unsigned char hash[SHA_DIGEST_LENGTH];
			unsigned char *hash = (unsigned char*)malloc(SHA_DIGEST_LENGTH);
			// create tmp of start_word as otherwise SHA gives always the same hash?
			unsigned char *tmp_word = (unsigned char*)malloc(search_space_nr);
			memcpy(tmp_word, start_word, search_space_nr);
			SHA1(tmp_word, sizeof(tmp_word) - 1, hash);
			free(tmp_word);

			unsigned char LSB[lsb_length];
			get_LSB(hash, LSB, lsb_length, SHA_DIGEST_LENGTH, lsb_nr);
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
				MPI_Send(start_word, search_space_nr, MPI_UNSIGNED_CHAR, 
						status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				MPI_Send(tmp, search_space_nr, MPI_UNSIGNED_CHAR, 
						status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				MPI_Send(start_space, search_space_nr, MPI_UNSIGNED_CHAR, 
						status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				// Send the LSB	
				MPI_Send(LSB, lsb_length, MPI_UNSIGNED_CHAR, 
						status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				free(tmp);
			}
			increment_word(start_word, search_space_nr);
			free(hash);
		}

		// finished searching the space - now wait for all workers to finish
		double worked_time[world_size];
		int finished_counter = 1;
		while(finished_counter != world_size){
			MPI_Recv(&status_msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);
			status_msg = 0; // the no more work flag
			// Send that work is finished
			MPI_Send(&status_msg, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			// receive worked time
			MPI_Recv(&worked_time[status.MPI_SOURCE], 1, MPI_DOUBLE, status.MPI_SOURCE, MPI_ANY_TAG,
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			finished_counter++;
		}
		worked_time[id] = MPI_Wtime() - time_start;
		FILE *file = fopen("time.txt", "w");
		for (int i = 0; i < world_size; i++) {
			fprintf(file, "Process %d : %f seconds\n", i, worked_time[i]);
		}
		fclose(file);
	} else {
		// Get work request
		while(true) {
			// send 'i am ready'
			MPI_Send(&status_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			
			// receive anser from host, either there is work or no work
			MPI_Recv(&status_msg, 1, MPI_INT, 0, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status);
			if (status_msg == 0) {
				// send worked time
				double time_end = MPI_Wtime() - time_start;
				MPI_Send(&time_end, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
				break;
			}
			unsigned char *word = (unsigned char*)malloc(search_space_nr);
			unsigned char *start_space = (unsigned char*)malloc(search_space_nr);
			unsigned char *end_space = (unsigned char*)malloc(search_space_nr);
			unsigned char *LSB = (unsigned char*)malloc(lsb_length);
			//receive search space	
			MPI_Recv(word, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(start_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(end_space, search_space_nr, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// receive LSB
			MPI_Recv(LSB, lsb_length, MPI_UNSIGNED_CHAR, 
					0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// Calculate the hash
			do_compare(LSB, lsb_length,lsb_nr, word, start_space, end_space,search_space_nr, id);
			
			free(word);
			free(LSB);
			free(start_space);
			free(end_space);
		}
	}	
	MPI_Finalize();
	return 0;
}
