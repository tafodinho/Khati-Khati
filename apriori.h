/*
 * File: apriori.h
 * Purpose: Defines the functions to be used in the apriori algorithm.
 *
 */


#ifndef APRIORI_H
#define APRIORI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRANS_DB "retail.dat.tmp" /* Transactional database */

#define BASKET_MAX_CHARS 100000
#define BASKET_MAX_ITEMS 1000
#define ITEM_MAX 10
#define ONE_ITEMSET_ARRAY_MAX 16500
#define OTH_ITEMSET_ARRAY_MAX 524287
#define ITEMSET_MAX_SIZE 10
#define SUPPORT_THRESHOLD 5


typedef struct itemsets * Itemsets;
typedef struct itemsetPtr * ItemsetPtr;
typedef struct c_itemsets * C_ItemsetPtr; /* current candidate set pointer */

struct c_itemsets {
    int * itemsets;
    C_ItemsetPtr next;
};

struct itemsets{
    int * itemsets;
    int cnt;
    Itemsets next;
};

struct itemsetPtr{
    Itemsets itemset_ptr;
    int distinct_itemsets;
    int itemsets_cnt;
};

/*Generate Assciation rules based on:
 * -- Min Support(#transaction A & B/Total # of transactions)
 * -- Min confidence(#transaction A & B/ #transactions of A)
 * -- Generate Item-sets based on previous K-Item sets.
 */
int * candidate_frequencies1;

int * frequent_frequencies1;

ItemsetPtr candidate_frequencies_prev;
ItemsetPtr frequent_frequencies_prev;
ItemsetPtr candidate_frequencies_cur;
ItemsetPtr frequent_frequencies_cur;

/* Reads the Transaction database and stores items on Arrays */
void scandb(int n, int * c_freq1, int * f_freq1, ItemsetPtr c_freq_cur, ItemsetPtr f_freq_cur, ItemsetPtr c_freq_prev, ItemsetPtr f_freq_prev);

/* Generates the candidate item sets for k = 1, 2, ..n */
void generate_candidate_one_itemsets(int *,int[],int);
void generate_candidates_prev(int itemsetcnt,ItemsetPtr c_prev,ItemsetPtr f_cur);
void apriori_generate_cand_2_itemsets(int * f_freq1, int itemset_cnt,ItemsetPtr c_freq_cur,int items[],C_ItemsetPtr c_itemset_ll,int item_cnt,ItemsetPtr c_prev, ItemsetPtr f_cur, FILE *f_itemsets, int *distinct_itemsets_cnt, int *tot_itemsets_cnt);
void apriori_generate_cand_n_itemsets(ItemsetPtr f_freq1, int itemset_cnt,ItemsetPtr c_freq_cur,int items[],C_ItemsetPtr c_itemset_ll,int item_cnt,ItemsetPtr c_prev, ItemsetPtr f_cur, FILE *f_itemsets, int *distinct_itemsets_cnt, int *tot_itemsets_cnt);


/* Saves the frequent itemsets to a file., k = 1,2, ..n */
void save_frequent_one_itemsets(FILE *f_itemsets,int * c_freq1,int,int);
void save_frequent_n_itemsets(int, FILE *f_itemsets,ItemsetPtr f_cur,int,int);


/* gets the frequent item sets for k = 1, 2 .. n */
void get_frequent_one_itemsets(int * c_freq1,int * f_freq1,int *distinct_itemsets_cnt, int *tot_itemsets_cnt);
void get_frequent_n_itemsets(int, ItemsetPtr c_cur,ItemsetPtr f_cur, int *, int *);


/* Creates new item set node from Array */
Itemsets create_new_itemset_node(int itemset_cnt,int itemset_array[]);

/* Creates the Ck+1 itemset list */
void insert_candidate_itemset(int itemset_cnt,int hval,int itemset_array[],ItemsetPtr c_cur);

/* clears memory of list */
void release_memory(ItemsetPtr freq);

/*  hashes itemset elements on array. */
int hashval(int itemset[],int len);


#endif
