/**
 * Filename: apriori.c
 * Purpose: Defines the functions for the FP Growth algorithm as used
 * Author: Bawe Emmanuel
 */
#include "fptree.h"


fptreenodePtr
create_fptree_node(int sup)
{
	fptreenodePtr new_node =
		(struct fptree_node *) malloc(sizeof(struct fptree_node));
	new_node->support = sup;
	new_node->num_nodes++;

	return new_node;
}

fpgsubtreePtr
create_fpsubtree_node(int name, int support, fpgsubtreePtr prev)
{
	fpgsubtreePtr new_node =
		(struct fpgsubtree_node *)
		malloc(sizeof(struct fpgsubtree_node));
	new_node->item_name = name;
	new_node->item_count = support;
	new_node->parent = prev;

	return new_node;
}

fpgheaderPtr
create_fpgheader(int col_num)
{
	fpgheaderPtr new_node =
		(fpgheaderPtr) malloc(sizeof(struct fpgheader));
	new_node->item_name = col_num;

	return new_node;
}

fpgsupsetsPtr
create_fpgsupsets(int itemsets[], int size, int sup, fpgsupsetsPtr next)
{
	int i = 0;
	fpgsupsetsPtr new_node =
		(fpgsupsetsPtr) malloc(sizeof(struct fpgsupsets));
	for (i = 0; i < size; i++)
		new_node->item_set[i] = itemsets[i];

	new_node->support = sup;
	new_node->next = next;

	return new_node;
}

fpgcolcntPtr
create_fpgcolcnt(int col, int sup)
{
	fpgcolcntPtr new_node =
		(fpgcolcntPtr) malloc(sizeof(struct fpgcolcnt));
	new_node->col_num = col;
	new_node->support = sup;

	return new_node;
}

/**
 Note: Get size of data array. 
 */
void
create_fptree(fptreePtr fptree)
{
	int i = 0;
	fptree = (fptreePtr) malloc(sizeof(struct fptree));
	fptree->root = create_fptree_node(1);
	fptree->header_table = create_fpgheader(0);

	/* create header tables as population progresses */
	for (i = 0; i < num_oflines; i++) {
		if (data[i] != NULL)
			add_to_fptree(fptree->root, 0, data[i],
				      BASKET_MAX_CHARS, 1,
				      fptree->header_table);
	}
}

void
add_to_fptree(fptreenodePtr ref, int place, int itemset[], int size, int sup,
	      fpgheaderPtr header)
{
	if (place < size) {
		if (!add_to_fptree1(ref, place, itemset, size, sup, header))
			add_to_fptree2(ref, place, itemset, size, sup,
				       header);
	}
}

