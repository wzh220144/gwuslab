#include "malloc.h"

void *malloc_align(int size) {
	void *address=malloc(size+DEF_PAGE_SIZE);
	if(address==NULL) {
		printf("Cannot %d bytes memory!!!\n", size);
		exit(0);
	}

	unsigned long shift = DEF_PAGE_SIZE - ((unsigned long)address + sizeof(unsigned long))&(DEF_PAGE_SIZE-1) ;

	unsigned long temp = (unsigned long) address ;
	unsigned long *a=(unsigned long *)( temp + shift );
	a[0]= temp;
	return &a[1];
}

void free_align(void *address) {
	unsigned long *a=(unsigned long *)( (unsigned long)address-sizeof(unsigned long));
	free( (void *)a[0] );
}
