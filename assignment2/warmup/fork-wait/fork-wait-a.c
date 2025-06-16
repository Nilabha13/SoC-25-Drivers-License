#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Usage: %s <N>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int N = atoi(argv[1]);

	for(int i = 0; i < N; i++){
		pid_t pid = fork();
		if(pid < 0){
			perror("fork failed");
			exit(EXIT_FAILURE);
		} else if(pid == 0){
			// Child process
			printf("Child PID: %d\n", getpid());
			exit(EXIT_SUCCESS);
		}
	}

	for(int i = 0; i < N; i++){
		wait(NULL);
	}

	return 0;
}
