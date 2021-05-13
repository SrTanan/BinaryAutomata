#include <stdlib.h>
typedef enum
{
	true=1,false=0
}bool;

typedef struct
{
	unsigned char* buffer;
	size_t size;
	int width;
	int height;
}grid;
