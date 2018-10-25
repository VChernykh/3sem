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
		// область видимости переменных лучше делать минимальной, т.е. раз num_1 используется только в цикле while, то ровно перед ним её и объявить
		
		// FIXIT: чтобы не дублировать код, воспользуйтесь, пожалуйста, тернарным оператором:
		fd_r = open(atoi(argv[1]) == 1 ? name_s : name_e, O_RDONLY);

		if (fd_r == - 1){
			printf("Невозможно открыть fifo\n");
			exit(-1);
		}

		int num;
		// Кажется, что смысл не изменится, а лишние вопросы исчезнут, если назовете переменные просто buffer и num 
		do{
			char* buffer = (char*) calloc(max_size, sizeof(char));	// читает из fifo 
			num = read(fd_r, buffer, max_size - 1);			// выводит полученную строку		
			printf("%s", buffer);
			free(buffer);
		}while(num);
	
	}
	

	if (pid > 0){
		// Замечания и пожелания для данной части кода аналогичны

		fd_w = open(atoi(argv[1]) == 0 ? name_s : name_e, O_WRONLY);

		if (fd_w == - 1){
			printf("Невозможно открыть fifo\n");
			exit(-1);
		}

		int num;
		do{
			char* buffer = (char*) calloc(max_size, sizeof(char));			
			fgets(buffer, max_size - 1, stdin);				// считывает вводимую строку
			num = write(fd_w, buffer, max_size - 1);			// записывает в файл fifo
			free(buffer);
		}while(num);
	}

	close(fd_r);
	close(fd_w);
	return 0;
}
