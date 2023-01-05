#ifndef _LAB6_UTILS_H_
#define _LAB6_UTILS_H_

#include "list.h"

#define PORT 9000
//#define ADDRESS_PATTERN "tcp://localhost:"
#define ADDRESS_PATTERN "tcp://127.0.0.1:"
#define SERVER_PATTERN  "tcp://*:"
#define PING_PATTERN    "inproc://ping-"
#define MAXSIZE         1024
#define MIN_INDEXES     8
#define END_COL         "\x1b[0m"
#define GREEN           "\x1b[32m"
#define min(a, b) a > b ? b : a;

#define check_null(foo, msg) do { \
if (foo == NULL) { perror(msg); exit(EXIT_FAILURE); } \
} while(0);

#define check_neg_one(foo, msg) do { int __res = foo; \
if (__res == -1) { perror(msg); exit(EXIT_FAILURE); } \
} while(0);

typedef struct
{
    char cmd[10];
    int num;
    int id;
    char str[1024];
} Message;

void send_msg(Message *m, void *socket);
void recv_msg(Message *m, void *socket);

int *__z_function(char *str, int size);
void print_array(int size, int *arr);
void print_part_of_str(char *str, int from, int to, char *res, int *len);
void find_substrings(Message *m);

void client_address_gen(int id, char *address);
char *server_address_gen(int id);
int ping(int id);
int echo(List *childs, int id, void *socket);


#endif