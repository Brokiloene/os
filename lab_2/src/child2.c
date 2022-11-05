#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	//fprintf(stdin, "hello from child2\n");
	int size;
	char* text = NULL;
	while (read(0, &size, sizeof(int)) != 0) {
		text = malloc(sizeof(char) * size);
		if (read(0, text, size * sizeof(char)) == -1) {return 5;}
		if (text == NULL) { return 6; }

		int new_size = size;
		for (int i = 0, end = size - 1; i < end; i++) {
			if (text[i] == ' ' && text[i] == text[i + 1]) {
				for (int j = i, end = size - 1; j < end; j++) {
					text[j] = text[j + 1];
				}
				size--;
				i--;
			}
		}

		if (write(1, &new_size, sizeof(int)) == -1) { return 4; }
		if (write(1, text, new_size * sizeof(char)) == -1) { return 4; }
		if (text[0] == '0' && size == 2) {break;}
	}
	free(text);
	return 0;
}