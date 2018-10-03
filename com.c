#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>


#define max_number_words  40
#define string_max_size  200

void split(char* string, char* separators, char** words, int* count){	// string - вводимая строка, separators - срока символов разделителей,
       								 	// words - массив ссылок на полученые слова,
									// count - ссылка на количество слов в строке

    char* str = (char*) calloc (strlen(string) + 1, sizeof(char));	// создаем копию полученной строки
    str = strncpy(str, string, strlen(string));				

    char* token;							
    token = strtok(str, separators);					// разделение на слова, token - новое слово
    int i = 0;								// счетчик token

    while(token != NULL){

	words[i] = (char*) calloc (strlen(token) + 1, sizeof(char));        // выделение памяти и добваление слова в массив words

	words[i] = strncpy(words[i], token, strlen(token));	
	i++;
	token = strtok(NULL, separators);
    }
    *count = i;    
}

void free_memory(char* str, char** words, int count){				
    free(str);
    for (int j = 0; j < count; j++){
	    free(words[j]);
    }
    free(words);
}


int main (int argc, char* argv[]) {

	FILE *fp;
	char name[] = "text.txt";
	fp = fopen(name, "r");
	if(fp == NULL) {
 		perror("ERROR OPENING FILE\n");						// проверка открытия файла
	}

	int size;									// количество строк
	int time;									// timeout в сек
	int num;									// буфер
	int count;									// переменная используемая в функции split 
        char separators[] = " \t\n";							// массив разделителей

	fscanf(fp, "%d %d", &size, &time);						// чтение первой строки в файле: количества строк и timeout для команд
	
	char** coms = (char**) calloc (size, sizeof(char*));				// массив строк, каждая строка является командой из файла 
	int* times = (int*) calloc (size, sizeof(int));					// массив времени начала исполнение команды в сек
        char** words = (char**) calloc (max_number_words, sizeof(char*));		// массив указателей на отдельные слова в определенной команде
	char* buffer = (char*) calloc (string_max_size, sizeof(char));			// буфер для считывания

	for(int i = 0; i < size; i++){							// считывание из файла
		fscanf(fp, "%d ", &num);
		times[i] = num;
		fgets(buffer, string_max_size, fp);
		coms[i] = (char*) calloc (strlen(buffer) + 1, sizeof(char));
		coms[i] = strncpy(coms[i], buffer, strlen(buffer));
	}
	
	
	int status;
	pid_t pid;
	for (int i = 0; i < size; i++){							// разделение на 2 процесса для каждой команды
		pid = fork();
		if (pid == 0){
			pid = fork();							// дочерний процесс делиться на исполняющий и контролирующий timeout
			if (pid == 0){
				sleep(times[i]);					
				split(coms[i], separators, words, &count);
				execvp(words[0], words);				// исполнение команды
			}else{
				sleep(times[i] + time);
				if (waitpid(pid, &status, WNOHANG) <= 0){		// завершение процесса (timeout)
					printf("THE %d PROCESS STOPPED\n", i + 1);
					kill(pid, 0);
				}
				exit(0);
			}		
		}	
	
	}	
	free_memory(buffer, coms, size);
	fclose(fp);
  	return 0;
}   
