#ifndef __GWU_KMEM_CACHE__
#define __GWU_KMEM_CACHE__

#include "hash.h"
#include "slab.h"
#include <stdio.h>

#define DEF_CACHE_NUM 32	//default:there is 32 cache in system, 
				//the size of the slab of cache[i] is store in SIZE[32] arrary
				//from 8 bytes to 9kb, in roughly 12.5% increasement


#define DEF_SLAB_MIN_SIZE DEF_PAGE_SIZE	//default: the slab in every cache has at least allocate 1 page memory
					//to simulate actual memory allocation

#define LARGE_OBJECT_ID	19	//any object whose size is smaller than SIZE[LARGE_OBJECT_ID], it belongs to 
				//small slab, otherwise, it belongs to large slab

#define DEF_COLOR_MAX 64		//used for color, it is the biggest color, in this program, it will not be used

#define KM_SLEEP 0
#define KM_NOSLEEP 1
//==================================================================================================================
/*
 * When the computer(actually, this program) start up, I first build up 20 caches for other program use,
 * if none of the caches cannot fit the need of a program, computer will report a error. The object size * of the 20 caches are 16 bytes, 32 bytes, 64 bytes ...
 * Every slab in every cache must allocate integer number times page memory(4096 bytes).
 * Iniatially, there will be no slab in each cache
*/

struct kmem_cache {

	char *name;	//cache name
	int align;
	int id;
	
	void(*constructor)(void *, int);
	void(*destructor)(void *, int);

	struct slab *slab_full;	//slabs in cache will be divide into two types:  free, full.
	struct slab *slab_free;		//Unlike paper, here I change the way to build freelist.	
	
	int slab_cacheID;	//the id of the cache that stores slab descripter
};

//==================================================================================================================================

struct kmem_cache kcache[DEF_CACHE_NUM];

int hash_cacheID;	//cache ID which is used to control the memory built for hash

int OBJECT_NUM[DEF_CACHE_NUM];//object num in every cahce
//====================================================================================================================================

void kmem_cache_destory(struct kmem_cache *cache);

void *kmem_cache_alloc(struct kmem_cache *cache, int flag);
void kmem_cache_free(struct kmem_cache *cache, void *address);

void kmem_cache_grow();
void kmem_cache_reap();

void *kmem_alloc(int size);
void kmem_free(void *);

int cache_id_estimate(int size, int align);

void kmem_cache_create(char *name, int size, int align, int id, void(*constructor)(void *, int), void(*destructor)(void *, int));



#endif	//__GWU_CACHE__
