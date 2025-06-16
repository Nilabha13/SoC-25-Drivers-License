#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    if(argc != 3){
        fprintf(stderr, "Incorrect number of arguments\n");
        exit(EXIT_FAILURE);
    }
    
    char *cmd = argv[1];
    char *arg = argv[2];
    
    pid_t pid = fork();

    if(pid < 0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if(pid == 0){
        // Child process
        execlp(cmd, cmd, arg, NULL);

        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    wait(NULL);
    printf("Command successfully completed\n");

    return 0;
}