#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>

//read end is 0 
//write end is 1
//read(33, 0x7fff08895fb4, 4)       = -1 EBADF (Bad file descriptor)
//write(34, "\0\0\0\0", 4 <unfinished ...>
//int execve(const char *pathname, char *const _Nullable argv[],char *const _Nullable envp[]);

int main(){
    int main_pipe[2];
    pipe(main_pipe);
    dup2(main_pipe[0], 33);
    dup2(main_pipe[1], 34);

    int second_pipe[2];
    pipe(second_pipe);
    dup2(second_pipe[0], 53);
    dup2(second_pipe[1], 54);
    //8640  read(53,  <unfinished ...>
    //8641  write(54, "\1\0\0\0", 4)          = -1 EBADF (Bad file descriptor)
    //we need one more pipe

    execve("./riddle", NULL, NULL);
}
