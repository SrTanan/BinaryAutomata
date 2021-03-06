#include "byteops.h"

void set_bit(grid* input, int x, int y, bool val)
{
	int x_byte = x/8;
	int curr_byte = ((input->width/8)*y)+x_byte;
	input->buffer[curr_byte] ^= (-(unsigned char)val ^ input->buffer[curr_byte]) & (1u << x-(x_byte*8));
}

void toggle_bit(grid* input, int x, int y)
{
	int x_byte = x/8;
	int curr_byte = ((input->width/8)*y)+x_byte;
	input->buffer[curr_byte] ^= (1u<<x-(x_byte*8));
}

bool get_bit(grid* input, int x, int y)
{
	int x_byte = x/8;
	int curr_byte = ((input->width/8)*y)+x_byte;	

	return ( input->buffer[curr_byte] & (1u << (x-(x_byte*8))) ) ? true : false;
}

void clear_buffer(grid* input_buffer)
{
	for (int i=0; i<input_buffer->size; i++)
	{
		input_buffer->buffer[i] = 0;
	}
}

unsigned char count_bits(unsigned char byte)
{
	static const unsigned char LOOKUP_TABLE[16] =
	{
		0,1,1,2,1,2,2,3,
		1,2,2,3,2,3,3,4
	};
	
	return LOOKUP_TABLE[byte & 0x0F] + LOOKUP_TABLE[byte >> 4];
}

unsigned char get_neighbours(grid* map, int x, int y) // This actually restrains x and y, but it's a good compromise for performance
{
	unsigned char output_byte = 0;

	if (x+1<map->width && x-1 > 0)
	{
		if (y+1<map->height && y-1 > 0)
		{

			output_byte |= get_bit(map,x-1,y-1);
			output_byte |= get_bit(map,x-1,y) << 1;
			output_byte |= get_bit(map,x-1,y+1) << 2;
			output_byte |= get_bit(map,x,y-1) << 3;
			output_byte |= get_bit(map,x+1,y-1) << 4;
			output_byte |= get_bit(map,x+1,y) << 5;
			output_byte |= get_bit(map,x,y+1) << 6;
			output_byte |= get_bit(map,x+1,y+1) << 7;
			return output_byte;
		}
	}
	
	return output_byte;
}
