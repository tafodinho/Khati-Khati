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

	return new_node;
}
