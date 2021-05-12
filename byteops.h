#include "main.h"

void set_bit(grid* input, int x, int y, bool val);
void toggle_bit(grid* input, int x, int y);
bool get_bit(grid* input, int x, int y);
void clear_buffer(grid* input);
unsigned char get_neighbours(grid* map, int x, int y);
unsigned char count_bits(unsigned char byte);
