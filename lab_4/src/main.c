// 14 вариант Child1 переводит строки в нижний регистр. Child2 убирает все задвоенные пробелы.

#include <stdio.h>    // STDOUT_FILENO & size_t & printf & getline
#include <sys/mman.h> // mmap & shm_open
#include <sys/stat.h> // S_xxxx
#include <fcntl.h>    // O_xxxx   
#include <unistd.h>   // ftruncate      
#include <string.h>   // memcpy
#include <semaphore.h>// sem_t
#include <sys/wait.h> // wait

#include "lab4_utils.h"

void print(char *str, size_t size)
{
	for (size_t i = 0; i < size; ++i) {
		printf("%c", str[i]);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	char *shmpath = "/shmlab4";
	int fd = shm_open(shmpath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	check_err(fd, "shm_open error");

	check_err(ftruncate(fd, sizeof(Shared_str)), "ftruncate error");
	Shared_str *shr_str = mmap(NULL, sizeof(*shr_str), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (shr_str == MAP_FAILED) {check_err(-1, "mmap error")};

	check_err(sem_init(&shr_str->p_to_c1, 1, 0), "sem_init error");
	check_err(sem_init(&shr_str->c1_to_c2, 1, 0), "sem_init error");
	check_err(sem_init(&shr_str->c2_to_p, 1, 0), "sem_init error");

	int pid = fork();
	check_err(pid, "fork error");
	if (pid == 0) { // child1
		check_err(execlp("./child", "child", (char *)NULL), "execlp child1 error");
	}

	int pid2 = fork();
	check_err(pid, "fork error");
	if (pid2 == 0) { // child2
		check_err(execlp("./child2", "child2", (char *)NULL), "execlp child2 error");
	}

	size_t len = 16;
	int size = 0;
	char* text = NULL;
	size = getline(&text, &len, stdin);
	check_err(size, "getline error");
	text[size - 1] = '\0';
	while(text[0] != '0') {
		if (size > MAX_SIZE) {
			printf("size of string shall be <= 1024\n");
			break;
		}
		shr_str->size = size;
		memcpy(&shr_str->str, text, size);
		check_err(sem_post(&shr_str->p_to_c1), "sem_post error");

		check_err(sem_wait(&shr_str->c2_to_p), "sem_wait error");
		print(shr_str->str, shr_str->size);

        size = getline(&text, &len, stdin);
		check_err(size, "getline error");
		text[size - 1] = '\0';
	}
	shr_str->size = STOP;
	check_err(sem_post(&shr_str->p_to_c1), "sem_post error");
	free(text);

	check_err(munmap(shr_str, sizeof(*shr_str)), "munmap error");
	shm_unlink(shmpath);
	wait(NULL);
	wait(NULL);
	
	return 0;
}
