/**
 * Filename: apriori.c
 * Purpose: Defines the functions for the FP Growth algorithm as used
 * Author: Bawe Emmanuel
 */
#include "fptree.h"


fptreenodePtr create_fptree_node(int sup) {
	fptreenodePtr new_node = (struct fptree_node *)malloc(sizeof(struct fptree_node));
	new_node->support = sup;
	new_node->num_nodes++;
	
	return new_node;
}

fpgsubtreePtr create_fpsubtree_node(int name, int support, fpgsubtreePtr prev) {
	fpgsubtreePtr new_node = (struct fpgsubtree_node *)malloc(sizeof(struct fpgsubtree_node));
	new_node->item_name = name;
	new_node->item_count = support;
	new_node->parent = prev;
	
	return new_node;
}

fpgheaderPtr create_fpgheader(int col_num) {
	fpgheaderPtr new_node = (fpgheaderPtr)malloc(sizeof(struct fpgheader));
	new_node->item_name = col_num;

	return new_node;
}

fpgsupsetsPtr create_fpgsupsets( int itemsets[], int size, int sup, fpgsupsetsPtr next) {
	int i = 0;
	fpgsupsetsPtr new_node = (fpgsupsetsPtr)malloc(sizeof(struct fpgsupsets));
	for( i = 0; i < size; i++)
		new_node->item_set[i] = itemsets[i];

	new_node->support = sup;
	new_node->next = next;
	
	return new_node;
}

fpgcolcntPtr create_fpgcolcnt(int col, int sup) {
	fpgcolcntPtr new_node = (fpgcolcntPtr)malloc(sizeof(struct fpgcolcnt));
	new_node->col_num = col;
	new_node->support = sup;
	
	return new_node;
}

/**
 Note: Get size of data array. 
 */
void create_fptree(fptreePtr fptree){
	int i = 0;
	fptree = (fptreePtr)malloc(sizeof(struct fptree));
	fptree->root = create_fptree_node(1);
	fptree->header_table = create_fpgheader(0);
	
	/* create header tables as population progresses */
	for( i = 0; i < num_oflines; i++) {
		if (data[i] != NULL)
			add_to_fptree(fptree->root, 0,data[i], BASKET_MAX_CHARS, 1, fptree->header_table);
	}
}

void add_to_fptree(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header){
	if (place < size) {
		if (!add_to_fptree1(ref, place, itemset, size, sup, header))
			add_to_fptree2(ref,place,itemset,size,sup,header);
	}
}

bool add_to_fptree1(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header) {

	while (ref->child != NULL) {
		if( itemset[place] == ref->child->node->item_name) {
			ref->child->node->item_count += sup;
			num_updates++;
			add_to_fptree(ref->child, place + 1, itemset, size, sup, header);
			
			return true;
		} 
		
		if (itemset[place] < ref->child->node->item_name) {
			return false;
		}
		ref = ref->child;
	}
	
	return false;
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
		if (col_num == header->item_name) {
			tmp = header->next;
			header->next = new_node;
			new_node->next = tmp;
			break;
		}
		header = header->next;
	}
}

void add_rem_itemsets(fptreenodePtr ref, fpgsubtreePtr back, int place, int itemset[], int size, int sup, 
					fpgheaderPtr header) {
	/* Process if more items in itemset */
	if (place < size) {
		fpgsubtreePtr subtree = create_fpsubtree_node(itemset[place],sup, ref->node); /* Create new prefix subtree node */
		fptreenodePtr fpnode =  create_fptree_node(sup); /* create new fptree node incorporating subtree node */
		fpnode->node = subtree;
		add_ref_to_fpghtable(itemset[place], subtree, header); /* Adds link to header table */
		ref->child = fpnode; /* Add into FP tree */
		add_rem_itemsets(fpnode, subtree, place + 1, itemset, size, sup, header); /** Proceed down branch with rest of itemsets */
	}

}

