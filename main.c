#include "byteops.h"
#include "input_handling.h"
#include <string.h>

void print_grid(grid* input_grid)
{
	
	move(0,0);

	for (int y=0; y<input_grid->height; y++)
	{
		move(y,0);
		for (int x=0; x<input_grid->width; x++)
		{
			addch( (input_grid->buffer[((input_grid->width/8)*y)+(x/8)] & (1u << (x-((x/8)*8)))) ? '#' : '.' );
		}

		addch('|');
	}
	
	move(input_grid->height,0);
	for (int i=0; i<input_grid->width; i++)
	{
		addch('_');
	}

	move(input_grid->height+1,0);
	refresh();
}

void conway_turn(grid* input)
{	
	grid next_turn;
	next_turn.width = input->width;
	next_turn.height = input->height;

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

	for (int i=0; i<sizeof(next_turn.buffer); i++)
	{
		input->buffer[i] = next_turn.buffer[i];
	}	
}

int main(void)
{
	grid screen;
	screen.width = 80;
	screen.height = 24;
	
	clear_buffer(&screen);		
	data_bus bus;
	bus.status = 0;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

	printf("\033[?1003h\n");
	pthread_t input_thread;
	pthread_create(&input_thread, NULL, run, &bus);
	
	MEVENT click;
	int key;
	
	char output[128];
	size_t output_size = sizeof(output);

	bool pause = false;

	for (;;)
	{
		
		pthread_mutex_lock(&bus.lock);
		
		if ( (bus.status & ( 1u<<4 ) ) > 0 ) break;
		
		if ( ( bus.status & ( 1u<<2 ) ) != 0)
		{
			click = bus.curr_click;
			
			if ( (click.x<screen.width) && (click.y<screen.height) )
				toggle_bit(&screen, click.x, click.y);
			else if ( (click.x == 100) && (click.y == 12) )
				pause ^= 1u;

			bus.status &= ~( 1u<<2 );	
		}

		if ( ( bus.status & ( 1u<<3 ) ) != 0)
		{
			key = bus.curr_key;
			
			if ( keyname(key) == " " )
			{
				pause ^= 1u;
			}

			bus.status &= ~( 1u<<3 );
		}
		
		bus.status &= ~1u;
		pthread_mutex_unlock(&bus.lock);
			
		if (!pause) conway_turn(&screen);
		
		print_grid(&screen);	
		move(12,100);
		printw("P");

		refresh();
	}
	
	printf("\033[?1003l\n");
	endwin();
}
