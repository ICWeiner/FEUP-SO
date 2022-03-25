#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	
	if(argc == 2){
		FILE *file = fopen(argv[1],"r");
		while(!feof(file)){
			char buffer[100];
			size_t size = fread(buffer,sizeof(char),99,file);
			buffer[size] = '\0';
			printf("%s",buffer );
		}
		printf("\n");	
	}
	return 0;
}