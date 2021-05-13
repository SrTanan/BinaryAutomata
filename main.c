#include "byteops.h"
#include "window_handling.h"

void conway_turn(grid* input)
{	
	grid next_turn;
	next_turn.width = input->width;
	next_turn.height = input->height;
	next_turn.size = input->size;
	next_turn.buffer =  malloc(next_turn.size);

	clear_buffer(&next_turn);

	for (int x=0; x<input->width; x++)
	{
		for (int y=0; y< input->height; y++)
		{
			unsigned char neighbour_count = count_bits( get_neighbours(input, x,y) );

			if ( get_bit(input, x,y) )
			{
				set_bit(&next_turn,x,y,true);

				
				if ( neighbour_count < 2 ) set_bit(&next_turn,x,y,false);
				if ( neighbour_count > 3 ) set_bit(&next_turn,x,y,false);
				
				
			}
			else if (neighbour_count ==  3) set_bit(&next_turn, x, y, true);
		}
	}

	for (int i=0; i<next_turn.size; i++)
	{
		input->buffer[i] = next_turn.buffer[i];
	}
	free(next_turn.buffer);
}

int main(void)
{

	bool pause = false;
	bool close = false;
	bool clicking =  false;
	bool shift = false;

	drawing_context context = create_context("BinaryAutomata");
	SDL_Event event;

	grid screen;
	screen.width = 700;
	screen.height = 600;
	screen.size = (screen.width*screen.height)/8;
	screen.buffer = malloc(screen.size);
	clear_buffer(&screen);

	
	for (;;)
	{

		while(SDL_PollEvent(&event))
		{

			switch(event.type)
			{
				case SDL_MOUSEMOTION:
					if (shift && clicking)
					{
						context.curr_camera.pos.x += (float)-1.0f*((float)event.motion.xrel/(float)2.0f);
						context.curr_camera.pos.y += (float)-1.0f*((float)event.motion.yrel/(float)2.0f);
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
						clicking = true;
					break;
				
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
						clicking = false;
					break;
				
				case SDL_MOUSEWHEEL:
					context.curr_camera.zoom += ( event.wheel.y );
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_SPACE)
						pause ^= 1u;
					if (event.key.keysym.sym == SDLK_LSHIFT)
						shift = true;
					break;
				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_LSHIFT)
						shift = false;
					break;

				case SDL_QUIT:
					close = true;
					break;
			}
		}

		if (close) break;

		if (clicking && !shift)
		{
			int mousex,mousey;
			SDL_GetMouseState(&mousex,&mousey);

			int x = context.curr_camera.pos.x+(mousex/context.curr_camera.cell_size.x);
			int y = context.curr_camera.pos.y+(mousey/context.curr_camera.cell_size.y);

			if (x<screen.width && y<screen.height)
				toggle_bit(&screen,x,y);
		}

		if (!pause) conway_turn(&screen);
		
		drawing_routine(screen, &context);
	}
	
	SDL_DestroyWindow(context.curr_window);
	SDL_Quit();

	return 0;
}
