#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define max_size 30						
#define mask 0777						// маска для fifo


int main(int argc, char* argv[]){

	char name_s[] = "first.fifo";
	char name_e[] = "second.fifo";

	mkfifo(name_s, mask);					// создаем два файла fifo с указанными именами
	mkfifo(name_e, mask);
	
	int fd_r;						// дескрипторы файлов fifo
	int fd_w;

	printf("Отладочная информация:\nимена fifo - %s and %s\nномера соответсвующих дескрипторов fifo - %d and %d\n", name_s, name_e, fd_r, fd_w);
	// выводим отладочную информацию
	pid_t pid;
	pid = fork();						// разделяем процесс на два: для считывания из fifo и ввода в fifo
	
	if (pid == 0){				
		int num_1;
		if(atoi(argv[1]) == 1){
			fd_r = open(name_s, O_RDONLY);
			if (fd_r == - 1){
				printf("Невозможно открыть first.fifo\n");
				exit(-1);
			}
		}else{

			fd_r = open(name_e, O_RDONLY);
			if (fd_r == - 1){
				printf("Невозможно открыть second.fifo\n");
				exit(-1);
			}
		}

		do{
			char* buffer_1 = (char*) calloc(max_size, sizeof(char));	// читает из fifo 
			num_1 = read(fd_r, buffer_1, max_size - 1);			// выводит полученную строку		
			printf("%s", buffer_1);
			free(buffer_1);
		}while(num_1);
	
	}
	

	if (pid > 0){						
		int num_2;
		if(atoi(argv[1]) == 0){
			fd_w = open(name_s, O_WRONLY);
			if (fd_w == - 1){
				printf("Невозможно открыть first.fifo\n");
				exit(-1);
			}
		}else{
			fd_w = open(name_e, O_WRONLY);
			if (fd_w == - 1){
				printf("Невозможно открыть second.fifo\n");
				exit(-1);
			}
		}

		do{
			char* buffer_2 = (char*) calloc(max_size, sizeof(char));			
			fgets(buffer_2, max_size - 1, stdin);				// считывает вводимую строку
			num_2 = write(fd_w, buffer_2, max_size - 1);			// записывает в файл fifo
			free(buffer_2);
		}while(num_2);
	}

	close(fd_r);
	close(fd_w);
	return 0;
}

