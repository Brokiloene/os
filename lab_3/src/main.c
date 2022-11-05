// 12. Наложить K раз фильтр, использующий матрицу свертки, на матрицу, состоящую из 
// вещественных чисел. Размер окна 3х3

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#define CONV_SIZE 3
#define CONV_VAL (double) 1 / 9

double conv_matrix[CONV_SIZE][CONV_SIZE] = {{CONV_VAL, CONV_VAL, CONV_VAL}, {CONV_VAL, CONV_VAL, CONV_VAL}, {CONV_VAL, CONV_VAL, CONV_VAL}};

typedef struct {
	int conv_cnt;

	int thread_cnt;
	int thread_number;

	int new_im_size;
	int res_size;

	double *new_img_matrix;
	double *res_matrix;

} Thread_args;

void init_conv_matrix(double conv_matrix[][CONV_SIZE]) {
	for (int i = 0; i < CONV_SIZE; i++) {
		for (int j = 0; j < CONV_SIZE; j++) {
			conv_matrix[i][j] = CONV_VAL;
		}
	}
}

int get_ind(int i, int j, int size) {
	return i * size + j;
}

void thread_args_init(Thread_args *args, int conv_cnt, int thread_cnt, int new_im_size, int res_size, double *new_img_matrix, double *res_matrix) {

	int row_set = conv_cnt;
	for (int i = 0; i < thread_cnt; ++i) {
		args[i].thread_number = i;
		args[i].thread_cnt = thread_cnt;
		args[i].conv_cnt = conv_cnt;

		args[i].new_im_size = new_im_size;
		args[i].res_size = res_size;

		args[i].new_img_matrix = new_img_matrix;
		args[i].res_matrix = res_matrix;
	}
}

void print(int size, double *matrix) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%lf ", matrix[get_ind(i, j, size)]);
		}
		printf("\n");
	}
	printf("\n");
}

void print2(int size, double *matrix) {
	FILE *f;
	if ((f = fopen("text2.txt", "w")) == NULL) {
		perror("Error opening file");
    	return;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(f, "%lf\n", matrix[get_ind(i, j, size)]);
		}
	}
}

void *convolution(void *a) {
	Thread_args *args = (Thread_args *)a;

	int n_elems = CONV_SIZE * CONV_SIZE; //всего элементов в матрице
	int n_opers = n_elems / args->thread_cnt; //сколько операций на поток
	int n_rest = n_elems % args->thread_cnt; //остаток операций

	int start, end;
	if (args->thread_number == 0) {
		start = args->thread_number * n_opers;
		end = (args->thread_number + 1) * n_opers + n_rest; //первый поток делает больше
	} else {
		start = args->thread_number * n_opers + n_rest;
		end = (args->thread_number + 1) * n_opers + n_rest;
	}

	int row_offset = 0, col_offset = 0;
	while (CONV_SIZE + row_offset <= args->new_im_size) {
		col_offset = 0;
		while (CONV_SIZE + col_offset <= args->new_im_size) {
			for (int op = start; op < end; ++op) {
				int row = op / CONV_SIZE;
				int col = op % CONV_SIZE;

				args->res_matrix[get_ind(row_offset, col_offset, args->res_size)] += conv_matrix[row][col] * 
				args->new_img_matrix[get_ind(row_offset + row, col_offset + col, args->new_im_size)];
			}
		++col_offset;
		}
	++row_offset;
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 3) {
		perror("Too few arguments");
		return 2;
	}
	int conv_cnt = atoi(argv[1]);
	if (conv_cnt < 1) {
		perror("First argument shall be >=1");	
		return 2;
	}

	int thread_cnt = atoi(argv[2]);
	if (thread_cnt < 1) {
		perror("Second argument shall be >=1");
		return 2;	
	}

	int im_size; 
	printf("Enter a size of image matrix:\n");
	scanf("%d", &im_size);
	if (im_size < 3) {
		perror("Size of image matrix shall be >=3");
		return 2;
	}
	double img_matrix[im_size][im_size];

	printf("Enter an image matrix:\n");
	for (int i = 0; i < im_size; i++) {
		for (int j = 0; j < im_size; j++) {
			scanf("%lf", &img_matrix[i][j]);
		}
	}
	
	int res_size = im_size - CONV_SIZE + 1;
	double *res_matrix = (double *) calloc(res_size * res_size, sizeof(double));

	int new_im_size = im_size;
	double *new_img_matrix = (double *) malloc(sizeof(double) * new_im_size * new_im_size);
	memcpy(new_img_matrix, img_matrix, sizeof(double) * new_im_size * new_im_size);

	printf("Image matrix:\n");
	print(new_im_size, new_img_matrix);

	clock_t begin = clock();

	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * thread_cnt);
	Thread_args *th_args = (Thread_args *) malloc(sizeof(Thread_args) * thread_cnt);
	
	for (int i = 0; i < conv_cnt; ++i) { //применить k раз свертку
		if (i != 0) {
			new_im_size = res_size;
			new_img_matrix = (double*) realloc(new_img_matrix ,sizeof(double) * new_im_size * new_im_size);
			memcpy(new_img_matrix, res_matrix, sizeof(double) * new_im_size * new_im_size);
		}

		res_size = new_im_size - CONV_SIZE + 1;
		if (res_size < 0) { //матрицу свертки можно применить только к матрице размером >= 3x3
			res_size = 1; 
			break;
		}
		res_matrix = (double *) realloc(res_matrix, sizeof(double) * res_size * res_size);
		memset(res_matrix, 0, sizeof(res_matrix) * res_size * res_size);

		thread_args_init(th_args, conv_cnt, thread_cnt, new_im_size, res_size, new_img_matrix, res_matrix);	

		for (int j = 0; j < thread_cnt; ++j) {
			if (pthread_create(&threads[j], NULL, convolution, (void *) &th_args[j]) != 0) {
				perror("Failed to create thread");
				return 3;
			}
		}

		for (int j = 0; j < thread_cnt; ++j) {
			if (pthread_join(threads[j], NULL) != 0) {
				perror("Failed to join thread");
				return 3;
			}
		}

		//printf("intermediate res:\n");
		//print(res_size, res_matrix);
	}

	printf("Res matrix:\n");
	print(res_size, res_matrix);
	
	free(res_matrix);
	free(new_img_matrix);
	free(threads);
	free(th_args);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent: %lf sec\n", time_spent);

	return 0;
}