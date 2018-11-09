#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
 
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SIZE 100000000
#define SUCCESS 0


//ускорение a(отношение времен) для n потоков:
//  при n = 2, a = 1.12
//  при n = 3, a = 1.21
//  при n = 4, a = 1.27

// Судя по вашим данным, у вас замедление, а не ускорение
// Попробуйте взять код ф-и get_wall_time для linux отсюда https://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows
// Кажется, что код написан верно и должны получить ускорение, а не замедление


double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time, NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

struct data{				// данный передаваемые каждой нити
	int step;	
	int* mass;
	int current;
	double sum;
	double disp;
	double average;
};

void* find_sum(void* argv){
	struct data* data_thr = (struct data*) argv;
	int new = data_thr->current + data_thr->step;
	double sum = 0;
	for(int i = data_thr->current; i < new; i++){
		sum += data_thr->mass[i];
	}
	data_thr->sum = sum;
}

void* find_disp(void* argv){
	struct data* data_thr = (struct data*) argv;
	int new = data_thr->current + data_thr->step;
	float disp = 0;
	// FIXIT: у вас при подсчете среднего циклы от current до current + step ... здесь же от 0 до step
	// Кажется, что всего один вариант может быть правильным.
	for(int i = data_thr->current; i < new; i++){
		disp +=  (data_thr->mass[i] - data_thr->average) * (data_thr->mass[i] - data_thr->average);
	}
	data_thr->disp = disp;
}

int main()
{
	int num;				// количество нитей
	printf("Enter number of threads:\n");
	scanf("%d", &num);	
	// FIXIT: а что если SIZE не делится нацело на num? Кажется, что в данном случае мы потеряем несколько последних элементов.
	int step = SIZE / num;
	int create_status;
	int join_status;
	
	double average; 
	double disp;
	double total_sum = 0;
	double total_disp = 0;
	pthread_t* threads = (pthread_t* ) calloc (num, sizeof(pthread_t));
	int* mass = (int*) calloc (SIZE, sizeof(int));
	struct data* structs = (struct data* ) calloc (num, sizeof(struct data));
	structs[0].mass = mass;
	structs[0].step = step;
	structs[0].sum = 0;
	structs[0].disp = 0;
	structs[0].current = 0;
	structs[0].average = 0;
	for(int i = 1; i < num; i++){
		structs[i].current = structs[i - 1].current + step;
		structs[i].mass = mass;
		structs[i].sum = 0;
		structs[i].disp = 0;
		if (i == (num - 1)){
			structs[i].step = SIZE - (step * (num - 1));
		}
		else
			structs[i].step = step;
	}

	for(int i = 0; i < SIZE; i++){		//тривиальный случай для проверки
		mass[i] = 1;
	}
///вычисление среднего
	double wall0 = get_wall_time();
	double cpu0  = get_cpu_time();

	for(int i = 0; i < num; i++){
		pthread_create(&(threads[i]), NULL, find_sum, &(structs[i]));
		    if (create_status != 0) {
			printf("main error: can't create thread, status = %d\n", create_status);
			exit(ERROR_CREATE_THREAD);
		    }
	}
	for(int i = 0; i < num; i++){
		join_status = pthread_join(threads[i], (void**)NULL);
		if (join_status != SUCCESS) {
			exit(ERROR_JOIN_THREAD);
		}
	}

	for(int i = 0; i < num; i++){
		total_sum += structs[i].sum;
	}
	average = total_sum / SIZE;
///вычисление дисперсии
	for(int i = 0; i < num; i++){
		structs[i].average = average;
	}
	for(int i = 0; i < num; i++){
		create_status = pthread_create(&(threads[i]), NULL, find_disp, &(structs[i]));
		if (create_status != 0) {
			printf("main error: can't create thread, status = %d\n", create_status);
			exit(ERROR_CREATE_THREAD);
		}
	}

	for(int i = 0; i < num; i++){
		join_status = pthread_join(threads[i], (void**)NULL);
		if (join_status != SUCCESS) {
			exit(ERROR_JOIN_THREAD);
		}
	}
	for(int i = 0; i < num; i++){
		total_disp += structs[i].disp;
	}
	disp = total_disp / (SIZE - 1);
	double wall1 = get_wall_time();
	double cpu1  = get_cpu_time();
///
	double time_spent_cpu = cpu1 - cpu0;
	double time_spent_wall = wall1 - wall0;

	//printf("average: %f\ndispersion: %f\n", average, disp);	// вывод результатов
	printf("CPU time:%f\n", time_spent_cpu);
	printf("Wall time:%f\n", time_spent_wall);

	free(threads);
	free(mass);
	free(structs);
	return 0;
}
