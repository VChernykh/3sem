#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define SIZE 200

void find_file(char* current_dir, int need_level, int level, char* name, int* found){

	struct dirent* parametrs;
	struct stat buffer;

	DIR* current = opendir(current_dir);
	parametrs = readdir(current);

	if(level > need_level){
		return;
	}

	while(parametrs != 0){
		if((strcmp(parametrs->d_name, "..") != 0) && (strcmp(parametrs->d_name, ".") != 0)){
		
			char* path_copy = (char*) calloc(SIZE, sizeof(char));
			strcat(path_copy, current_dir);
			strcat(path_copy, "/");
			strcat(path_copy, parametrs->d_name);

			stat(path_copy, &buffer);

			if(S_ISDIR(buffer.st_mode)){
				find_file(path_copy, need_level, level + 1, name, found);
			}

			if(S_ISREG(buffer.st_mode)){
				if(strcmp(parametrs->d_name, name) == 0){
					printf("The path to the file: %s\n", current_dir);
					(*found)++;
					return;
				}
			}
		}
		parametrs = readdir(current);	
	}
	return;
}

int main(int argc, char* argv[])
{
	char* answer = (char*) calloc (SIZE, sizeof(char));
	strcat(answer, argv[2]);
	int need_level;
	int found = 0;
	printf("Enter maximum level\n");
	scanf("%d", &need_level);

	find_file(argv[1], need_level, 0, answer, &(found));
	printf("Found:%d times\n", found);

	free(answer);
	return 0;
}
