all: main

main:
	mpiCC src/main.c src/word_check.c src/worker.c -o main -lcrypto

clean:
	rm main
