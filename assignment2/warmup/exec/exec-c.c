#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("Before execlp\n");

    execlp("fake_cmd", "fake_cmd", NULL);

    printf("After execlp\n");
    perror("execlp failed");

    return 0;
}