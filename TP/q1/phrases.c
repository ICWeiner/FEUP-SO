#include <stdio.h>
#include <string.h>



int main(int argc, char *argv[]){

	FILE *  fp;

	if( argc == 2){
		fp= fopen(argv[1], "r");
	}else if( argc == 3){
		if(argv[1] == "-l"){
			fp = fopen(argv[2], "r");
		}else{
			printf("unkown argument");
			return 1;
		}
		
	}else{
		printf("usage: phrases [-l] file\n");
		return 1;
	}

	if (!fp){
	    printf("couldn read file\n");
	    return 1;
	}

	int lines = 0;

	
	char currChar;
	//currChar = fgetc(fp);
	printf("[%d] ",lines + 1);

	while(currChar != EOF){
		if (currChar = fgetc(fp) ) == EOF;
        printf("%c",currChar);
        

        if(currChar == '.' || currChar == '!' || currChar == '?'){
        	if( (currChar = fgetc(fp) ) == EOF){
        		break;
        	}else{
				lines++;
				printf("\n[%d] ",lines + 1);
        	}
        }
    }
	fclose(fp);
}
