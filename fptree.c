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
	fptree->root = create_fptree_node(1);
	fptree->header_table = create_fpgheader(0);
	
	/* create header tables as population progresses */
	for( i = 0; i < size; i++) {
		if (data[i] != NULL)
			add_to_fptree(fptree->root, 0,data[i], size, 1, fptree->header_table);
	}
}

void add_to_fptree(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header){
	if (place < size) {
		if (!add_to_fptree1(ref, place, itemset, size, sup, header))
			add_to_fptree2(ref,place,itemset,size,sup,header);
	}
}

bool add_to_fptree1(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header) {

	while (ref.child != NULL) {
		if( itemset[place] == ref->child->node.item_name) {
			ref->child->node.item_count += sup;
			num_updates++;
			add_to_fptree(ref.child, place + 1, itemset, size, sup, header);
			
			return true;
		} 
		
		if (itemset[place] < ref->child->node.item_name) {
			return false;
		}
		ref = ref->child;
	}
	
	return false;
}

void add_to_fptree2(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header) {
	
	fpgsubtreePtr subtree = create_fpsubtree_node(itemset[place],sup, ref.node); /* Create new prefix subtree node */
	fptreenodePtr fpnode =  create_fptree_node(sup); /* create new fptree node incorporating subtree node */
	fpnode.node = subtree;
	
	/** Note: Still to implement this function */
	add_ref_to_fpghtable(itemset[place], subtree, fpnode); /* Adds link to header table */
	ref.child = fpnode; /* Add into FP tree */
	add_rem_itemsets(fpnode, subtree, place, itemset, size, sup, header); /* Proceed down branch with rest of itemsets */
}


