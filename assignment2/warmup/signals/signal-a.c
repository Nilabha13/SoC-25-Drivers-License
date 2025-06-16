#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(){
    pid_t pid = fork();

    if(pid < 0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if(pid == 0){
        // Child process
        printf("Child started with PID %d, sleeping for 20 seconds...\n", getpid());
        sleep(20);
        printf("Child woke up");
        exit(EXIT_SUCCESS);
    }

    sleep(2);
    printf("Parent: Sending SIGKILL to child with PID %d\n", pid);
    if(kill(pid, SIGKILL) == -1){
        perror("kill failed");
        exit(EXIT_FAILURE);
    }

    int status;
    waitpid(pid, &status, 0);

    if(WIFSIGNALED(status))
        printf("Parent: Child terminated by signal %d\n", WTERMSIG(status));
    else
        printf("Parent: Child exited normally\n");

    return 0;
}