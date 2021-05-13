#include "byteops.h"

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
	
	bool pause = false;

	for (;;)
	{
		conway_turn(&screen);	
	}
}
