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
        printf("I am child\n");
        exit(EXIT_SUCCESS);
    }

    printf("I am parent\n");
    wait(NULL);

    return 0;
}