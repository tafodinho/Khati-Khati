/**
 * Filename: apriori.c
 * Purpose: Defines the functions for the FP Growth algorithm as used
 * Author: Bawe Emmanuel
 */
#include "fptree.h"


fptreePtr create_fptree_node(int sup) {
	fptreePtr new_node = (struct fptreePtr)malloc ((sizeof(struct fptreePtr));
	new_node->support = sup;
	new_node->num_nodes++;
	
	return new_node;
}

fpgsubtreePtr create_fpsubtree_node(int name, int support, fpgsubtreePtr prev) {
	fpgsubtreePtr new_node = (struct fpgsubtreePtr)malloc(sizeof(struct fpgsubtreePtr));
	new_node->item_name = name;
	new_node->item_count = support;
	new_name->parent = prev;
	
	return new_node;
}

fpgheaderPtr create_fpgheader(int col_num) {
	fpgheaderPtr new_node = (struct fpgheader)malloc(sizeof(struct fpgheader));
	new_node->item_name = col_num;
	new_node = new->next;

	return new_node;
}

fpgsupsetsPtr create_fpgsubsets( int itemsets[], int size, int sup, fpgsubsetsPtr next) {
	int i = 0;
	fpgsupsetsPtr new_node = (struct fpsupsets)malloc(sizeof(struct fpgsupsets));
	new_node->item_set = (int)malloc(sizeof(int) * size);
	for( i = 0; i < size; i++)
		new_node->item_set[i] = itemsets[i];
	new_node->support = sup;
	new_node->next = next;
	
	return new_node;
}

fpgcolcntPtr create_fpgcolcnt(int col, int sup) {
	fpgcolcntPtr new_node = (struct fpgcolcnt)malloc(sizeof(struct fpgcolcnt));
	new_node->col_num = col;
	new_node->support = sup;
	
	return new_node;
}

/**
 Note: Get size of data array. 
 */
void create_fptree(fptreePtr fptree, int size){
	int i = 0;
	fptree = (struct fptree)malloc(sizeof(struct fptree));
	fptree->header_table = create_fpgheader(0);
	
	/* create header tables as population progresses */
	for( i = 0; i < size; i++) {
		if (data[i] != NULL)
			add_to_fptree1(fptree->root, 0,data[i], 1, fptree->header_table);
	}
}
	