bool
add_to_fptree1(fptreenodePtr ref, int place, int itemset[], int size, int sup,
	       fpgheaderPtr header)
{

	while (ref->child != NULL) {
		if (itemset[place] == ref->child->node->item_name) {
			ref->child->node->item_count += sup;
			num_updates++;
			add_to_fptree(ref->child, place + 1, itemset, size,
				      sup, header);

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
void
add_ref_to_fpghtable(int col_num, fpgsubtreePtr new_node, fpgheaderPtr header)
{

	while (header != NULL) {
		if (col_num == header->item_name) {
			header->node = new_node;
			break;
		}
		header = header->next;
	}
}

void
add_rem_itemsets(fptreenodePtr ref, fpgsubtreePtr back, int place,
		 int itemset[], int size, int sup, fpgheaderPtr header)
{
	fpgsubtreePtr subtree;
	fptreenodePtr fpnode;
	/* Process if more items in itemset, check if itemset value is invalid(-1) then skip. */
	if (place < size && place >= 0 && itemset[place] >= 0) {
		subtree = create_fpsubtree_node(itemset[place], sup, ref->node);	/* Create new prefix subtree node */
		fpnode = create_fptree_node(sup);	/* create new fptree node incorporating subtree node */
		fpnode->node = subtree;
		add_ref_to_fpghtable(itemset[place], subtree, header);	/* Adds link to header table */
		ref->child = fpnode;
		/* Add into FP tree */
		add_rem_itemsets(fpnode, subtree, place + 1, itemset, size, sup, header); /** Proceed down branch with rest of itemsets */
	}

	if (place < size && place >= 0 && itemset[place] < 0)
		add_rem_itemsets(ref, back, place + 1, itemset, size, sup,
				 header);

	return;
}

void
add_to_fptree2(fptreenodePtr ref, int place, int itemset[], int size, int sup,
	       fpgheaderPtr header)
{
	fpgsubtreePtr subtree;
	fptreenodePtr fpnode;

	/* check if itemset value is invalid(-1) then skip. */
	if (itemset[place] >= 0) {
		subtree = create_fpsubtree_node(itemset[place], sup, ref->node);	/* Create new prefix subtree node */
		fpnode = create_fptree_node(sup);	/* create new fptree node incorporating subtree node */
		fpnode->node = subtree;


		/** Note: Still to implement this function */
		add_ref_to_fpghtable(itemset[place], subtree, header);	/* Adds link to header table */
		ref->child = fpnode;	/* Add into FP tree */
	}

	add_rem_itemsets(fpnode, subtree, place + 1, itemset, size, sup, header);	/* Proceed down branch with rest of itemsets */
}

void
start_mining(fptreePtr fptree)
{
	int itemset[2];
	/* Loop through header table item by item */
	while (fptree->header_table != NULL) {
		/* check for null link */
		if (fptree->header_table->node != NULL) {
			start_mining2(fptree, fptree->header_table->node,
				      fptree->header_table->item_name,
				      itemset, 2);
		}
	}
}

void
start_mining2(fptreePtr fptree, fpgsubtreePtr node, int item,
	      int itemset_sofar[], int size)
{
	fpgcolcntPtr count[size];
	fpgheaderPtr lheader;
	fptreePtr ltree;
	int support, i;
	int code_sofar[size + 1];
	char freq_file[20];
	char str[15];
	FILE *fp;

	/** Count support for current item in header table and store in a tree */
	support = gensup_headtable(node);
	code_sofar[0] = item;/** Resizes given array so that its length is increased by one element, new element added to front */
	for (i = 0; i < size; i++)
		code_sofar[i + 1] = itemset_sofar[i];

	/** Write frequent itemsets, add support count */
	strcpy(freq_file, "../freq_j_");
	sprintf(str, "%d", size + 1);
	strcat(freq_file, str);
	strcat(freq_file, "_itemsets");
	fp = fopen(freq_file, "w");
	if (fp == NULL) {
		fprintf(stderr, "\n Error opening file: %s", freq_file);
		exit(EXIT_SUCCESS);
	}




	if (support >= SUPPORT_THRESHOLD) {
		for (i = 0; i < size; i++)
			fprintf(fp, " %d", code_sofar[i]);

		fprintf(fp, " %d; %d\n", code_sofar[i], support);
	}

	/** Collect ancestor itemsets and store in linked list */
	generate_ancestor(fptree->start_tmp_sets, node);

	/** Process ancestor itemsets. */
	if (fptree->start_tmp_sets != NULL) {
		count_fpgsingles(fptree);	/* Count singles in linked list */
		lheader = create_local_htable(count);	/* Create and pop local header table */
		if (lheader != NULL) {
			//prune_ancestors(fptree, count); /* Prune ancestor itemsets */
			ltree->root = gen_local_fptree(fptree, lheader);	/* Create new local root for local FP tree */
			start_mining2(ltree, lheader->node, lheader->item_name, code_sofar, size + 1);	/* Mine new FP tree */
		}

	}
}

int
gensup_headtable(fpgsubtreePtr node)
{
	int count = 0;
	while (node != NULL) {
		count += node->item_count;
		num_updates++;
		node = node->next;
	}
	return 0;
}

void
generate_ancestor(fpgsupsetsPtr start_tmp_sets, fpgsubtreePtr ref)
{
	int sup;

	while (ref != NULL) {
		sup = ref->item_count;
		get_ancestor(ref->parent);

		/* Add to linked list with current support */
		if (itemset != NULL)
			start_tmp_sets =
				create_fpgsupsets(itemset, size, sup,
						  start_tmp_sets);

		ref = ref->next;
	}

}

int *
get_ancestor(fpgsubtreePtr ref)
{

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

int
rem_elt(int old_itemset[], int n)
{
	int i;
	if (size <= n)
		return 0;
	else {
		for (i = n; i < size; i++)
			old_itemset[i] = old_itemset[i + 1];
	}

	return 0;
}

void
prune_ancestors(fptreePtr fptree, fpgcolcntPtr count[])
{
	fpgsupsetsPtr ref = fptree->start_tmp_sets;
	int i;
	while (ref != NULL) {
		for (i = 0; i < colcnt; i++) {
			if (count[ref->item_set[i]]->support <
			    SUPPORT_THRESHOLD)
				rem_elt(ref->item_set, i);/** Removes in frequent itemset */
		}

		ref = ref->next;
	}
}

int
count_fpgsingles(fptreePtr fptree)
{
	int index, place = 0;

	fpgsupsetsPtr supsets = fptree->start_tmp_sets;
	count[0] = create_fpgcolcnt(0, 0);
	/* Init array */
	for (index = 1; index < OTH_ITEMSET_ARRAY_MAX; index++)
		count[index] = create_fpgcolcnt(0, 0);

	while (supsets != NULL) {
		for (index = 0; index < OTH_ITEMSET_ARRAY_MAX; index++) {
			place = supsets->item_set[index];
			count[place]->support += supsets->support;
			num_updates++;
		}
		supsets = supsets->next;
	}
	colcnt = index;
	return 0;
}


fpgheaderPtr
create_local_htable(fpgcolcntPtr count[])
{
	int index;
	fpgheaderPtr lhtable;
	lhtable = local_htable_unordered(count);

	return lhtable;
}

fpgheaderPtr
local_htable_unordered(fpgcolcntPtr count[])
{
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
	fpgheaderPtr lhtable;
	place = 1;
	for (i = 1; i < colcnt; i++) {
		if (count[i]->support >= SUPPORT_THRESHOLD) {
			lhtable = create_fpgheader(count[i]->col_num);
			lhtable->next;
		}
	}

	return lhtable;
}

void
local_htable_ordered(fpgheaderPtr table[], int hsize, fpgcolcntPtr count[],
		     int size)
{
	bool order;
	fpgheaderPtr tmp;
	int i, place1, place2;

	do {
		i = 1;
		order = true;
		while (i < hsize - 1) {
			place1 = table[i]->item_name;
			place2 = table[i + 1]->item_name;
			if (count[place1]->support > count[place2]->support) {	/* swap */
				order = false;
				tmp = table[i];
				table[i] = table[i + 1];
				table[i + 1] = tmp;
			}
			i++;
		}
	} while (order == false);
}

fptreenodePtr
gen_local_fptree(fptreePtr fptree, fpgheaderPtr table)
{
	fptreePtr ltree;
	ltree->start_tmp_sets = fptree->start_tmp_sets;
	ltree->root = create_fptree_node(0);

	while (ltree->start_tmp_sets != NULL) {
		if (ltree->start_tmp_sets->item_set != NULL)
			add_to_fptree2(ltree->root, 0,
				       ltree->start_tmp_sets->item_set, size,
				       ltree->start_tmp_sets->support, table);

		ltree->start_tmp_sets = ltree->start_tmp_sets->next;
	}

	return ltree->root;
}

fptreenodePtr
realloc_fptree(fptreenodePtr old, fptreenodePtr new_node)
{
	fptreenodePtr tmp;

	if (old == NULL) {
		old = create_fptree_node(new_node->support);
		old->num_nodes += new_node->num_nodes;
		old->node = new_node->node;
		old->fpgheader = new_node->fpgheader;
		old = old->child;
		return old;
	}

	while (old != NULL) {
		if (new_node->node->item_name < old->node->item_name) {
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

void
out_fpsubtree(fpgheaderPtr htable)
{

	printf("\nPrefix Subtree from Header Table");
	printf("\n HEADER = %d ,", htable->item_name);
	while (htable->node != NULL) {
		printf("\nSub tree: %d, %d, ", htable->node->item_count,
		       htable->node->item_name);
		htable->node = htable->node->next;
	}
}

int
out_fpsubtree2(fpgsubtreePtr ref)
{
	int counter = 1;
	while (ref != NULL) {
		printf("\n Counter: %d, item_name: %d , item_count: %d",
		       counter, ref->item_name, ref->item_count);
		counter++;
		ref = ref->next;
	}

	return counter;
}

void
out_fptree(fptreePtr ref)
{
	fptreenodePtr tmp = ref->root;
	if (ref == NULL)
		return;

	while (tmp != NULL) {
		out_fpsubtree2(tmp->node);
		tmp = tmp->child;
	}

}

int
calc_storage(fptreenodePtr ref, int storage)
{
	if (ref == NULL)
		return storage;

	while (ref != NULL) {
		storage += 20;
		ref = ref->child;
	}
	return storage;
}

void
out_fptree_storage(fptreenodePtr root)
{
	int storage = 8;
	storage = calc_storage(root->child, storage);

	printf("\n FP Tree storage = %d bytes", storage);
	printf("\n FP tree update = %ld", num_updates);
	printf("\n\n");
}

void
input_dataset()
{

	FILE *trans;
	char item[ITEM_MAX];
	char basket[BASKET_MAX_ITEMS];
	int i, j, item_no, num_lines = 0;
	char *p;

	trans = fopen(TRANS_DB, "r");
	if (trans == NULL) {
		fprintf(stderr,
			"\n Error Opening Transaction Database file: %s",
			TRANS_DB);
		exit(EXIT_SUCCESS);
	}


	/* read one line at a time from the file */
	while (fgets(basket, BASKET_MAX_CHARS, trans) != NULL) {

		for (i = 0; i < BASKET_MAX_ITEMS; i++)
			data[num_lines][i] = -1;

		item_no = 0;

		/*read line items into items array */
		p = basket;

		/* stops at the end of string character */
		while (*p != '\0') {
			if (*p == '\0')
				break;
			while (isspace(*p)) {	/* skip spaces */
				++p;
				continue;
			}
			if (*p == '\0')
				break;
			j = 0;
			while (!isspace(*p)) {
				item[j++] = *p;
				++p;
			}
			item[j] = '\0';	//stores null at end of string.

			data[num_lines][item_no] = atoi(item);
			item_no++;
		}
		num_lines++;
	}
	num_oflines = num_lines;

}

/** Sorts array in descending order */
void
order_count_array()
{
	int attr, qty, i, j;
	for (i = 0; i < MAX_LINES - 1; i++)
		j = i + 1;
	while (conv[j][1] > conv[i][1] && j < MAX_LINES) {
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

void
order_input_data()
{
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

/* GET NUM OF SUPPORTED ONE ITEM SETS */
int
gen_freq_one_itemsets()
{
	int i, j, counter = 0;
	char freq_file[20];
	char str[15];
	FILE *fp;
	strcpy(freq_file, "../freq_j_");
	sprintf(str, "%d", 1);
	strcat(freq_file, str);
	strcat(freq_file, "_itemsets");

	fp = fopen(freq_file, "w");
	if (fp == NULL) {
		fprintf(stderr, "\n Error opening file: %s", freq_file);
		exit(EXIT_SUCCESS);
	}

	for (i = 0; i < MAX_LINES; i++) {
		if (conv[i][1] >= SUPPORT_THRESHOLD) {
			fprintf(fp, "\n %d; %d", conv[i][0], conv[i][1]);
			counter++;
		}
	}

	return counter;
}

void
recast_data_prune_unsupported()
{
	int itemset[BASKET_MAX_CHARS][2];
	int attr, i, j, k = 0;

	for (i = 0; i < MAX_LINES; i++) {
		k = 0;
		if (data[i] != NULL) {
			for (j = 0; j < BASKET_MAX_CHARS; j++) {
				attr = data[i][j];

				/** check support */
				if (conv[attr][1] >= SUPPORT_THRESHOLD) {
					itemset[k][0] = conv[attr][0];
					itemset[k++][1] = conv[attr][1];
				}
				data[i][j] = -1;	/* reset array. */
			}

			for (j = 0; j < k; j++)
				data[i][j] = itemset[j][0];
		}
	}
}

void
clear_subtree(fpgsubtreePtr stree)
{
	fpgsubtreePtr ts;
	while (stree != NULL) {
		ts = stree;
		stree = stree->next;
		ts->item_name = 0;
		ts->item_count = 0;
		free(ts->parent);
		free(ts);
	}
}

void
clear_header(fpgheaderPtr t_header)
{
	fpgheaderPtr thead;

	while (t_header != NULL) {
		thead = t_header;
		t_header = t_header->next;
		thead->item_name = 0;
		free(thead->node);
		free(thead);
	}
}

void
clear_supsets(fpgsupsetsPtr supsets)
{
	fpgsupsetsPtr t_sups = supsets;

	while (supsets != NULL) {
		t_sups = supsets;
		supsets = supsets->next;
		free(t_sups->item_set);
		t_sups->support = 0;
		free(t_sups);
	}
}

void
release_memory(fptreePtr fptree)
{
	fptreenodePtr t_head;
	fpgsubtreePtr ts_tree;
	fpgheaderPtr t_header = fptree->header_table;
	fpgsupsetsPtr t_sups = fptree->start_tmp_sets;

	clear_header(t_header);
	clear_supsets(t_sups);
	fptree->tmp_index = 0;
	fptree->num_nodes = 0;

	/** clear all root nodes */
	while (fptree->root != NULL) {
		t_head = fptree->root;
		fptree->root = fptree->root->child;
		clear_subtree(t_head->node);
		clear_header(t_head->fpgheader);
		t_head->support = 0;
		t_head->num_nodes = 0;
		free(t_head);
	}
}
