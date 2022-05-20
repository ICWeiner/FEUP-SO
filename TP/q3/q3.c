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
#define READ_END 0
#define WRITE_END 1


struct wordsCypher {
    char* wordA;
    char* wordB;
};

int parent( int pipes_fd[2]){
    /*close(pipes_fd[READ_END]);

    int file_fd = open(file,O_RDONLY);

    if (file_fd == -1) {
        fprintf(stderr, "Failed to open file '%s'. Cause: %s\n", file,
                strerror(errno));
        close(pipes_fd[WRITE_END]);
        return EXIT_FAILURE;
    }

    char buf[BUFSIZE];
    int bytes;

    while ((bytes = read(file_fd, buf, BUFSIZE)) > 0) {
        if (write(pipes_fd[1], buf, bytes) == -1) {
            fprintf(stderr, "Failed to write to pipe. Cause: %s\n",
                    strerror(errno));
            close(file_fd);
            close(pipes_fd[1]);
            return EXIT_FAILURE;
        }
    }
    if (bytes == -1) { // handle errors while reading file
        fprintf(stderr, "Error while reading '%s'. Cause: %s\n", file,
                strerror(errno));
        close(file_fd);
        close(pipes_fd[WRITE_END]);
        return EXIT_FAILURE;
    }

    close(pipes_fd[WRITE_END]);
    close(file_fd);*/
    return EXIT_SUCCESS;
}

int child(int pipes_fd[2])
    FILE* fp = fopen("cypher.txt","r");

    if (fp == NULL) {
        close(pipes_fd[WRITE_END]);
        return EXIT_FAILURE;
    }else{
        char c;
        char buf[20]; //por favor nao testar com palavras maiores que 20
        int i = 0,j = 0, flag = 0;
        struct wordsCypher words* = malloc(sizeof(struct wordsCypher));
        while ( ( c = fgetc(fp)) != EOF){
            printf ("%c", c);
            if(c!=' '){
                buf[i] = c;
                i++;
            }else if(flag == 0){// guardar palavra em wordA
                strncpy(words[j]->wordA, buf, 20);
                i = 0; //reset buf counter
                flag = 1; //switch to wordB
            }else{
                words[j] -> wordB = buf;
                i = 0;
                j++;
                flag = 0;
                words = realloc(sizeof(struct wordsCypher * (j + 1 ) ) );
            }
        }
    }

    // close pipe writing end
    //close(pipes_fd[WRITE_END]);

    /*
    // read file in chunks of BUFSIZE and write to pipe
    char buf[BUFSIZE];
    int bytes;
    while ((bytes = read(pipes_fd[READ_END], buf, BUFSIZE)) > 0) {
        //write(STDOUT_FILENO, buf, bytes); // write to STDOUT
    }

    if (bytes == -1) { // handle errors while reading from pipe
        fprintf(stderr, "Error while reading from pipe. Cause: %s\n",
                strerror(errno));
        close(pipes_fd[READ_END]);
        return EXIT_FAILURE;
    }

    // close pipe
    close(pipes_fd[READ_END]);*/
    return EXIT_SUCCESS;
}



int main(int argc, char *argv[]){
    pid_t pid;
    int pipes_fd[2];

    //Create and open pipe
    if(pipe(pipes_fd)<0){
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    if((pid=fork())<0){
        perror("fork error");
        exit(EXIT_FAILURE);
    }else if(pid>0){
        // parent
        /*
        int r = parent(argv[1], pipes_fd);

        // wait for child and exit
        if (waitpid(pid, NULL, 0) < 0) {
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        return r;*/
    }else{
        return child(pipes_fd);
    }
}
