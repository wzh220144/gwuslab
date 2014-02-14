#ifndef __GWU_HASH__
#define __GWU_HASH__
#define DEF_HASH_NUM 37	//address%DEF_HASH_NODE is the id of hash node

struct hash {
	unsigned long buf;
	unsigned long bufctl;
	struct hash *next;
};

struct hash *hash_node[DEF_HASH_NUM];

void add_hash_node(unsigned long buf, unsigned long bufctl);

void del_hash_node(unsigned long buf, unsigned long bufctl);

unsigned long find_hash_node(unsigned long buf);

#endif //__GWU_HASH__
