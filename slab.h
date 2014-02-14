#ifndef __GWU_SLAB__
#define __GWU_SLAB__

struct kmem_bufctl {
	struct kmem_bufctl *next;
	void *object;
	struct slab *slab_pointer;
};

struct slab{
	int kmem_bufctl_cacheID;
	int free_count;
	void *freelist;
	struct slab *next;
	struct slab *pre;
};

#endif //__GWU_SLAB__
