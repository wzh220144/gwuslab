#include <stdlib.h>
#include <stdio.h>

#define DEF_PAGE_SIZE	4096	//default: the page size is 4096 bytes

void *malloc_align(int size);
void free_align(void *address);
