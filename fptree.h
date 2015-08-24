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

typedef struct fptree_node      *fptreenodePtr;
typedef struct fpgsubtree_node  *fpgsubtreePtr;
typedef struct fptree_node      *fptreePtr;
typedef struct fpgheader        *fpgheaderPtr;
typedef struct fpgsupsets       *fpgsupsetsPtr;
typedef struct fpgcolcnt        *fpgcolcntPtr;
typedef struct fptree           *fptreePtr;

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
	fpgsubtreePtr node; /* store counts and a reference to a child branch. */
	;  /*  used to link nodes on the fptree, enables cross linking */
};

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
 * fpgheader: A header table, array of structures used to link into fp tree.
 *            All FPtree nodes with the same identifier are linked together
 *            starting from a node in a header table.(Cross linking occurs here)
 */
struct fpgheader {
	int item_name; /* 1 itemset attribute identifier */
	fpgsubtreePtr next; /* forward link to the next node */
}

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
 * fpgcolcnt: stores the counts
 */
struct fpgcolcnt {
	int col_num; /* column/attribute ID number */
	int support; /* associated support count */
};

/**
 * fptree: Structure is built to store fp tree nodes.
 */
struct fptree {
	
    /** Start reference for FP-tree. */
    fptreenodePtr root;
    /** Start reference for header table. */
    fpgheaderPtr header_table; 
    /** Start reference for supportedSets linked list (temporary storage only).*/
    fpgsupsetsPtr start_tmp_sets;
    
    // Other fields 
    
    /** Temporary storage for an index into an array of FP-tree nodes.
     *  Used when reassigning child reference arrays. */
    int tmp_index;
    
    /** Number of nodes created. */
    int num_nodes;
};

/**
 * create_fptree_node(): creates an fptree node with support arg 
 *
 * @param sup, support count
 * @return new node
 */
fptreePtr create_fptree_node(int sup);

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
 * create_fpgheader(): creates an FP growth Header table node.
 *
 * @param col_num, gives the attribute name.
 * @return new fp header table node
 */
fpgheaderPtr create_fpgheader(int col_num);

/**
 * create_fpgsubsets(): Creates new fp growth subsets
 * 
 * @param itemsets
 * @param support
 * @param next_node
 * @return new fp growth support sets.
 */
fpgsupsetsPtr create_fpgsubsets( int itemsets[], int size, int sup, fpgsubsetsPtr next);

/**
 * create_fpgcolcnt():
 *
 * @param col, the column/attribute ID number
 * @param sup, the associated support value
 */
fpgcolcntPtr create_fpgcolcnt(int col, int sup);

/** ------ FUNCTIONS ------ */

/** CREATE FP-TREE */   
/** Top level method to commence the construction of the FP-Tree. */
void create_fptree();
   
/** 
 * add_to_fptree(): Searches through current list of child refs looking for given item set.
 *                  If reference for current itemset found increments support count and 
 *                  proceed down branch, otherwise adds to current level. 
 * @param ref the current location in the FP-tree (rootNode at start).
 * @param place the current index in the given itemset.
 * @param itemset the given itemset.
 * @param sup the associated support value for the given itemset.
 * @param header the link to the appropriate place in the header table.
 */
void add_to_fptree(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header);
   
/**
 * add_to_fptree1():  Searches through existing branch and if itemset found updates the 
 *                    support count and returns true, otherwise return false. 
 * @param ref the current FP-tree node reference.
 * @param place the current index in the given itemset.
 * @param itemset the given itemset.
 * @param sup the associated support value for the given itemset.
 * @param header the link to the appropriate place in the header table. 
 * @return true if given itemset exists in FP-tree, and false otherwise. 
 */ 
bool add_to_fptree1(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header)
    
/** 
 * add_to_fptree2(): Adds new node to FP-tree. (Adds first attribute in itemSet and then rest of sequence). 
 *
 * @param ref the current FP-tree node reference.
 * @param place the current index in the given itemset.
 * @param itemset the given itemset.
 * @param sup the associated support value for the given itemset.
 * @param header the link to the appropriate place in the header table.
 */
void add_to_fptree2(fptreenodePtr ref, int place, int itemset[], int size, int sup, fpgheaderPtr header)
    
/**
 * add_rem_itemsets(): Continues adding attributes in current itemset to FP-tree.
 *
 * @param ref the current FP-tree node reference.
 * @param back the backwards link to the previous node.
 * @param place the current index in the given itemset.
 * @param itemset the given itemset.
 * @param sup the associated support value for the given itemset.
 * @param header the link to the appropriate place in the header table.
 */
void add_rem_itemsets(fptreenodePtr ref, fpgsubtreePtr back, tnt place, int itemset[], int size, int sup, 
					fpgheaderPtr header);

/**  -----------------------------------------------------------------
    *                                                           
    *                       FP-TREE MINING                      
    *                                                           
    * ---------------------------------------------------------------
    
    ** Methodology:

    1) Step through header table from end to start (least common single 
    attribute to most common single attribute). For each item.
    a) Count support by following node links and add to linked list of 
       supported sets.
    b) Determine the "ancestor trails" connected to the nodes linked to the
       current item in the header table.
    c) Treat the list of ancestor itemsets as a new set of input data and 
       create a new header table based on the accumulated supported counts of 
       the single items in the ancestor itemsets 
    d) Prune the ancestor itemsets so as to remove unsupported items.
    e) Repeat (1) with local header table and list of pruned ancestor itemsets 
       as input 
*/
/** START MINING FUNCTIONS */

