#include "hash.h"
#include "kmem_cache.h"

void add_hash_node(unsigned long buf, unsigned long bufctl) {
	struct hash *thash = (struct hash *)kmem_cache_alloc(&kcache[hash_cacheID], sizeof(struct hash));
	int id=buf%DEF_HASH_NUM;
	thash->buf=buf;
	thash->bufctl=bufctl;
	if(hash_node[id]==NULL) {
		hash_node[id]=thash;
		thash->next=NULL;
	}
	else {
		thash->next=hash_node[id];
		hash_node[id]=thash;
	}
}

unsigned long find_hash_node(unsigned long buf) {
	struct hash *thash;
	int id=buf%DEF_HASH_NUM;
	if(hash_node[id]==NULL) {
		printf("ERROR: FIND_HASH_NODE!!\n");
		exit(0);
	}
	thash=hash_node[id];
	while(thash!=NULL) {
		if(thash->buf==buf)
			break;
		thash=thash->next;
	}
	return thash->bufctl;
}
