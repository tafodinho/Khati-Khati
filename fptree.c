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
	add_ref_to_fpghtable(itemset[place], subtree, header); /* Adds link to header table */
	ref.child = fpnode; /* Add into FP tree */
	add_rem_itemsets(fpnode, subtree, place + 1, itemset, size, sup, header); /* Proceed down branch with rest of itemsets */
}

void add_rem_itemsets(fptreenodePtr ref, fpgsubtreePtr back, int place, int itemset[], int size, int sup, 
					fpgheaderPtr header) {
	/* Process if more items in itemset */
	if (place < size) {
		fpgsubtreePtr subtree = create_fpsubtree_node(itemset[place],sup, ref.node); /* Create new prefix subtree node */
		fptreenodePtr fpnode =  create_fptree_node(sup); /* create new fptree node incorporating subtree node */
		fpnode.node = subtree;
		add_ref_to_fpghtable(itemset[place], subtree, header); /* Adds link to header table */
		ref.child = fpnode; /* Add into FP tree */
		add_rem_itemsets(fpnode, subtree, place + 1, itemset, size, sup, header); /** Proceed down branch with rest of itemsets */

} 

/** 
 *  add_ref_to_fpghtable(): Adds reference to new FP-tree node to header table moving old reference 
 *					   		so that it becomes a link from the new FP-tree node.
 *  @param col_num the given attribute.
 *  @param new_node the newly created FP-tree node.
 *  @param header the reference to the header table(array). 
 */
void add_ref_to_fpghtable(int col_num, fpgsubtreePtr new_node, fpgheaderPtr header) {
	fpgsubtreePtr tmp;
	
	while (header != NULL) {
		if (col_num == header.item_name) {
			tmp = header.next;
			header.next = new_node;
			new_node.next = tmp;
			break;
		}
		header = header->next;
	}
}

void start_mining(fptreePtr fptree, fpgheaderPtr table, int itemset_sofar[], int size) {
	
	/* Loop through header table item by item */
	while (table != NULL) {
		/* check for null link */
		if (table.next != NULL) {
			start_mining2(fptree, table.next, table.item_name, itemset_sofar, size);
		}
	}

}

void start_mining2(fptreePtr fptree, fpgsubtreePtr node, int item, int itemset_sofar[], int size) {
	fpgcolcntPtr count;
	fpgheaderPtr lheader;
	fptreenodePtr lroot;
	int support, i;
	int code_sofar[size + 1];
	/** Count support for current item in header table and store in a tree */
	support = gensup_headtable(node);
	code_sofar[0] = item;/** Resizes given array so that its length is increased by one element, new element added to front */
	for (i = 0;i < size; i++)
		code_sofar[i+1] = itemset_sofar[i];
	add_to_tree(code_sofar, size + 1, support);/** NOTE: To be implemented. */
	
	/** Collect ancestor itemsets and store in linked list */
	generate_ancestor(fptree.start_tmp_sets, node);
		
	/** Process ancestor itemsets. */
	if (fptree.start_tmp_sets != NULL) {
		count = count_fpgsingles(); /* Count singles in linked list */
		lheader = create_local_htable(count); /* Create and pop local header table */
		if (lheader != NULL) {
			prune_ancestors(fptree, count); /* Prune ancestor itemsets */
			lroot = gen_local_fptree(lheader)/* Create new local root for local FP tree */
			start_mining(lroot, lheader, code_sofar, size + 1); /* Mine new FP tree */
		}
		
	}
}

int gensup_headtable(fpgsubtreePtr node) {
	int count = 0;
	while (node != NULL) {
		count += node.item_count;
		num_updates++;
		node = node->next;
	}
	return 0;
}

void generate_ancestor(fpgsupsetsPtr start_tmp_sets, fpgsubtreePtr ref) {
	int sup;
	int *ancestor_code;
	
	while (ref != NULL) {
		sup = ref.item_count;
		ancestor_code = get_ancestor(ref.parent);
		
		/* Add to linked list with current support */
		if (ancestor_code != NULL)
			start_tmp_sets = create_fpgsubsets(ancestor_code, sizeof(ancestor_code)/sizeof(int), sup, start_tmp_sets);
			
		ref = ref->next;
	}

}

int * get_ancestor(fpgsubtreePtr ref) {
	int itemset[ OTH_ITEMSET_ARRAY_MAX];
	int i = 0;
	if (ref == NULL)
		return NULL;
	while (ref != NULL) {
		itemset[i++] = ref.item_name;
		ref = ref->parent;
	}
	size = i;
	return itemset;
}

void prune_ancestors(fptreePtr fptree, fpgcolcntPtr count[]) {
	fpgsubtreePtr ref = fptree.start_tmp_sets;
	
	while (ref != NULL) {
		for ( i = 0; i < sizeof(count)/sizeof(fpgcolcntPtr); i++) {
			if (count[ref.item_set[i]].support < SUPPORT_THRESHOLD)
				rem_elt(ref.item_set, i);/** Removes in frequent itemset */
		}
		
		ref = ref->next;
	}	
}

int rem_elt(int old_itemset[], int n) {
	if ( sizeof(old_itemset)/sizeof(int) <= n)
		return old_itemset;	
	else { 
		for ( i = n; i < [sizeof(old_itemset)/sizeof(int); i++)
			old_itemset[i] = old_itemset[i + 1]
	}
	
	return 0;	
}

fpgcolcntPtr count_fpgsingles(fptreePtr fptree) {
	int index, place = 0;
	
	fpgsupsetsPtr supsets = fptree.start_tmp_sets;
	fpgcolcntPtr count[] =
	count[0] = create_fpgcolcnt(0,0) 
	/* Init array */
	for( index = 1; index < colcnt ; index++)
		count[index] = create_fpcolcnt(0,0);
	while (supsets != NULL) {
		for( index = 0; index < colcnt; index++) {
			place = supsets.item_set[index];
			count[place].support += supsets.support;
			num_updates++;
		}
		supsets = supsets->next;
	}
	colcnt = i;
	return count;
}


fpgheaderPtr create_local_htable(fpgcolcntPtr count[]) {
	int index;
	fpgheaderPtr lhtable;
	lhtable = local_htable_unordered(count, colcnt);
	
	return lhtable;
}

		
