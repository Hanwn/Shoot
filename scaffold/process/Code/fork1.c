#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    printf("%d,Begin",getpid());
    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid == 0) {
        //child
        printf("[%d]:child is running.\n",getpid());
    }else{
        //parent
        printf("[%d]:parent is running.\n",getpid());
    }
    printf("[%d] end\n",getpid());

    exit(0);
}