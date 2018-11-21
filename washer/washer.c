#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <ctype.h>

#define TABLE_LIMIT 10
#define END 39

void sem(int count, int number, int semid){
        struct sembuf mysembuf;

	mysembuf.sem_op = number; 
	mysembuf.sem_num = count;               

        mysembuf.sem_flg = 0;
        semop(semid, &mysembuf, 1);

}

int main(){
	int time;
	char type;
	int msgid, semid;
	key_t key;
	int time_sleep;
 	struct msgbuf{
		long mestype;
		char dish_type;
	}buf;

	char pathname[] = "messg.c";
	key = ftok(pathname, 0);
	msgid = msgget(key, 0);
        semid = semget(key, 1, 0);
        sem(0, TABLE_LIMIT, semid);


	FILE* washing_time = fopen("washing_time.txt", "r");
	int number_types;
	fscanf(washing_time, "%d", &number_types); 
	int* times_wash = (int*) calloc(number_types + 1, sizeof(int));
	for(int i = 0; i < number_types; i++){
        	fscanf(washing_time, "%d", &type);
                fscanf(washing_time, "%d", &time);
        	times_wash[(int)type] = time;
	}

	FILE* stack_dishes = fopen("stack_dishes.txt", "r");
	int number_dishes;
	fscanf(stack_dishes, "%d", &number_dishes);
	for(int i = 0; i < number_dishes; i++){
		fscanf(stack_dishes, "%d", &type);
		printf("\n---\nWASHER started to wash: %d\n", type);
		time_sleep = times_wash[(int)type];
                sleep(time_sleep);
		sem(0, -1, semid);
		buf.mestype = 1;
		buf.dish_type = type;
		msgsnd(msgid, &buf, sizeof(char), 0);
		printf("WASHER put on the table: %d type\n---\n", type);
	}
	buf.mestype = 1;
	buf.dish_type = END;
        msgsnd(msgid, &buf,  sizeof(char), 0);
	printf("\nWASHER finished all work\n");
	
	free(times_wash);	
	fclose(washing_time);
	fclose(stack_dishes);
	return 0;
}

