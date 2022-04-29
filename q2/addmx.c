#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
  // check if exactly two arguments are present 
  if(argc != 3){
    fprintf(stderr,"usage: addmx file1 file2\n");
    exit(EXIT_FAILURE);
  }

  // check if file one can be opened and is readable
  int f_one = open(argv[1], O_RDONLY);
  if (f_one == -1) {
    fprintf(stderr,"error: cannot open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // check if file two can be opened and is readable
  int f_two = open(argv[2], O_RDONLY);
  if (f_two == -1) {
    fprintf(stderr,"error: cannot open %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  char* l_one;
  char* l_two;

  // check if line from file one was read to char pointer
  if(fgets (l_one, 4, argv[1])==NULL) {
    fprintf(stderr,"error: cannot read %s\n",argv[1]);
    exit(EXIT_FAILURE);
  }

  // check if line from file two was read to char pointer
  if(fgets (l_two, 4, argv[2])==NULL) {
    fprintf(stderr,"error: cannot read %s\n",argv[2]);
    exit(EXIT_FAILURE);
  }

  int result = strcmp(l_one, l_two);
  if(result != 0){
    fprintf(stderr,"error: matrices have different sizes\n");
    exit(EXIT_FAILURE);
  }
}