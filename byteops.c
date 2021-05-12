#include "byteops.h"

void set_bit(grid* input, int x, int y, bool val)
{
	input->buffer[((input->width/8)*y)+(x/8)] ^= (-(unsigned char)val ^ input->buffer[((input->width/8)*y)+(x/8)]) & (1u << x-((x/8)*8));
}

void toggle_bit(grid* input, int x, int y)
{
	input->buffer[((input->width/8)*y)+(x/8)] ^= (1u<<x-((x/8)*8));
}

bool get_bit(grid* input, int x, int y)
{
	return ( input->buffer[((input->width/8)*y)+(x/8)] & (1u << (x-((x/8)*8))) ) ? true : false;
}

void clear_buffer(grid* input_buffer)
{
	for (int i=0; i<sizeof(input_buffer->buffer); i++)
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

unsigned char get_neighbours(grid* map, int x, int y)
{
	unsigned char output_byte = 0;
	unsigned char index = 0;

	for (int i=x-1; i<x+2; i++)
	{
		if (i<0) continue;
		if (i>map->width) break;

		for (int j=y-1; j<y+2; j++)
		{
			if (j<0) continue;
			if (j>map->height) break;
			if (i==x && j==y) continue;
			
			output_byte |= ( get_bit(map,i,j) << index);
			index++;
		}
	}
	
	return output_byte;
}
