#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>

int main() {

    int p1[2]; // Array to hold the file descriptors for the first pipe
    int p2[2]; // Array to hold the file descriptors for the second pipe
    
    // Create the first pipe. p1[0] is the read end, p1[1] is the write end.
    pipe(p1);

    // Create the second pipe. p2[0] is the read end, p2[1] is the write end.
    pipe(p2);

    // Duplicate the read end of p1 to file descriptor 33
    dup2(p1[0], 33);
    // Duplicate the write end of p1 to file descriptor 34
    dup2(p1[1], 34);
    // Duplicate the read end of p2 to file descriptor 53
    dup2(p2[0], 53);
    // Duplicate the write end of p2 to file descriptor 54
    dup2(p2[1], 54);

    // Path to the executable to be run
    char exec[] = "./riddle";
    // Arguments for the new program
    char *newargv[] = {exec, NULL, NULL, NULL };
    // Environment variables for the new program (none in this case)
    char *newenviron[] = { NULL };

    // Replace the current process with a new process image
    execve(exec, newargv, newenviron);
    
    return 0;
}