void add_to_fptree2(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header) {
	
	fpgsubtreePtr subtree = create_fpsubtree_node(itemset[place],sup, ref->node); /* Create new prefix subtree node */
	fptreenodePtr fpnode =  create_fptree_node(sup); /* create new fptree node incorporating subtree node */
	fpnode->node = subtree;
	
	/** Note: Still to implement this function */
	add_ref_to_fpghtable(itemset[place], subtree, header); /* Adds link to header table */
	ref->child = fpnode; /* Add into FP tree */
	add_rem_itemsets(fpnode, subtree, place + 1, itemset, size, sup, header); /* Proceed down branch with rest of itemsets */
}

void start_mining(fptreePtr fptree) {
	int itemset[2];
	/* Loop through header table item by item */
	while (fptree->header_table != NULL) {
		/* check for null link */
		if (header->table->next != NULL) {
			start_mining2(fptree, fptree->header_table->next, fptree->header_table->item_name, itemset,2);
		}
	}
}

void start_mining2(fptreePtr fptree, fpgsubtreePtr node, int item, int itemset_sofar[], int size) {
	fpgcolcntPtr count[size];
	fpgheaderPtr lheader;
	fptreenodePtr lroot;
	int support, i;
	int code_sofar[size + 1];
	/** Count support for current item in header table and store in a tree */
	support = gensup_headtable(node);
	code_sofar[0] = item;/** Resizes given array so that its length is increased by one element, new element added to front */
	for (i = 0;i < size; i++)
		code_sofar[i+1] = itemset_sofar[i];
	/** NOTE add_to_tree(code_sofar, size + 1, support);: To be implemented. */
	
	/** Collect ancestor itemsets and store in linked list */
	generate_ancestor(fptree->start_tmp_sets, node);
		
	/** Process ancestor itemsets. */
	if (fptree->start_tmp_sets != NULL) {
		count = count_fpgsingles(fptree); /* Count singles in linked list */
		lheader = create_local_htable(count); /* Create and pop local header table */
		if (lheader != NULL) {
			prune_ancestors(fptree, count); /* Prune ancestor itemsets */
			lroot = gen_local_fptree(fptree, lheader)/* Create new local root for local FP tree */
			start_mining(lroot, lheader, code_sofar, size + 1); /* Mine new FP tree */
		}
		
	}
}

int gensup_headtable(fpgsubtreePtr node) {
	int count = 0;
	while (node != NULL) {
		count += node->item_count;
		num_updates++;
		node = node->next;
	}
	return 0;
}

void generate_ancestor(fpgsupsetsPtr start_tmp_sets, fpgsubtreePtr ref) {
	int sup;
	int *ancestor_code;
	
	while (ref != NULL) {
		sup = ref->item_count;
		ancestor_code = get_ancestor(ref->parent);
		
		/* Add to linked list with current support */
		if (ancestor_code != NULL)
			start_tmp_sets = create_fpgsupsets(ancestor_code, sizeof(ancestor_code)/sizeof(int), sup, start_tmp_sets);
			
		ref = ref->next;
	}

}

int * get_ancestor(fpgsubtreePtr ref) {
	int itemset[ OTH_ITEMSET_ARRAY_MAX];
	int i = 0;
	if (ref == NULL)
		return NULL;
	while (ref != NULL) {
		itemset[i++] = ref->item_name;
		ref = ref->parent;
	}
	size = i;
	return itemset;
}

