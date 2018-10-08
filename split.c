#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_number_words  40
#define string_max_size  200

void split(char* string, char* separators, char** words, int* count){	// string - вводимая для разбиения на слова строка, 
									// separators - строка символов-разделителей между словами,
       								 	// words - массив ссылок на слова в строке string,
									// count - ссылка на количество слов в строке string

    char* str = (char*) calloc (strlen(string) + 1, sizeof(char));	// копирование полученной строки (string)
    str = strncpy(str, string, strlen(string));				

    char* token;							
    token = strtok(str, separators);					// разделение копии строки на слова, token - новое слово
    int i = 0;								// счетчик слов(token)

    while(token != NULL){

	words[i] = (char*) calloc (strlen(token) + 1, sizeof(char));        // выделение памяти и добваление слова(token) в массив слов(words)
	words[i] = strncpy(words[i], token, strlen(token));		   
	i++;
	token = strtok(NULL, separators);
    }
    *count = i;    							    // изменение значения переменной count (количества слов в массиве слов)
}

void free_memory(char* str, char** words, int count){				// str - указатель на строку; words - массив ссылок; count - количество элементов в массиве words
    free(str);
    for (int j = 0; j < count; j++){
	    free(words[j]);
    }
    free(words);
}


void main () {
	
    char* str = (char*) calloc (string_max_size, sizeof(char));			// ввод строки для разбиения на слова
    fgets(str, string_max_size, stdin);						

    char** words = (char**) calloc (max_number_words, sizeof(char*));		// words - массив ссылок на полученые слова
    int count = 0;								// count - количество слов в строке
    char separators[] = " ,._\t\n";						// separators - строка символов разделителей
    
    split(str, separators, words, &count);

    printf("Вывод %d слов:\n", count);                                  	//проверка работы программы
    for (int j = 0; j < count; j++) {
	    printf("%s\n", words[j]);
    }

    printf("Введенная строка:\n%s", str);					

    free_memory(str, words, count);						
}

