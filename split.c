#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void split(char* string, char* separators, char** words, int* count){	// string - вводимая строка, separators - срока символов разделителей,
       								 	// words - массив ссылок на полученые слова,								       				       	 		       // count - ссылка на количество слов в строке
    								 	
    char* token;
    token = strtok(string, separators);					// разделение на слова, token - новое слово
    int i = 0;								// счетчик token
    while(token != NULL){
	// FIXIT: прочитайте, как strtok работает. что она делает с исходной строкой.
	words[i] = (char*) calloc (strlen(token), sizeof(char));        // выделение памяти и добваление слова в массив words
	// FIXIT: в языке си так строки не кипируются - вы скопировали указатель только -> при смене одной строки поменяется и вторая
	words[i] = token;
	i++;
	token = strtok(NULL, separators);
    }
    *count = i;							        	
}

void main () {
    // FIXIT: вынесите выделение и освобождение памяти в отдельные ф-и. это, конечно, надуманное требование, но вы лишний раз с указателями попрактикуетесь
	
    char* str = (char*) calloc (100, sizeof(char));
    fgets(str, 200, stdin);						//ввод строки
    char** words = (char**) calloc (1, sizeof(char*));			// words - массив ссылок на полученые слова
    int count = 0;							// count - количество слов в строке
    char separators[] = " ,.-\n\t";					//separators - срока символов разделителей
    split(str, separators, words, &count);
    printf("Вывод слов:\n");						// проверка
    for (int j = 0; j < count; j++){
	 printf("%s\n", words[j]);
    }
    free(words);
}