void prune_ancestors(fptreePtr fptree, fpgcolcntPtr count[]) {
	fpgsubtreePtr ref = fptree->start_tmp_sets;
	
	while (ref != NULL) {
		for ( i = 0; i < sizeof(count)/sizeof(fpgcolcntPtr); i++) {
			if (count[ref->item_set[i]]->support < SUPPORT_THRESHOLD)
				rem_elt(ref->item_set, i);/** Removes in frequent itemset */
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
	
	fpgsupsetsPtr supsets = fptree->start_tmp_sets;
	fpgcolcntPtr count[] =
	count[0] = create_fpgcolcnt(0,0) 
	/* Init array */
	for( index = 1; index < colcnt ; index++)
		count[index] = create_fpcolcnt(0,0);
	while (supsets != NULL) {
		for( index = 0; index < colcnt; index++) {
			place = supsets->item_set[index];
			count[place]->support += supsets->support;
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

fpgheaderPtr local_htable_unordered(fpgcolcntPtr count[]) {
	int counter = 1, i, place;
	/* Loops through array, counting frequent one item sets */
	for (i = 1; i < colcnt; i++) {
		if (count[i]->support >= SUPPORT_THRESHOLD)
			counter++;
	}
	/* Builds new Header Table array containing frequent items */
	if (counter == 1)
		return NULL;
		
	/* Populates header table */	
	fpgheaderPtr lhtable[colcnt];
	place = 1;
	for (i = 1;i < colcnt; i++) {
	    if (count[i]->support >= SUPPORT_THRESHOLD) {
	        lhtable[place] = create_fpgheader(count[i]->col_num);    
	        place++;
	    }
	 }    
	
	return lhtable;
}

void local_htable_ordered(fpgheaderPtr table[], int hsize, fpgcolcntPtr count[], int size) {
	bool order;
	fpgheaderPtr tmp;
	int i, place1, place2;
	
	do {
		i = 1;
		order = true;
		while ( i < hsize -1) {
			place1 = table[i]->item_name;
			place2 = table[i + 1]->item_name;
			if (count[place1]->support > count[place2]->support) { /* swap */
				order = false;
				tmp = table[i];
				table[i] = table[i + 1];
				table[i + 1] = temp;
			}
			i++;
		}
	} while (order == false);				
}

fptreenodePtr gen_local_fptree(fptreePtr fptree, fpgheaderPtr table) {
	fpgsupsetsPtr ref = fptree->start_tmp_sets;
	fptreenodePtr lroot = create_fptree_node(0);
	
	while (ref != NULL) {
		if (ref->item_set != NULL)
			add_to_fptree(lroot, 0, ref->item_set, ref->support, table);
			
		ref = ref->next;
	}
	
	return lroot;
}

fptreenodePtr realloc_fptree(fptreenodePtr old, fptreenodePtr new_node) {
	fptreenodePtr tmp;
	
	if (old == NULL) {
		old = create_fptree_node(new_node->support);
		old->num_nodes += new_node->num_nodes;
		old->node = new_node->node;
		old->fpgheader = new_node->fpgheader;
		old = old->child;
		return old;
	}
	
	while(old != NULL) {
		if ( new_node->node->item_name < old->node->item_name) {
			tmp = old;
			new_node->child = tmp;
			old = new_node;
			return old;
		}
		
		old = old->child;
	}
	old = new_node;
	
	return old;
}

void out_fpsubtree(fpgheaderPtr htable) {
	
	printf("\nPrefix Subtree from Header Table");
	printf("\n HEADER = %d ,", htable->item_name);
	while (htable->next != NULL) {
		printf("\nSub tree: %d, %d, ", htable->next->item_count, htable->next->item_name);
		htable->next = htable->next->next;
	}
}

int out_fpsubtree2(fpgsubtreePtr ref) {
	int counter = 1;
	while (ref != NULL) {
		printf("\n Counter: %d, item_name: %d , item_count: %d",counter,
			ref->item_name, ref->item_count);
		counter++;
		ref = ref->next;
	}
	
	return counter;
}

void out_fptree(fptreePtr ref) {
	fptreenodePtr tmp = ref->root;
	if (ref == NULL)
		return;
		
	while(tmp != NULL) {
		outfpsubtree2(tmp->node);
		tmp = tmp->child;
	}

}

int calc_storage(fptreenodePtr ref, int storage) {
	if (ref == NULL)
		return storage;
		
	while(ref != NULL) {
		storage += 20;
		ref = ref->child;
	}
	return storage;
}

void out_fptree_storage(fptreenodePtr root) {
	int storage = 8;
	storage = calc_storage(root->child, storage);
	
	printf("\n FP Tree storage = %d bytes", storage);
	printf("\n FP tree update = %d", num_updates);
	printf("\n\n");
}

void input_dataset() {

	FILE *trans;
    char item[ITEM_MAX];
    char basket[BASKET_MAX_ITEMS];
    int i,j,item_no, num_lines = 0;
    char *p;

    trans = fopen(TRANS_DB,"r");
    if( trans == NULL) {
		fprintf(stderr,"\n Error Opening Transaction Database file: %s", TRANS_DB);
		exit(EXIT_SUCCESS);
    }


    /* read one line at a time from the file */
    while (fgets(basket, BASKET_MAX_CHARS, trans) != NULL) {
        
        for (i=0;i<BASKET_MAX_ITEMS;i++)
            data[num_lines][i] = -1;

        item_no = 0;

        /*read line items into items array*/
        p = basket;

        /* stops at the end of string character */
        while (*p != '\0') {
            if (*p == '\0')
                break;
            while (isspace(*p)){ /* skip spaces */
                ++p;
                continue;
            }
            if (*p == '\0')
                break;
            j=0;
            while (!isspace(*p)){
                item[j++] = *p;
                ++p;
            }
            item[j] = '\0';//stores null at end of string.

            data[num_lines][item_no] = atoi(item);
            item_no++;
        }
        num_lines++;
    }
    num_oflines = num_lines;
	
}

void order_input_data() {
	int i, j, k;
	
	for (i = 0; i < num_oflines; i++) {
		for (j = 0; j < BASKET_MAX_CHARS; j++) {
			if (data[i][j] != conv[data[i][j]][0]) {
				conv[data[i][j]][0] = data[i][j];
				conv[data[i][j]][1] = 1;
			} else {
				conv[data[i][j]][1] += 1;
			}
		}
	}
	
	order_count_array();
}

/** Sorts array in descending order */
void order_count_array() {
	int attr, qty,i, j;
	for (i = 0; i < MAX_LINES -1;i++)
		j = i + 1;
		while ( conv[j][1] > conv[i][1] && j < MAX_LINES) {
			/** Swap both values */
			int tmp[2];
			tmp[0] = conv[i][0];
			tmp[1] = conv[i][1];
			conv[i][0] = conv[j][0];
			conv[i][1] = conv[j][1];
			conv[j][0] = tmp[0];
			conv[j][1] = tmp[1];
			j++;
		}
}

/* GET NUM OF SUPPORTED ONE ITEM SETS */
int gen_freq_one_itemsets() {
	int i, j, counter;
	char freq_file[20];
	FILE *fp;
	strcpy(freq_file,"../freq_j_");
    sprintf(str, "%d", 1);
    strcat(freq_file, str);
    strcat(freq_file, "_itemsets");
    
	fp = fopen(freq_file,"w");
    if (f_itemsets == NULL) {
		fprintf(stderr,"\n Error opening file: %s", freq_file);
		exit(EXIT_SUCCESS);
    }
    
	for (i = 0; i < NUM_LINES;i++) {
		if ( conv[i][1] < SUPPORT_THRESHOLD)
			return counter;
		else 
			fprintf(fp, "\n %d; %d", conv[i][0], conv[i][1]);
			
		counter++;
	}
	num_oflines = counter;
	return counter;
}

void recast_data_prune_unsupported() {
	int itemset[BASKET_MAX_CHARS][2];
	int attr, i, j, k = 0;
	
	for (i = 0; i < num_oflines; i++) {
		k = 0;
		if (data[i] != NULL) {
			for (j = 0; j < BASKET_MAX_CHARS; j++) {
				attr = data[i][j];
				
				/** check support */
				if (conv[attr][1] >= SUPPORT_THRESHOLD) {
					itemset[k][0] = conv[attr][0];
					itemset[k++][1] = conv[attr][1]
				}
				data[i][j] = -1;/* reset array. */
			}
			
			for (j = 0; j < k; j++)
				data[i][j] = item[j][0];
		}
	}
}

