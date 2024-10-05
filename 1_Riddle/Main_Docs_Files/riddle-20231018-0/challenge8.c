#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
    char filename[6];
    for (int i=0; i<10; i++){
        snprintf(filename, sizeof(filename), "bf%02d", i);
    

    int fd = open(filename, O_CREAT | O_RDWR, 0640);
    if (fd == -1){
        perror("open");
        return 1;
    }

    if (lseek(fd,1073741824,SEEK_SET) == (off_t)-1){
        perror("lseek");
        close(fd);
        return 1;
    }

    if (write(fd,"X",1) != 1){
        perror("write");
        close(fd);
        return 1;
    }
    }

    return 0;
}