CC     = gcc
CFLAGS = -Wall -g
BINS   = main_static_v1 main_static_v2 lib5_v1.so lib5_v2.so main_dynamic

all: $(BINS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
%.a: %.o
	ar rcs $@ $<
%.so: %.o
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^ -lc

main_static_v1: main_static.c lib5_v1.a
	$(CC) $(CFLAGS) $^ -o main_static_v1

main_static_v2: main_static.c lib5_v2.a
	$(CC) $(CFLAGS) $^ -o main_static_v2

main_dynamic: main_dynamic.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -l5_v1 -l5_v2


clean:
	rm *.a *.so main_static_v1 main_static_v2 main_dynamic