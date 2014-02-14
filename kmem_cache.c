#include "kmem_cache.h"
#include "slab.h"
#include "malloc.h"
#include "hash.h"

int SIZE[DEF_CACHE_NUM]={8, 10, 12, 16, 20, 25, 32, 40, 50, 64, 80, 100, 128, 160, 200, 256, 
			320, 400, 512, 640, 800, 1024, 1280, 1600, 2048, 2560, 3200, 4096, 
			5000, 6250, 8192, 9216};

int cache_id_estimate(int size, int align) {	//estimate which cache will be used 
/*
 * If object size is smaller than or equal to 1/8 page, there would be a slab structure at each end of page
 * with other obejcts.
 * If object size is bigger than 1/8 page, slab structure would be placed on other slab according to its size.
*/
	int i;
	for(i=0; i<DEF_CACHE_NUM; i++) {
		if(size<SIZE[i])
			return i;
	}
	return -1;
}

/*
 *parameters name, constructor and destructor will not be used in kmem_cache_node_creater. Just pass in NULL
*/

void kmem_cache_create(char *name, int size, int align, int id, void(*constructor)(void *, int), void(*destructor)(void *, int)) {
	kcache[id].name = name;
	kcache[id].align = align;
	kcache[id].id = id;
	kcache[id].constructor = constructor;
	kcache[id].destructor = destructor;
	kcache[id].slab_free=NULL;
	kcache[id].slab_full=NULL;
	kcache[id].slab_cacheID=-1;
}

void calObjectNum() {
	int i;
	for(i=0; i<DEF_CACHE_NUM; i++) {
		if(i<LARGE_OBJECT_ID) {
			OBJECT_NUM[i]=( DEF_PAGE_SIZE-sizeof(struct slab) )/SIZE[i];
		}
		else if(SIZE[i]<=4096){
			OBJECT_NUM[i]=DEF_PAGE_SIZE/SIZE[i];
		}
		else if(SIZE[i]<=8192) {
			OBJECT_NUM[i]=DEF_PAGE_SIZE*2/SIZE[i];
		}
		else OBJECT_NUM[i]=DEF_PAGE_SIZE*3/SIZE[i];
	}
}

void startup () {	//routine for seting up cache for computer
	int i;
	calObjectNum();
	hash_cacheID = cache_id_estimate(sizeof(struct hash), 0);
	for(i=0; i<DEF_CACHE_NUM;i++) {
		kmem_cache_create(NULL, SIZE[i], 0, i, NULL, NULL);
	}
	printf("Caches have been set up!!\n");
}

void slab_remove(struct slab **list, struct slab *tslab) {		//remove tslab from list
	if( (*list) == NULL ) {
		printf("ERROR: slab_remove() !!!\n");
		exit(0);
	}
	else if ( (*list)->pre == (*list) ){
		*list=NULL;
	}
	else {
		(tslab->pre)->next=tslab->next;
		(tslab->next)->pre=tslab->pre;
		if( (*list)==tslab )
			*list=tslab->next;
	}
}

void slab_add(struct slab **list, struct slab *tslab) {	//add tslab to list
	if( (*list)  == NULL ) {	//if *list is null, *list is tslab
		*list=tslab;
		tslab->next=tslab;
		tslab->pre=tslab;
	}
	else{	//tslab->pre and tslab->next must change the value first, (*list)->next must do it at the last
		tslab->pre=(*list);
		tslab->next=(*list)->next;
		((*list)->next)->pre=tslab;
		(*list)->next=tslab;
	}	

}

