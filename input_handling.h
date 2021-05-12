#include <curses.h>
#include <pthread.h>
#include <stdio.h>

typedef struct {
	unsigned char status;
	pthread_mutex_t lock;
	MEVENT curr_click;
	int curr_key;
}data_bus;

void *run(void* input_bus);
