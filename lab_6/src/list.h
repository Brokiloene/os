#ifndef _LIST_H_
#define _LIST_H_

typedef struct
{
	int size;
	int *values;
} List;

List *list_create();
int size(List *l);
void _resize(List *l);
void push_back(List *l, int num);
void print_list(List *l);
void list_delete(List *l, int num);
int list_find(List *l, int num);
void list_destroy(List *l);
int list_get(List *l, int ind);

#endif