//14 вариант Child1 переводит строки в нижний регистр. Child2 убирает все задвоенные пробелы.

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	// if (argc < 2) {
	// 	return 0;
	// }
	// int size;
	// size = atoi(argv[1]) + 1;
	// if (size <= 1) {
	// 	return 0;
	// }
	
	int fd[3][2];
	for (int i = 0; i < 3; i++) {
		if (pipe(fd[i]) == -1) { return 2; }
	}
	
	int pid = fork();

	if (pid == -1) { return 3; }
	if (pid == 0) { // 65--90 +32
		close(fd[1][0]);
		close(fd[0][1]);
		close(fd[2][0]);
		close(fd[2][1]);

		dup2(fd[0][0], STDIN_FILENO);
		dup2(fd[1][1], STDOUT_FILENO);

		close(fd[0][0]);
		close(fd[1][1]);

		execlp("./child", "child", (char *)NULL);
	}

	int pid2 = fork();
	if (pid2 == -1) { return 3; }
	if (pid2 == 0) { //child 2
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][0]);

		dup2(fd[1][0], STDIN_FILENO);
		dup2(fd[2][1], STDOUT_FILENO);

		close(fd[1][0]);
		close(fd[2][1]);
		execlp("./child2", "child2", (char *)NULL);

		
	}
	close(fd[0][0]);
	close(fd[1][0]);
	close(fd[1][1]);
	close(fd[2][1]);

	
	size_t len = 0;
	int size = 0;
	char* text = NULL;
	do {
		size = getline(&text, &len, stdin);
		text[size - 1] = '\0';
		if (size == -1) {return 6;}
		if (write(fd[0][1], &size, sizeof(int)) == -1) { return 4; }
		if (write(fd[0][1], text, size * sizeof(char)) == -1) { return 4; }
		
		if (read(fd[2][0], &size, sizeof(int)) == -1) { return 5; }
		if (read(fd[2][0], text, size * sizeof(char)) == -1) { return 5; }	
		
		if (text[0] != '0' && size != 2) {
			printf("%s\n", text);
		}
		
	} while (text[0] != '0' && size != 2);
	free(text);
	
	wait(NULL);
	wait(NULL);

	close(fd[0][1]);
	close(fd[2][0]);

	return 0;
}