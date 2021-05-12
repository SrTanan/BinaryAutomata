#include "input_handling.h"

void *run(void* input_bus) {
	data_bus* bus = (data_bus*)input_bus;

	for (;;)
	{
		int c = wgetch(stdscr);

		if ( c == ERR) continue;
		
		pthread_mutex_lock(&bus->lock);
		if ( c == KEY_MOUSE )
		{
			MEVENT event;
				
			if ( getmouse(&event) == OK )
			{
				
				bus->curr_click = event;
				bus->status |= 1u<<2;
			}
		}
		else
		{
	
			if (keyname(c) == "KEY_F(10)")
			{
				bus->status |= 1u<<4;
				break;
			}

			bus->curr_key = c;
			bus->status |= 1u<<3;
		}

		bus->status &= ~( 1u<<7 );
		pthread_mutex_unlock(&bus->lock);
	}

}
