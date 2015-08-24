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
#include <ctype.h>

#define TRANS_DB "retail.dat.tmp" /* Transactional database */

#define BASKET_MAX_CHARS 100000
#define BASKET_MAX_ITEMS 1000
#define ITEM_MAX 10
#define ONE_ITEMSET_ARRAY_MAX 16500
#define OTH_ITEMSET_ARRAY_MAX 524287
#define ITEMSET_MAX_SIZE 10
#define SUPPORT_THRESHOLD 5


/**
 * Ttree_node: Total support tree node. This the basic DS for the Total support tree
 *             used in the FP tree Arrays of this structure are used to store nodes
 *             at the same level in any sub-branch of the Ttree(total support tree). 
 * Note:       Increment num_nodes each time a node is created.(get num_nodes)
 */
typedef struct Ttree_node * totsuptreePtr;
struct Ttree_node {
	int support = 0;      /* support count associated with the itemset represented by node */
	int num_nodes = 0;    /* number of nodes on the total support tree */
	totsuptreePtr child;  /* Reference to child(if any) for this node */
};





#endif
