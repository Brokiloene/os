#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int size;
	char* text = NULL;
	while ((read(0, &size, sizeof(int))) != 0) {
		text = malloc(sizeof(char) * size);
		read(0, text, size * sizeof(char));
		if (text == NULL) { return 6; }
		for (int i = 0; i < size; i++) {
			int ch = text[i];
			if (ch >= 65 && ch <= 90) {
				ch += 32;
			}
			text[i] = ch;
		}
		write(1, &size, sizeof(int));
		write(1, text, size * sizeof(char));
		if (text[0] == '0' && size == 2) {break;}
	}
	free(text);
	return 0;
}