void *kmem_cache_alloc(struct kmem_cache *cache, int flag) {
	struct slab **free=&cache->slab_free;
	struct slab **full=&cache->slab_full;
	struct slab *tslab;
	void *address;
	int i, offset;
	int id=cache->id;
	int object_size=SIZE[id];
	unsigned long *tt;
	struct kmem_bufctl *bufctl;
	int size;
	unsigned long temp;
	if(id<LARGE_OBJECT_ID) {		//if it is small object
		//printf("Allocte SMALL_OBJECT!!\n");
		if( (*free) != NULL ) {		//freelist has available object
			tslab=*free;
			address=tslab->freelist;	//get object address
			//rebuild freelist
			if(tslab->free_count==1) {
				tslab->freelist=NULL;
				slab_remove(free, tslab);
				slab_add(full, tslab);
			}
			else {
				tt=tslab->freelist;
				tslab->freelist= (void *)(*tt);
			}
			tslab->free_count--;
		}
		else {	//freelist has no available object
			address=malloc_align(DEF_PAGE_SIZE);	//get object address
			temp = (unsigned long)address;
			tslab = (struct slab *)(temp+DEF_PAGE_SIZE-sizeof(struct slab));//according object address, 
										//caculate address of slab
			tslab->free_count = OBJECT_NUM[id]-1;
			//rebuild freelist
			if(OBJECT_NUM[id]>1) {
				slab_add(free, tslab);
				tslab->freelist = (void *)(temp+SIZE[id]);
			}
			else {
				tslab->freelist=NULL;
				slab_add(full, tslab);
			}
			//use free object as bufctl, init bufctl
			for(i=1; i<OBJECT_NUM[id]-1;i++) {
				tt=(unsigned long *)(i*object_size+temp);
				*tt= (i+1)*object_size+temp;
			}
		}
	}
	else {		//if it is large obejct
		//printf("Allocate LARGE_OBJECT!!!\n");
		if((*free)!=NULL) {	//there is an avialable object
			tslab=*free;
			bufctl=tslab->freelist;
			tslab->free_count--;
			address=bufctl->object;//get object address
			//rebuild slab's freelist
			if(tslab->free_count==0) {//if no object is avialable
				tslab->freelist=NULL;
				slab_remove(free, tslab);
				slab_add(full, tslab);
			}
			else	//if else
				tslab->freelist=bufctl->next;
		}
		else {	//if there is no avialable object in cache
			if(SIZE[id]<=4096)
				size=4096;
			else if(SIZE[id]<=8192)
				size=8192;
			else size=12288;
			address = malloc_align(size);//get object address
			temp = (unsigned long)address;
			cache->slab_cacheID=cache_id_estimate(sizeof(struct slab), 0);	//recording cacheID which allocates mrmory for slab
			tslab = (struct slab *)kmem_cache_alloc(&kcache[cache->slab_cacheID], KM_SLEEP);//allocating memory for slab
			tslab->kmem_bufctl_cacheID=cache_id_estimate( sizeof(struct kmem_bufctl)*OBJECT_NUM[id], 0);//recording cacheID which allocates
															//memory for bufctl
			bufctl = (struct kmem_bufctl *)kmem_cache_alloc(&kcache[tslab->kmem_bufctl_cacheID], KM_SLEEP);//allocate memory for bufctl
			for(i=0; i<OBJECT_NUM[id]; i++) {		//init bufctl
				bufctl[i].slab_pointer=tslab;
				bufctl[i].object=(void *)(temp+SIZE[id]*i);
				if( i==(OBJECT_NUM[id]-1) )
					bufctl[i].next=NULL;
				else bufctl[i].next=&bufctl[i+1];
				add_hash_node(temp+SIZE[id]*i, (unsigned long)&bufctl[i]);
			}
			tslab->free_count=OBJECT_NUM[id]-1;
			//rebuild slab's freelist
			if(OBJECT_NUM[id]==1) {	//if no object is avialable
				slab_add(full, tslab);
				tslab->freelist=NULL;
			}
			else {//if else
				slab_add(free, tslab);
				tslab->freelist=&bufctl[1];
			}
		}
	}
	return address;
}

void kmem_cache_free(struct kmem_cache *cache, void *address) {
	int id=cache->id;
	int slab_cache_id=cache->slab_cacheID;
	struct slab *tslab;
	struct slab **free=&cache->slab_free;
	struct slab **full=&cache->slab_full;
	struct kmem_bufctl *tbufctl;
	unsigned long *tt;
	unsigned long temp;
	if(id<LARGE_OBJECT_ID) {
		//printf("Free SMALL_OBEJCT!!!\n");
		temp=(unsigned long)address;
		//according the object address, find the page address, then the slab address
		temp=temp/DEF_PAGE_SIZE*DEF_PAGE_SIZE;
		tslab = (struct slab *)(temp+DEF_PAGE_SIZE-sizeof(struct slab));
		if(tslab->free_count==0) {	//ifcurrently this slab is in cache's fulllist
						//delete this from cache's fulllist, add to cache's freelist
			slab_remove(full, tslab);
			slab_add(free, tslab);
		}
		tslab->free_count++;
		tt=(unsigned long *)address;
		*tt=(unsigned long)tslab->freelist;	//add this object to slab's freelist
		tslab->freelist=tt;
	}
	else {
		//printf("Free LARGE_OBJECT!!!\n");
		//using the object address with hash table, find the bufctl address, then find
		//slab address
		temp=(unsigned long)address;
		temp=find_hash_node(temp);
		tbufctl = (struct kmem_bufctl *)temp;
		tslab = tbufctl->slab_pointer;
		if(tslab->free_count==0) {	//if currently, this slab is in cache's fulllist,
						//delete this from cache's fulllist, add to cache's freelist
			slab_remove(full, tslab);
			slab_add(free, tslab);
		}
		tslab->free_count++;
		tbufctl->next = tslab->freelist;	//add this object to slab's freelist
		tslab->freelist = tbufctl;
	}
}

void kmem_cache_size(int id) {
	
}
