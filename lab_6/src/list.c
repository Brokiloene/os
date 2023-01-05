#include "list.h"

#include <stdio.h>
#include <stdlib.h>

List *list_create()
{
	List *l = (List *) malloc(sizeof(List));
	l->values = (int *) malloc(sizeof(int));
	l->size = 0;
	return l;
}

int size(List *l)
{
	return l->size;
}

void __resize(List *l)
{
	l->values = realloc(l->values, sizeof(int) * (l->size));
}

void push_back(List *l, int num)
{
	l->size++;
	__resize(l);
	l->values[l->size - 1] = num;
}

void print_list(List *l)
{
	int sz = size(l);
	// if (sz == 0) {
	// 	return;
	// }
	for (int i = 0; i < sz; ++i) {
		printf("%d ", l->values[i]);
	}
	printf("\n");
}

void list_delete(List *l, int num)
{
	int sz = size(l);
	for (int i = 0; i < sz; ++i) {
		if (l->values[i] == num) {
			for (int j = i; j < sz - 1; ++j) {
				l->values[j] = l->values[j + 1];
			}
			l->size--;
			break;
		}
	}
	if (l->size != 0) {
		__resize(l);
	}
}

int list_find(List *l, int num)
{
	int sz = size(l);
	for (int i = 0; i < sz; ++i) {
		if (l->values[i] == num) {
			return 1;
		}
	}
	return -1;
}


void list_destroy(List *l)
{
	free(l->values);
	free(l);
}

int list_get(List *l, int ind)
{
	int sz = size(l);
	if (ind < sz) {
		return l->values[ind];
	} else {
		return -1;
	}
}
