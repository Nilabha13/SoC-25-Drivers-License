#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    char **ls_args = malloc((argc + 1) * sizeof(char *));
    if(!ls_args){
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    ls_args[0] = "ls";
    for(int i = 1; i < argc; i++){
        ls_args[i] = argv[i];
    }
    ls_args[argc] = NULL;

    execvp("ls", ls_args);

    perror("execvp failed");
    free(ls_args);

    return 0;
}