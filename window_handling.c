#include "byteops.h"
#include "window_handling.h"

void scale_camera(drawing_context* context)
{	
	camera* curr_camera = &context->curr_camera;
	int curr_width, curr_height;
	SDL_GetWindowSize(context->curr_window, &curr_width, &curr_height);

	if (curr_width <=0)
		curr_width = 800;
	if (curr_height <=0)
		curr_height = 600;

	curr_camera->cell_size.x = (curr_width/curr_camera->scale_factor.x);
	curr_camera->cell_size.y = (curr_height/curr_camera->scale_factor.y);
	
}

drawing_context create_context(const char* title)
{
	drawing_context output;
	
	if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
		printf("error initializing SDL: %s\n", SDL_GetError());
	
	output.curr_window = SDL_CreateWindow(title,
					      SDL_WINDOWPOS_CENTERED,
					      SDL_WINDOWPOS_CENTERED,
					      800,  600, SDL_WINDOW_RESIZABLE);
	
	output.curr_renderer = SDL_CreateRenderer(output.curr_window, -1, 0);

	output.curr_camera.pos.x =0;
	output.curr_camera.pos.y = 0;
	output.curr_camera.scale_factor.x = 40;
	output.curr_camera.scale_factor.y = 30;

	scale_camera(&output);

	return output;
}

void drawing_routine(grid curr_grid, drawing_context *context)
{

	SDL_SetRenderDrawColor(context->curr_renderer, 51,51,51,SDL_ALPHA_OPAQUE);
	SDL_RenderClear(context->curr_renderer);
	
	SDL_SetRenderDrawColor(context->curr_renderer,255,255,255,SDL_ALPHA_OPAQUE);

	scale_camera(context);
	
	context->tmp_rect.w = context->curr_camera.cell_size.x;
	context->tmp_rect.h = context->curr_camera.cell_size.y;

	
	for (int x = context->curr_camera.pos.x; x < (context->curr_camera.pos.x+context->curr_camera.scale_factor.x); x++)
	{
		if (x>=curr_grid.width) break;

		for (int y = context->curr_camera.pos.y; y < (context->curr_camera.pos.y+context->curr_camera.scale_factor.y); y++)
		{
			if (y>=curr_grid.height) break;

			if (get_bit(&curr_grid, x,y))
			{
				context->tmp_rect.x = (x)*context->tmp_rect.w;
				context->tmp_rect.y = (y)*context->tmp_rect.h;
				SDL_RenderDrawRect(context->curr_renderer, &context->tmp_rect);
			}
		}
	}
	
/*
	SDL_Rect tmp_rect = {0,0,10,10};
	for (int i=0; i< curr_grid.width; i++)
		for (int j=0; j< curr_grid.height; j++)
		{
			if (get_bit(&curr_grid,i,j))
			{
				tmp_rect.x = i*tmp_rect.w;
				tmp_rect.y = j*tmp_rect.h;
				SDL_RenderDrawRect(context->curr_renderer, &tmp_rect);
			}
		}
*/
	SDL_RenderPresent(context->curr_renderer);	
}
