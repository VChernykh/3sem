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

#define TABLE_LIMIT 2
#define DISHES_TYPES_QUANTITY 4
#define END 39

void sem(int count, int number, int semid){
        struct sembuf mysembuf;

        mysembuf.sem_op = number;        
	mysembuf.sem_num = count;

        mysembuf.sem_flg = 0;
        semop(semid, &mysembuf, 1);

}

int main()
{
        int time;
        char type;
        int mesgid, semid;
        key_t key;
	int time_sleep;	
	struct msgbuf{
                long mestype;
                char dish_type;
        }buf;

        char pathname[] = "messg.c";
        key = ftok(pathname, 0);
        if((mesgid = msgget(key, 0)) > 0){
		msgctl(mesgid, IPC_RMID, 0);
	}
	mesgid = msgget(key, 0666 | IPC_CREAT);
        if((semid = semget(key, 1, 0)) > 0){
                semctl(semid, IPC_RMID, 0);
        }
        semid = semget(key, 1, 0666 | IPC_CREAT);
	printf("\nCLEANER waiting for washer\n");

        FILE* cleaning_time = fopen("cleaning_time.txt", "r");
        int number_types;
	fscanf(cleaning_time, "%d", &number_types); 
	int* times_clean = (int*) calloc(number_types + 1, sizeof(int));
        for(int i = 0; i < number_types; i++){
        	fscanf(cleaning_time, "%d", &type);
                fscanf(cleaning_time, "%d", &time);
        	times_clean[(int)type] = time;
        }

	while(msgrcv(mesgid, &buf, sizeof(char), 1, 0)){
		
		sem(0, 1, semid);
		if (buf.dish_type == END){
			printf("CLEANER finished all work\n");
			return 0;
		}
		type = (char)buf.dish_type;
		printf("\n---\nCLEANER began to wipe: %d\n", type);

                time_sleep = times_clean[(int)type];
                sleep(time_sleep);
                
		printf("CLEANER finished to wipe: %d\n---\n", type);
        }
	
	fclose(cleaning_time);
	return 0;
}
