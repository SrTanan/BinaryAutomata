#include "byteops.h"
#include "window_handling.h"
#include <pthread.h>

typedef struct{
	grid* grid;
	grid* output;
	int begin;
	int end;
	pthread_mutex_t* lock;
}thread_args;

void* conway_turn(void* input_args) //Note: We don't need to use mutexes for this because we never read the same data between different threads.
{	
	
	thread_args* args = (thread_args*)input_args;
	grid *next_turn = args->output;
	clear_buffer(next_turn);

	for (int x=0; x<args->output->width; x++)
	{
		for (int y=0; y< args->output->height; y++)
		{
			unsigned char neighbour_count = count_bits( get_neighbours(args->grid, x,y+((args->begin*8)/args->grid->width)) );
			bool alive = get_bit(args->grid, x,y+((args->begin*8)/args->grid->width));

			if ( alive )
			{
				set_bit(args->output,x,y,true);

				
				if ( neighbour_count < 2 ) set_bit(args->output,x,y,false);
				if ( neighbour_count > 3 ) set_bit(args->output,x,y,false);
				
				
			}
			else if (neighbour_count ==  3) set_bit(args->output, x, y, true);
		}
	}
}

int main(void)
{

	bool pause = false;
	bool close = false;
	bool clicking =  false;
	bool shift = false;

	grid screen;
	screen.width = 800;
	screen.height = 600;
	screen.size = (screen.width*screen.height)/8;
	screen.buffer = malloc(screen.size);
	clear_buffer(&screen);

	const int thread_amount = 8;
	pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;
	thread_args threads_args[thread_amount];
	pthread_t threads[thread_amount];
	grid test_array[thread_amount];

	for (int i=0; i<thread_amount; i++)
	{
		
		threads_args[i].grid = &screen;
		threads_args[i].begin = (screen.size/thread_amount)*i;
		threads_args[i].end = (screen.size/thread_amount)*(i+1);
		threads_args[i].lock = &data_lock;
		
		threads_args[i].output = &test_array[i];
		threads_args[i].output->width = screen.width;
		threads_args[i].output->height = (screen.height/thread_amount);
		threads_args[i].output->size = (screen.width*(screen.height/thread_amount))/8;
		threads_args[i].output->buffer =  malloc(threads_args[i].output->size);
	}

	drawing_context context = create_context("BinaryAutomata");
	SDL_Event event;
	
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
		
		if (!pause)
		{
			for (int i=0; i<thread_amount; i++)
			{				
				pthread_create(&threads[i],NULL,conway_turn,&threads_args[i]);
			}

			for (int i=0; i<thread_amount; i++)
			{
				pthread_join(threads[i], NULL);
			}

			for (int i=0; i<thread_amount; i++)
			{
				for (int j=0; j<threads_args[i].output->size; j++)
				{
					screen.buffer[j+threads_args[i].begin] = threads_args[i].output->buffer[j];
				}
			}
		}

		drawing_routine(screen, &context);
	}
	
	SDL_DestroyWindow(context.curr_window);
	SDL_Quit();

	return 0;
}
