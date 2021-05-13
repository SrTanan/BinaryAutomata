#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct {
	int x;
	int y;
}vector;

typedef struct{
	vector pos;
	vector cell_size;
	vector scale_factor;
}camera;

typedef struct{
	SDL_Window* curr_window;
	SDL_Renderer* curr_renderer;
	camera curr_camera;
	SDL_Rect tmp_rect;
}drawing_context;

void scale_camera(drawing_context* context);
drawing_context create_context(const char* title);
void drawing_routine(grid curr_grid, drawing_context *context);
