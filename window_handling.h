#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct{
	SDL_Window* curr_window;
	SDL_Renderer* curr_renderer;
}drawing_context;

typedef struct{
	grid* grid;
	pthread_mutex_t* lock;
	bool close;
}data_bus;

void* run_thread(void* args);
