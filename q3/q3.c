#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFSIZE 256

int parent(char *file, int pipes[2]){
    int file = open(file,"r");
    if(file == 1){
        fprintf(stderr, "Failed to open the file %s. Cause: %s\n", file, strerror(errno));
        return EXIT_FAILURE;
    }
    char buf[BUFSIZE];
    int bytes; 
    while ((bytes = read(file, buf, BUFSIZE)) > 0) {
        if (write(pipes[1], buf, bytes) == -1) {
            fprintf(stderr, "Failed to write to pipe. Cause: %s\n",
                    strerror(errno));
            close(file);
            close(pipes[1]);
            return EXIT_FAILURE;
        }
    }
    return 0;
}

int child(int pipes[2]){
    return 0;
}

int main(int argc, char *argv[]){
    pid_t pid;
    int pipes[2];
    //Create and open pipe
    if(pipe(pipes)<0){
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if((pid=fork())<0){
        perror("fork error");
        exit(EXIT_FAILURE);
    }else if(pid>0){ 
        //Reading from parent and pass to pipe
        int gen1 = parent(argv[1], pipes[2]);
        if(waitpid(pid, NULL, 0) < 0){ //waiting for child
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }   
        //
    }else{
        return child(pipes[2]);
    }
}