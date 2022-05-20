#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  FILE *f1;
  FILE *f2;

  // check if exactly two arguments are present
  if (argc != 3) {
    fprintf(stderr, "usage: addmx file1 file2\n");
    exit(EXIT_FAILURE);
  }

  // check if file one can be opened and is readable
  f1 = fopen(argv[1], "r");
  if (f1 == NULL) {
    fprintf(stderr, "error: cannot open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // check if file two can be opened and is readable
  f2 = fopen(argv[2], "r");
  if (f2 == NULL) {
    fprintf(stderr, "error: cannot open %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  // gets two first digits too check if matrices are equal
  int m1_x, m1_y, m2_x, m2_y;
  char buffer1, buffer2;

  fscanf(f1, "%d %c %d", &m1_x, &buffer1, &m1_y);
  fscanf(f2, "%d %c %d", &m2_x, &buffer2, &m2_y);

  if (m1_x != m2_x || m1_y != m2_y) {
    fprintf(stderr, "error: matrices have different sizes\n");
    exit(EXIT_FAILURE);
  }

  int array1[m1_x * m1_y];
  int array2[m2_x * m2_y];

  size_t size_array1 = sizeof(array1);
  size_t size_array2 = sizeof(array2);

  int *marray1 = mmap(NULL, size_array1 , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  int *marray2 = mmap(NULL, size_array2 , PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);

  for (int i = 0; i < m1_x * m1_y; i++) {
    fscanf(f1, "%d", &marray1[i]);
  }

  for (int i = 0; i < m2_x * m2_y; i++) {
    fscanf(f2, "%d", &marray2[i]);
  }

  pid_t pid; // process id's
  int i;
  for(i=0; i<m1_y ;i++){
    if ((pid = fork()) == -1) {
      perror("fork");
      return EXIT_FAILURE;
    }
    if(pid == 0) break;
  }

  if(pid == 0){
    while(i< m1_x * m1_y){
      for(int j=0; j<m1_y ;j++){
        marray1[i + j] += marray2[i + j];
      }
      i += m1_y;
    }
  }
  if(pid != 0){
    if (waitpid(pid, NULL, 0) == -1) {
    perror("wait");
    return EXIT_FAILURE;
    }
    fprintf(stdout, "%dx%d\n", m1_x, m2_y);
    for (int i = 0; i < m1_x * m1_y; i++) {
      fprintf(stdout, "%d\t", marray1[i]);
      if(i % 3 == 0 && i != 0)
      fprintf(stdout, "\n");
    }
  }

  fprintf(stdout, "\n");

  fclose(stdout);
  fclose(f1);
  fclose(f2);
}