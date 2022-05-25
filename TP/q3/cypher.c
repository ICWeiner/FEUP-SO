#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFSIZE 1024
#define READ_END 0
#define WRITE_END 1


struct wordsCypher {
	char* wordA;
	int Asize;
	char* wordB;
	int Bsize;
};

int str_compare(char str1[], char str2[], int size){
    //printf("%s %s \n",str1,str2);
    for( int i=0; i < size; i++) {
        //printf("%c %c",str1[i],str2[i]);
        if( str1[i] == '\0' || str2[i] == '\0')
            return -1;
        if (str1[i] != str2[i]) {
            return -1;
        }
    }
    return 0;
}

int concat(char *text,char *add, int add_start, int add_size){
    for(int i = add_start, j = 0; i < add_size + add_start; i++ , j++){
        text[i] = add[j];
    }

    //text[add_start +add_size + 1] = ' ';
    return 0;
}

struct wordsCypher* read_into_struct(FILE* fp){
        int i = 0,j = 0, flag = 0;//i used for counting chars, j used for counting word pairs, flag used for words
        //int words_quant;

        char c;
        size_t size = 8;
        char* buf = malloc( sizeof(char) * size);

        struct wordsCypher* words = malloc( sizeof(struct wordsCypher) * 1) ;//array of structs

        //TODO: add error verification to malloc/realloc?

        while ( ( c = fgetc(fp)) != EOF){
            //printf ("%c", c);
            if ( i == size){
                buf = realloc(buf, (size = size*2) );
            }

            if(c!=' '){
                buf[i] = c;
                i++;
            }else if(flag == 0 || c == '\n'){// guardar palavra em wordA
                buf[i] = '\0';
                words[j].wordA = malloc(i * sizeof(char) );
                words[j].Asize = i;

                strncpy(words[j].wordA, buf, i);

                free(buf); // erase and reallocate buffer
                size = 8;
                buf = malloc( sizeof(char) * size);

                i = 0; //reset buf counter
                flag = 1; //switch to wordB
            }else if(flag == 1 || c == '\n'){
                buf[i] = '\0';
                words[j].wordB = malloc(i * sizeof(char) );
                words[j].Bsize = i;

                strncpy(words[j].wordB, buf, i);

                free(buf);
                size = 8;
                buf = malloc( sizeof(char) * size);

                //printf("\npalavra 1: %s palavra 2: %s ",words[j].wordA,words[j].wordB);

                i = 0;
                j++;
                flag = 0;

                //struct wordsCypher *temp = realloc(words,sizeof(struct wordsCypher) * (j + 1) );

                words =  realloc(words, sizeof(struct wordsCypher) * (j + 1)  );
            }
        }

        words[j].wordB = malloc(i * sizeof(char) ); //TO-DO:last word isnt being stored in the cycle, probably sohuld fix that
        words[j].Bsize = i;
        strncpy(words[j].wordB, buf, i);

        //words_quant = j + 1;//read above comment

        return words;
};

int parent(int pipes_fd[2]){
	//fscanf(stdin,"%s",reading);

	/*if (file_fd == -1) {
		fprintf(stderr, "Failed to open file '%s'. Cause: %s\n", file,
				strerror(errno));
		close(pipes_fd[WRITE_END]);
		return EXIT_FAILURE;
	}*/

	close(pipes_fd[READ_END]);//TODO: é necessario error prevention para stdin/stdout

	char buffer[10];
	char space[1];
	space[0] = ' ';

	while(fscanf(stdin,"%s",buffer) != EOF){
		int len = strlen(buffer);
		//printf("%s %ld",reading ,strlen(reading));
		if (write(pipes_fd[1], buffer, len) == -1 || write(pipes_fd[1], space, 1) == -1 ) {//write to pipe word by word, add space after word
			fprintf(stderr, "Failed to write to pipe. Cause: %s\n",
					strerror(errno));
		}
	}

	close(pipes_fd[WRITE_END]);//close 
	
	/*while ((bytes = read(file_fd, buf, BUFSIZE)) > 0) {
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
	}*/

	close(pipes_fd[WRITE_END]);
	//close(file_fd);
	return EXIT_SUCCESS;
}

int child(int pipes_fd[2]){
	FILE* fp = fopen("cypher.txt","r");

	if (fp == NULL) {
		close(pipes_fd[WRITE_END]);
		return EXIT_FAILURE;
	}else{

        struct wordsCypher* words = read_into_struct(fp);

		close(pipes_fd[WRITE_END]);

		// read pipe in chunks
		/*char pipe_buf[231];
		int bytes;
		while ((bytes = read(pipes_fd[READ_END], pipe_buf, 231)) > 0) {
			//printf("%s\n",pipe_buf);
			//printf("%d\n",bytes);
			write(STDOUT_FILENO, pipe_buf, bytes);
		}*/

		char read_word[16];
        char text[BUFSIZE];// reallocar se necessario
        int text_counter = 0;
        int words_quant = sizeof(struct wordsCypher) / sizeof (words);


        FILE *pipe_read_stream = fdopen(pipes_fd[READ_END],"r");

		while(fscanf(pipe_read_stream,"%s",read_word) != EOF){
			int read_len = strlen(read_word);
            int j = 0;
            for(int i = 0; i < words_quant ; i++,j++ ){
                if(str_compare(words[i].wordA,read_word,words[i].Asize) == 0){// words are the same
                    concat( text,words[i].wordB,text_counter,words[i].Bsize);
                    text_counter += words[i].Bsize ;
                    text[text_counter] = ' ';
                    text_counter++;
                    break;
                    //printf(" '%s' is equal to '%s', apparently\n",read_word,words[i].wordA);

                    //trocar e meter em text[]
                    //adicionar carateres especiais, se estes existirem
                }else if(str_compare(words[i].wordB,read_word,words[i].Bsize) == 0){
                    concat( text,words[i].wordA,text_counter,words[i].Asize);
                    text_counter += words[i].Asize ;
                    text[text_counter] = ' ';
                    text_counter++;
                    break;
                    //printf("'%s' is equal to '%s', apparently\n",read_word,words[i].wordB);

                    //trocar e meter em text[]
                    //adicionar carateres especiais, se estes existirem
                }
            }
            if (j == words_quant){
                concat( text,read_word,text_counter,read_len);
                text_counter+= read_len;
                text[text_counter] = ' ';
                text_counter++;

                //printf("'%s' is NOT equal to '%s' or '%s', apparently\n",read_word,words[i].wordA,words[i].wordB);
            }

			//printf("%c %ld\n",read_word[len - 1] ,strlen(read_word));
			//printf("%s \n",read_word);
		}

        printf("%s",text);

		//write(STDOUT_FILENO, "ola", 3);

		/*if (bytes == -1) { // handle errors while reading from pipe
			fprintf(stderr, "Error while reading from pipe. Cause: %s\n",
					strerror(errno));
			close(pipes_fd[READ_END]);
			return EXIT_FAILURE;
		}*/

		// close pipe*/
		close(pipes_fd[READ_END]);
		return EXIT_SUCCESS;

	}
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
		//int r = parent(stdin, pipes_fd);

		int r = parent(pipes_fd);

		// wait for child and exit
		if (waitpid(pid, NULL, 0) < 0) {
			fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}



		return r;
	}else{
		return child(pipes_fd);
	}
}
