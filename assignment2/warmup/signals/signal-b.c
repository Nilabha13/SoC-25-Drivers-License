#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void handle_sigint(int sig){
    printf("I will run forever\n");
}

int main(){
    if(signal(SIGINT, handle_sigint) == SIG_ERR){
        perror("signal registration failed");
        exit(EXIT_FAILURE);
    }

    printf("Running infinte loop now...");

    while(1){}

    return 0;
}