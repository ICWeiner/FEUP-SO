#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  FILE *f1;
  FILE *f2;

  // check if exactly two arguments are present
  if (argc != 3)
  {
    fprintf(stderr, "usage: addmx file1 file2\n");
    exit(EXIT_FAILURE);
  }

  // check if file one can be opened and is readable
  f1 = fopen(argv[1], "r");
  if (f1 == NULL)
  {
    fprintf(stderr, "error: cannot open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // check if file two can be opened and is readable
  f2 = fopen(argv[2], "r");
  if (f2 == NULL)
  {
    fprintf(stderr, "error: cannot open %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }

  // gets two first digits too check if matrices are equal
  int m1_x, m1_y, m2_x, m2_y;
  char buffer1, buffer2;

  fscanf(f1, "%d %c %d",&m1_x,&buffer1,&m1_y);
  fscanf(f2, "%d %c %d",&m2_x,&buffer2,&m2_y);

  if (m1_x != m2_x || m1_y != m2_y)
  {
    fprintf(stderr, "error: matrices have different sizes\n");
    exit(EXIT_FAILURE);
  } 

  int array1[m1_x][m1_y];
  int array2[m2_x][m2_y];

  for(int i=0; i<m1_x ;i++)
    for(int j=0; j<m1_y ;j++)
      fscanf(f1, "%d",&array1[i][j]);
  
  for(int i=0; i<m2_x ;i++)
    for(int j=0; j<m2_y ;j++)
      fscanf(f2, "%d",&array2[i][j]);

  // check if line from file one was read to char pointer
  /*if (fgets(l_one, 4, argv[1]) == NULL)
  {
    fprintf(stderr, "error: cannot read %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  // check if line from file two was read to char pointer
  if ((l_two, 4, argv[2]) == NULL)
  {
    fprintf(stderr, "error: cannot read %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }
  int result = strcmp(l_one, l_two);
  if (result != 0)
  {
    fprintf(stderr, "error: matrices have different sizes\n");
    exit(EXIT_FAILURE);
  } 

  // ler o ficheiro e por dentro do array
  // e depois meter esse array para dentro da memoria partilhada

  int fscanf(argv[1], )

      while ()

          int *shared_memory = mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
}

*/

  fclose(f1);
  fclose(f2);
}