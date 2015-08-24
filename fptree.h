/*
 * File: apriori.h
 * Purpose: Defines the functions and structures to be used in the FP Growth algorithm.
 * Author: Bawe Emmanuel
 *
 */
 
#ifndef FPTREE_H
#define FPTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define TRANS_DB "retail.dat.tmp" /* Transactional database */

#define BASKET_MAX_CHARS 100000
#define BASKET_MAX_ITEMS 1000
#define ITEM_MAX 10
#define ONE_ITEMSET_ARRAY_MAX 16500
#define OTH_ITEMSET_ARRAY_MAX 524287
#define ITEMSET_MAX_SIZE 10
#define SUPPORT_THRESHOLD 5


typedef struct fpgsubtree_node *fpgsubtreePtr;
typedef struct fptree_node * fptreePtr;
typedef struct fpgheader *fpgheaderPtr;
typedef struct fpgsupsets *fpgsupsetsPtr;

/**
 * fptree_node: FP tree node. This the basic DS for the fp growth algorithm
 *             used in the FP tree Arrays of this structure are used to store nodes
 *             at the same level in any sub-branch of the fptree 
 * Note:       Increment num_nodes each time a node is created.(get num_nodes)
 */
struct fptree_node {
	int support;             /* support count associated with the itemset represented by node */
	int num_nodes;           /* number of nodes on the total support tree */
	fptreePtr child;         /* Reference to child(if any) for this node */
	fpgsubtreePtr fpsubtree; /* store counts and a reference to a child branch. */
	fpgheaderPtr fpgheader;  /*  used to link nodes on the fptree, enables cross linking */
};

/**
 * create_fptree_node(): creates an fptree node with support arg 
 *
 * @param sup, support count
 * @return new node
 */
fptreePtr create_fptree_node(int sup);

	
/**
 * fpgsubtree_node: This is a set enumeration tree which stores itemsets 
 *                    together with support values.
 *                    it is the FP growth Item prefix subtree node.
 */
struct fpgsubtree_node {
	int item_name;       /* attribute identifier */
	int item_count;      /* item support count */
	fpgsubtreePtr parent;/* backward link to the parent node on fptree */
	fpgsubtreePtr next;  /* forward link to next node starting with an elt in the header table */
};

/**
 * create_fpsubtree_node(): Three argument constructor
 * 
 * @param name, the itemset identifier. 
 * @param support,  the support value for the itemset.
 * @param prev, the backward link to the parent node. 
 * @return new node
 */
fpgsubtreePtr create_fpsubtree_node(int name, int support, fpgsubtreePtr prev);

/**
 * fpgheader: A header table, array of structures used to link into fp tree.
 *            All FPtree nodes with the same identifier are linked together
 *            starting from a node in a header table.(Cross linking occurs here)
 */
struct fpgheader {
	int item_name; /* 1 itemset attribute identifier */
	fpgsubtreePtr next; /* forward link to the next node */
}

/**
 * create_fpgheader(): creates an FP growth Header table node.
 *
 * @param col_num, gives the attribute name.
 * @return new fp header table node
 */
fpgheaderPtr create_fpgheader(int col_num);

/**
 * fpgsupsets : Stores ancestor itemsets, nodes in an FP tree that preceed the nodes 
 *              identified by following trails from a particular item on the header table.
 */
struct fpgsupsets {
	int *item_set;      /* item set label */
	int support;        /* support value for a given itemset */
	fpgsupsetsPtr next; /* references to next node on linked list. */
};

/**
 * create_fpgsubsets(): Creates new fp growth subsets
 * 
 * @param itemsets
 * @param support
 * @param next_node
 * @return new fp growth support sets.
 */
fpgsupsetsPtr create_fpgsubsets( int itemsets[], int size, int sup, fpgsubsetsPtr next);



#endif
