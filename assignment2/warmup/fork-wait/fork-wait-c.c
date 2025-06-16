#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    if(pid < 0){
			perror("fork failed");
			exit(EXIT_FAILURE);
    } else if(pid == 0){
        // Child process
        printf("Child PID: %d\n", getpid());
        exit(EXIT_SUCCESS);
    }

    int status;
    pid_t cpid = waitpid(pid, &status, 0);

    if(cpid == -1){
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }

    printf("Reaped child with PID: %d\n", cpid);

    return 0;
}