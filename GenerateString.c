#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

void GenerateString(int n, char* string) {  	// n - итераций, string - генерируемая строка
	int border = 0;				//позиция элемента в массиве, 
       				 		//относительно которого происходит отражение
	string[border] = 'a';
	char start = 'a'; 			//начальный элемент на позиции border
	int step = 0;  				// счетчик итераций
	while (n > step) {
		step++;
		border = (1 << step) - 1;
		start += 1;
		string[border] = start;
		/*
		V: лучше использовать готовую ф-ю strncpy, чем писать свою реализацию
		*/
		for (int i = 1; i <= border; i++) {  
			string[border + i] = string[i - 1];
		}
	}
}

int main() {
	int n;					//задаваемое число итераций (1 шаг - "aba")
	scanf("%d", &n);
	char* string = (char*)malloc((1 << (n + 1)));
	GenerateString(n, string);
	printf("%s\n", string);
	free(string);
	return 0;
}

