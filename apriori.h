/**
 * File: apriori.h
 * Purpose: Defines the functions to be used in the apriori algorithm.
 * Author: Bawe Emmanuel
 *
 */

#ifndef APRIORI_H
#define APRIORI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define TRANS_DB "retail.dat.tmp"	/* Transactional database */

#define BASKET_MAX_CHARS 100000
#define BASKET_MAX_ITEMS 1000
#define ITEM_MAX 10
#define ONE_ITEMSET_ARRAY_MAX 16500
#define OTH_ITEMSET_ARRAY_MAX 524287
#define ITEMSET_MAX_SIZE 10
#define SUPPORT_THRESHOLD 5


typedef struct itemsets *Itemsets;
typedef struct itemsetPtr *ItemsetPtr;
typedef struct c_itemsets *C_ItemsetPtr;	/* current candidate set pointer */

struct c_itemsets
{
	int *itemsets;
	C_ItemsetPtr next;
};

struct itemsets
{
	int *itemsets;
	int cnt;
	Itemsets next;
};

struct itemsetPtr
{
	Itemsets itemset_ptr;
	int distinct_itemsets;
	int itemsets_cnt;
};

/*Generate Assciation rules based on:
 * -- Min Support(#transaction A & B/Total # of transactions)
 * -- Min confidence(#transaction A & B/ #transactions of A)
 * -- Generate Item-sets based on previous K-Item sets.
 */
int *candidate_frequencies1;
int *frequent_frequencies1;

ItemsetPtr candidate_frequencies_prev;
ItemsetPtr frequent_frequencies_prev;
ItemsetPtr candidate_frequencies_cur;
ItemsetPtr frequent_frequencies_cur;

/* Reads the Transaction database and stores items on Arrays */
void scandb(int n, int *c_freq1, int *f_freq1, ItemsetPtr c_freq_cur,
	    ItemsetPtr f_freq_cur, ItemsetPtr c_freq_prev,
	    ItemsetPtr f_freq_prev);

/* Checks for infrequent subsets */
int infrequent_subset_found(int subset_len, int items[], int basket_cnt,
			    ItemsetPtr c_prev);
int r_infrequent_subset_found(int slen, int items[], int sub[], int ilen,
			      ItemsetPtr c_prev, int istart, int sstart);

/* checks for frequent subsets*/
int is_frequent_subset(ItemsetPtr c_prev, int sub[], int slen);

/* Generates the candidate item sets for k = 1, 2, ..n */
void generate_candidate_one_itemsets(int *, int[], int);
void apriori_generate_cand_n_itemsets(ItemsetPtr f_freq1, int itemset_cnt,
				      ItemsetPtr c_freq_cur, int items[],
				      C_ItemsetPtr c_itemset_ll, int item_cnt,
				      ItemsetPtr c_prev, ItemsetPtr f_cur,
				      FILE * f_itemsets,
				      int *distinct_itemsets_cnt,
				      int *tot_itemsets_cnt);
void r_apriori_generate_cand_n_itemsets(int slen, ItemsetPtr c_cur,
					int items[], int sub[],
					C_ItemsetPtr c_itemset_ll, int ilen,
					int istart, int sstart,
					ItemsetPtr c_prev);

/* Joins previous frequent itemsets to get current candidate itemsets */
void join_frequent_n_itemsets(ItemsetPtr c_cur, ItemsetPtr f_prev, int i_cnt,
			      int *distinct_itemsets_cnt,
			      int *tot_itemsets_cnt);

/* Saves the frequent itemsets to a file., k = 1,2, ..n */
void save_frequent_one_itemsets(FILE * f_itemsets, int *c_freq1, int, int);
void save_frequent_n_itemsets(int, FILE * f_itemsets, ItemsetPtr f_cur, int,
			      int);


/* gets the frequent item sets for k = 1, 2 .. n */
void get_frequent_one_itemsets(int *c_freq1, int *f_freq1,
			       int *distinct_itemsets_cnt,
			       int *tot_itemsets_cnt);


/* Creates new item set node from Array */
Itemsets create_new_itemset_node(int itemset_cnt, int itemset_array[]);

/* Creates the Ck+1 itemset list */
void insert_candidate_itemset(int itemset_cnt, int hval, int itemset_array[],
			      ItemsetPtr c_cur);

/* clears memory of list */
void release_memory(ItemsetPtr freq);

/*  hashes itemset elements on array. */
int hashval(int itemset[], int len);

#endif
