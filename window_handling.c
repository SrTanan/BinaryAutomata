#include "byteops.h"
#include "window_handling.h"

drawing_context createContext(const char* title)
{
	drawing_context output;
	
	if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
		printf("error initializing SDL: %s\n", SDL_GetError());
	
	output.curr_window = SDL_CreateWindow(title,
					      SDL_WINDOWPOS_CENTERED,
					      SDL_WINDOWPOS_CENTERED,
					      800,  600, 0);
	
	output.curr_renderer = SDL_CreateRenderer(output.curr_window, -1, 0);

	return output;
}

void drawing_routine(grid* curr_grid, drawing_context *context)
{

	SDL_SetRenderDrawColor(context->curr_renderer, 51,51,51,SDL_ALPHA_OPAQUE);
	SDL_RenderClear(context->curr_renderer);
	
	SDL_SetRenderDrawColor(context->curr_renderer,255,255,255,SDL_ALPHA_OPAQUE);

	SDL_Rect tmp_rect = {0,0,5,5};
	
	for (int x = 0; x < curr_grid->width; x++)
	{
		for (int y = 0; y < curr_grid->height; y++)
		{
			if (get_bit(curr_grid, x,y))
			{
				tmp_rect.x = x*tmp_rect.w;
				tmp_rect.y = y*tmp_rect.h;
				SDL_RenderDrawRect(context->curr_renderer, &tmp_rect);
			}
		}
	}

	SDL_RenderPresent(context->curr_renderer);	
}
