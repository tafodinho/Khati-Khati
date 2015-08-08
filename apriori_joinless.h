/*
 * File:   apriori_joinless.h
 * Author: root
 *
 * Created on April 18, 2015, 6:12 AM
 */


#ifndef APRIORI_JOINLESS_H
#define APRIORI_JOINLESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRANS_DB "retail.dat.tmp"

#define BASKET_MAX_CHARS 100000
#define BASKET_MAX_ITEMS 1000
#define ITEM_MAX 10
#define ONE_ITEMSET_ARRAY_MAX 16500
#define OTH_ITEMSET_ARRAY_MAX 524287
#define ITEMSET_MAX_SIZE 10
#define SUPPORT_THRESHOLD 5

typedef struct itemsets * Itemsets;
typedef struct itemsetPtr * ItemsetPtr;
typedef struct c_itemsets * C_ItemsetPtr;

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

int * candidate_frequencies1;
int * frequent_frequencies1;
ItemsetPtr candidate_frequencies_prev;
ItemsetPtr frequent_frequencies_prev;
ItemsetPtr candidate_frequencies_cur;
ItemsetPtr frequent_frequencies_cur;

//void scandb(int, int *,Itemsets, Itemsets);
void scandb(int n, int * c_freq1, int * f_freq1, ItemsetPtr c_freq_cur, ItemsetPtr f_freq_cur, ItemsetPtr c_freq_prev, ItemsetPtr f_freq_prev);
void generate_candidate_one_itemsets(int *,int[],int);
void save_frequent_one_itemsets(FILE *f_itemsets,int * c_freq1,int,int);
//void get_frequent_one_itemsets(int * c_freq1,int * f_freq1);
void get_frequent_one_itemsets(int * c_freq1,int * f_freq1,int *distinct_itemsets_cnt, int *tot_itemsets_cnt);
Itemsets create_new_itemset_node(int itemset_cnt,int itemset_array[]);
void insert_candidate_itemset(int itemset_cnt,int hval,int itemset_array[],ItemsetPtr c_cur);
void apriori_generate_cand_2_itemsets(int * f_freq1, int itemset_cnt,ItemsetPtr c_freq_cur,int items[],C_ItemsetPtr c_itemset_ll,int item_cnt,ItemsetPtr c_prev);
//void r_apriori_generate_cand_2_itemsets(int * f_freq1, int slen,ItemsetPtr c_cur,int items[],int sub[],C_ItemsetPtr c_itemset_ll,int ilen,int istart,int sstart);
void apriori_generate_cand_n_itemsets(int * f_freq1, int itemset_cnt,ItemsetPtr c_freq_cur,int items[],C_ItemsetPtr c_itemset_ll,int item_cnt,ItemsetPtr c_prev);
void r_apriori_generate_cand_n_itemsets(int slen,ItemsetPtr c_cur,int items[],int sub[],C_ItemsetPtr c_itemset_ll,int ilen,int istart,int sstart,ItemsetPtr c_prev);
void get_frequent_n_itemsets(int, ItemsetPtr c_cur,ItemsetPtr f_cur, int *, int *);
void save_frequent_n_itemsets(int, FILE *f_itemsets,ItemsetPtr f_cur,int,int);
void release_memory(ItemsetPtr freq);
void generate_candidates_prev(int itemsetcnt,ItemsetPtr c_prev,ItemsetPtr f_cur);
int hashval(int itemset[],int len);


#endif
