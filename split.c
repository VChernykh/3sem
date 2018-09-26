#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	// FIXIT: прочитайте, как strtok работает. что она делает с исходной строкой.
	words[i] = (char*) calloc (strlen(token) + 1, sizeof(char));        // выделение памяти и добваление слова в массив words

	// FIXIT: в языке си так строки не кипируются - вы скопировали указатель только -> при смене одной строки поменяется и вторая
	words[i] = strncpy(words[i], token, strlen(token));	
	i++;
	token = strtok(NULL, separators);
    }
    *count = i;    
}

void free_memory(char* str, char** words, int* count){				// функция очистки памяти
    free(str);
    for (int j = 0; j < *count; j++){
	    free(words[j]);
    }
    free(words);
}


void main () {
    // FIXIT: вынесите выделение и освобождение памяти в отдельные ф-и. это, конечно, надуманное требование, но вы лишний раз с указателями попрактикуетесь
	
    char* str = (char*) calloc (string_max_size, sizeof(char));			// ввод строки
    fgets(str, string_max_size, stdin);						

    char** words = (char**) calloc (max_number_words, sizeof(char*));		// words - массив ссылок на полученые слова
    int count = 0;								// count - количество слов в строке
    char separators[] = " ,.-\t\n";						// separators - срока символов разделителей
    
    split(str, separators, words, &count);

    printf("Вывод %d слов:\n", count);                                  	// проверка
    for (int j = 0; j < count; j++){
	    printf("%s\n", words[j]);
  }
    printf("Введенная строка:\n%s ", str);					//проверка изменений в введенной строке

    free_memory(str, words, &count);						//очистка памяти
}

