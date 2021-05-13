#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct{
	SDL_Window* curr_window;
	SDL_Renderer* curr_renderer;
}drawing_context;

drawing_context createContext(const char* title);
void drawing_routine(grid* curr_grid, drawing_context *context);