/**
 * start_mining():  Commences process of mining the FP tree. Commence with the bottom 
 *                  of the header table and work upwards. Working upwards from the bottom of 
 *                  the header table if there is a link to an FP tree node :
 *                  Count the support.
 *                  Build up itemSet sofar.
 *                  Add to supported sets.
 *                  Build a new FP tree: (i) create a new local root, (ii) create a 
 *                  new local header table and (iii) populate with ancestors.
 *                  If new local FP tree is not empty repeat mining operation.
 *                  Otherwise end. 
 * @param  table the reference to the current location in the header table (commencing with the last item).
 * @param itemset_sofar the label fot the current item sets as generated to date (null at start). */	
void start_mining(fpgheaderPtr table, int itemset_sofar[], int size);

/**
 * start_mining2():  Commence process of mining FP tree with respect to a single element in the header table.
 *
 * @param nodeLin the first link from the header table pointing to an FP-tree node.
 * @param item the label associated with the element of interest in the header table.
 * @param itemset_sofar the item set represented by the current FP-tree.
 * @param size
 */
void start_mining2(fpgsubtreePtr node, int item, int itemset_sofar[], int size);


/** ---------------------------------------------------------------------                                                                       
 *
 *                     PROCESS CURRENT HEADER TABLE                       
 * ----------------------------------------------------------------------   
 *  
 * GENERATE SUPPORT FOR HEADER TABLE SINGLE ITEM: 
 *
 * gensup_headtable(): Counts support for single attributes in header table by following node links. 
 * @param nodeLink the start link from the header table. 
 * @return the support valye for the item set indicated by the header table. 
 */
int gensup_headtable(fpgsubtreePtr node);

/**
 *  GENERATE ANCESTOR CODES 
 *  generate_ancestor(): Generates ancestor itemsets are made up of the parent nodes of a given 
 *                       node. This method collects such itemsets and stores them in a linked list 
 *                       pointed at by start_tmp_sets. 
 *  @param ref the reference to the current node in the prefix tree containing itemsets together with support values.
 */
void generate_ancestor(fpgsubtreePtr ref);

/**
 * GET ANCESTOR CODE
 * get_ancestor():  Generate the ancestor itemset from a given node. 
 * @param ref the reference to the current node in the prefix tree containing itemsets together with support values.
 */
int *get_ancestor(fpgsubtreePtr ref);

/**
 * PRUNE ANCESTOR CODES
 * prune_ancestor(): Removes elements in ancestor itemSets (pointed at by next) which are not supported by referring to count 
 *                   array (which contains all the current supported 1 itemsets). 
 * @param count      the array of fpgcolcnts structures 
 *                   describing the single item sets (in terms of labels and associated 
 *                   support), contained in a linked list of fpgsupsets
 *                   which in turn describe the ancestor nodes in an FP-tree that preceed the 
 *                   nodes identified by following a trail of links from a particular item in 
 *                   the header table.
 */
void prune_ancestors(fpgcolcntPtr count[], int size);

/** --------------------------------------------------------------------- 
 *                                                                        
 *      CREATE NEW HEADER TABLE FROM SINGLE ITEMS IN ANCESTOR CODES       
 *                                                                        
 * ----------------------------------------------------------------------   
 * COUNT SINGLES
 * count_fpgsingles(): Counts frequent 1 item sets in ancestor itemSets linked list and place into an array. 
 * @return array of @fpgcolcntPtr structures describing the  single item sets (in terms of labels and 
 *         associated support), contained in a linked list of @fpgsupsetsPtr which in turn describe the 
 *         ancestor nodes in an FP-tree that preceed the nodes identified by following 
 *         a trail of links from a particular item in the header table. 
 */
fpgcolcntPtr count_fpgsingles();

/**
 * CREATE LOCAL HEADER TABLE
 * create_local_htable(): Creates a local header table comprising those item that are supported in the count array. 
 * @param count the support for the 1 item sets. 
 * @return a FPgrowth header table.
 */
fpgheaderPtr create_local_htable(fpgcolcntPtr count[], int size);

/**
 * CREATE NEW LOCAL HEADER TABLE (UNORDERED)
 * local_htable_unordered(): Creating a new local header table, but unordered. 
 * @param count the support for the 1 itemsets. 
 * @return a FPgrowth header table.
 */
fpgheaderPtr local_htable_unordered(fpgcolcntPtr count[], int size);

/**
 * ORDER LOCAL HEADER TABLE
 * local_htable_ordered(): Orders local header table (currently unused).  
 * @param localHeaderTable the FPgrowth header table to be ordered.
 * @param count the support for the 1 item sets.
 */
void local_htable_ordered(fpgheaderPtr table[], int hsize, fpgcolcntPtr count[], int size);

/** -------------------------------------------------------------------
 *                                                                     
 *                         GENERATE NEW FP-TREE                        
 *                                                                   
 * -------------------------------------------------------------------- 
 * GENERATE LOCAL FP-tree
 * gen_local_fptree(): Generates a local FP tree 
 * @param table reference to start of header table containing links to an FP-tree produced during the FP-tree generation process.
 * @rerurn reference to the start of the generated FP-tree.
 */
fptreenodePtr gen_local_fptree(fpgheaderPtr table[], int hsize);


#endif
