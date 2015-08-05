/**
 *Filename: apriori.c
 *Purpose: Defines the functions for the classical apriori algorithm as used
 */
#include "apriori.h"



/*
 * scandb(): Scans the Transaction database and storing the frequent item sets to an array.
 */
void scandb(int n, int * c_freq1, int * f_freq1, ItemsetPtr c_cur, ItemsetPtr f_cur, ItemsetPtr c_prev, ItemsetPtr f_prev) {

    char freq_file[20];
    FILE *trans, *f_itemsets;
    char basket[BASKET_MAX_CHARS];
    char item[ITEM_MAX];
    int items[BASKET_MAX_ITEMS];
    int i,j,k,item_no;
    char *p;
    char str[15];
    int distinct_itemsets_cnt;
    int tot_itemsets_cnt;
    C_ItemsetPtr c_itemset_ll;



    strcpy(freq_file,"../freq_j_");
    sprintf(str, "%d", n);
    strcat(freq_file, str);
    strcat(freq_file, "_itemsets");

    trans = fopen(TRANS_DB,"r");
    if( trans == NULL) {
	fprintf(stderr,"\n Error opening file: %s", TRANS_DB);
	exit(EXIT_SUCCESS);
    }

    f_itemsets = fopen(freq_file,"w");
    if (f_itemsets == NULL) {
	fprintf(stderr,"\n Error opening file: %s", TRANS_DB);
	exit(EXIT_SUCCESS);
    }

    /* read one line at a time from the file */
    while (fgets(basket, BASKET_MAX_CHARS, trans) != NULL) {
        
        for (i=0;i<BASKET_MAX_ITEMS;i++)
            items[i] = -1;

        item_no=0;
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

            items[item_no] = atoi(item);
            item_no++;
        }


        /* We have now read a line from database and extracted integers into array items */
        if (n == 1) { //process for 1-itemsets
	    
            generate_candidate_one_itemsets(c_freq1,items,item_no);
	    get_frequent_one_itemsets(c_freq1,f_freq1,&distinct_itemsets_cnt, &tot_itemsets_cnt);
            save_frequent_one_itemsets(f_itemsets,f_freq1,distinct_itemsets_cnt, tot_itemsets_cnt);
	    //free(c_freq1); //release memory held by candidate-1 itemsets
        }
	
	
        if (n == 2) { /* process for 2-itemsets*/
            apriori_generate_cand_2_itemsets(f_freq1, n,c_cur,items,c_itemset_ll,item_no,c_prev, f_cur, f_itemsets,&distinct_itemsets_cnt, &tot_itemsets_cnt);
	    release_memory(c_prev);
            release_memory(c_cur);
            
        }
        if (n > 2) { /* process for itemsets > 2 */
            apriori_generate_cand_n_itemsets(f_cur, n,c_cur,items,c_itemset_ll,item_no,c_prev, f_cur, f_itemsets,&distinct_itemsets_cnt, &tot_itemsets_cnt);
	    get_frequent_n_itemsets(n,c_cur,f_cur,&distinct_itemsets_cnt, &tot_itemsets_cnt);
            save_frequent_n_itemsets(n,f_itemsets,f_cur,distinct_itemsets_cnt,tot_itemsets_cnt);
	    release_memory(c_prev);
            release_memory(c_cur);
            
        }
    }

}





/**
 * Joins Li-1 to Li-1 to get current candidate item sets. 
 * 
 */
void join_frequent_n_itemsets(ItemsetPtr f_cur, ItemsetPtr f_prev, int i_cnt) {

    //To work on this more..
    int i, j, k;
    Itemsets tmp, tmp2, c_tmp;

	
    for (i = 0; i < OTH_ITEMSET_ARRAY_MAX; i++) {
        c_tmp = f_cur[i].itemset_ptr;
        tmp = f_cur[i].itemset_ptr;
	tmp2 = f_cur[i + 1].itemset_ptr;

        while (tmp != NULL) {
	    
	    if( tmp != tmp2) {	
	        c_tmp = calloc(1, sizeof(struct itemsets));
                c_tmp->cnt= tmp->cnt + tmp2->cnt;
	        c_tmp->next = NULL;
	        c_tmp->itemsets = calloc(c_tmp->cnt, sizeof(int));
	        j = 0, k = 0;
	  
	        //adds first frequent item set.
	        for (k = 0; k < tmp->cnt; tmp++)
	             c_tmp->itemsets[j++] = tmp->itemsets[k];
         
                //adds second item set.
	        for( k = 0; k < tmp2->cnt; k++)
	            c_tmp->itemsets[j++] = tmp->itemsets[k];
               
            }


	   tmp = tmp->next;
	   tmp2 = tmp2->next;

       }// end while
              
    }
}


void release_memory(ItemsetPtr freq){
    int i,j;
    Itemsets tmp1,tmp2;

    for (i=0;i<OTH_ITEMSET_ARRAY_MAX;i++) {
        tmp1 = freq[i].itemset_ptr;
        while (tmp1 != NULL) {
            tmp2 = tmp1 -> next;
            free(tmp1->itemsets);
            free(tmp1);
            tmp1 = tmp2;
        }
        freq[i].distinct_itemsets = 0;
        freq[i].itemsets_cnt = 0;
        freq[i].itemset_ptr = NULL;
    }
}


/**
 * Generates the frequent n itemsets by pruning out elements below the support count
 * Pruning Step.
 */
void get_frequent_n_itemsets(int itemsetcnt,ItemsetPtr c_cur,ItemsetPtr f_cur,int *distinct_itemsets_cnt, int *tot_itemsets_cnt){
    int i;
    Itemsets its,c_its_tmp,f_its_tmp,f_its_tmp2;
    for (i=0;i<OTH_ITEMSET_ARRAY_MAX;i++) {
        f_cur[i].distinct_itemsets = f_cur[i].itemsets_cnt = 0;
        f_its_tmp = f_cur[i] . itemset_ptr;
        c_its_tmp = c_cur[i] . itemset_ptr;

        while (c_its_tmp != NULL) {
            //is the item frequent
            if (c_its_tmp -> cnt >= SUPPORT_THRESHOLD) {
                f_cur[i].distinct_itemsets++;
                f_cur[i].itemsets_cnt += c_its_tmp -> cnt;
                printf("%s %d %d: %d %d\n","&&&&&&&&",i,c_its_tmp -> cnt,c_its_tmp->itemsets[0],c_its_tmp->itemsets[1]);getchar();
                its = create_new_itemset_node(itemsetcnt,c_its_tmp-> itemsets);
                its -> cnt = c_its_tmp -> cnt;
                //*its = *c_its_tmp;
                if (f_its_tmp == NULL)
                    f_its_tmp = f_cur[i] . itemset_ptr = its;
                else {
                    f_its_tmp -> next = its;
                    f_its_tmp = f_its_tmp -> next;//printf("+++\n");//getchar();
                }
            }
            c_its_tmp = c_its_tmp-> next;
        }
    }



    //compute global n-itemset counts
    *distinct_itemsets_cnt = 0;
    *tot_itemsets_cnt = 0;
    for (i=0;i<OTH_ITEMSET_ARRAY_MAX;i++) {
        *distinct_itemsets_cnt += f_cur[i].distinct_itemsets;
        *tot_itemsets_cnt += f_cur[i].itemsets_cnt;
    }
}


void save_frequent_one_itemsets(FILE *f_itemsets,int * f_freq1,int distinct_itemsets_cnt,int tot_itemsets_cnt) {
    int i;
    fprintf(f_itemsets, "%d %d\n", distinct_itemsets_cnt,tot_itemsets_cnt);
    for (i=0;i<ONE_ITEMSET_ARRAY_MAX;i++) {
        if (f_freq1[i] >= SUPPORT_THRESHOLD){
            fprintf(f_itemsets, "%d;%d\n", i,f_freq1[i]);
        }
    }
    fflush(f_itemsets);
}
void save_frequent_n_itemsets(int itemsetcnt,FILE *f_itemsets,ItemsetPtr f_cur,int distinct_itemsets_cnt,int tot_itemsets_cnt){
    int i,j;
    Itemsets tmp3,f_its_tmp;
    //store basic statistics for this hash
    fprintf(f_itemsets, "%d %d\n", distinct_itemsets_cnt,tot_itemsets_cnt);

    for (i=0;i<OTH_ITEMSET_ARRAY_MAX;i++) {
        f_its_tmp = f_cur[i] . itemset_ptr;
        while (f_its_tmp != NULL) {
            //store itemset
            for (j=0;j<itemsetcnt-1;j++) {
                fprintf(f_itemsets, "%d ",f_its_tmp->itemsets[j]);
            }
            fprintf(f_itemsets, "%d;",f_its_tmp->itemsets[j]);  //last item ends with ;
            //store itemset count
            fprintf(f_itemsets, "%d\n",f_its_tmp->cnt);
            f_its_tmp = f_its_tmp-> next;
        }
    }fflush(f_itemsets);
}


//generate_candidate_one_itemsets for the line read
void generate_candidate_one_itemsets(int * c_freq1,int items[],int max_size) {
    int i;
    for (i=0;i<max_size;i++) {
        c_freq1[items[i]]++;  //update frequencies of candidate 1-itemsets
    }
}

void get_frequent_one_itemsets(int * c_freq1,int * f_freq1,int *distinct_itemsets_cnt, int *tot_itemsets_cnt) {
    int i;
    *distinct_itemsets_cnt = 0;
    *tot_itemsets_cnt = 0;
    for (i=0;i<ONE_ITEMSET_ARRAY_MAX;i++) {
        if (c_freq1[i] >= SUPPORT_THRESHOLD) {
            f_freq1[i]=c_freq1[i];
            *distinct_itemsets_cnt +=1;
            *tot_itemsets_cnt += c_freq1[i];
        }
    }
}


/**
 * Applies a Join to the previous frequent item sets
 * generate_candidate_2_itemsets for the line read
 * 
 */
void apriori_generate_cand_2_itemsets(int * f_freq1, int itemset_cnt,ItemsetPtr c_cur,int items[],C_ItemsetPtr c_itemset_ll,int basket_cnt,ItemsetPtr c_prev, ItemsetPtr f_cur, FILE *f_itemsets, int *distinct_itemsets_cnt, int *tot_itemsets_cnt) {

    int i,j,k,h,hval;
    int itemset[itemset_cnt]; 
    release_memory(f_cur);//deallocated 
   
    Itemsets tmp; //stores newly generated candidate list

    //joins previous frequent item sets and 
    for (k = 0; k < basket_cnt - 1;k++) {
	
   	 for (j = k + 1; j < basket_cnt; j++) {
            itemset[0] = items[j];
            itemset[1] = items[k];

            //now compute hash value and store join to candidate list
            hval = hashval(itemset,2);
            insert_candidate_itemset(itemset_cnt,hval,itemset, c_cur);
	
	    //Pruning Step
	    tmp = create_new_itemset_node(2,itemset);		
	    if ( is_frequent_subset( c_cur,itemset, itemset_cnt)) {
		hval = hashval(itemset,2);
		insert_candidate_itemset(itemset_cnt, hval, itemset, f_cur);
	    }
	      //printf("%s %d %d\n","&&&&",itemset[0],itemset[1]);
        }

    }//end for loop

    //Saving the frequent itemsets.
    fprintf(f_itemsets, "%d %d\n", distinct_itemsets_cnt,tot_itemsets_cnt);
     for (i = 0;i < OTH_ITEMSET_ARRAY_MAX; i++) {
        tmp = f_cur[i] . itemset_ptr;
        while (tmp != NULL) {

            //store itemset
            for (j = 0; j < itemset_cnt - 1; j++) {
                fprintf(f_itemsets, "%d ",tmp->itemsets[j]);
            }

            fprintf(f_itemsets, "%d;",tmp->itemsets[j]);  //last item ends with ;

            //store itemset count
            fprintf(f_itemsets, "%d\n",tmp->cnt);
            tmp = tmp-> next;
        }
    }
    
}


/**
 *To be modified to support for n _itemsets.
 */
//generate_candidate_n_itemsets for the line read
void apriori_generate_cand_n_itemsets(ItemsetPtr f_freq1, int itemset_cnt,ItemsetPtr c_cur,int items[],C_ItemsetPtr c_itemset_ll,int basket_cnt,ItemsetPtr c_prev, ItemsetPtr f_cur, FILE *f_itemsets, int *distinct_itemsets_cnt, int *tot_itemsets_cnt) {
    int * sub = calloc(itemset_cnt,sizeof(int));    //we want subsequences of size itemset_cnt
    int hval, k;

    //Joins the prev frequent Items sets,  without applying apriori property
    join_frequent_n_itemsets(c_cur, f_freq1, itemset_cnt);

    
}

void r_apriori_generate_cand_n_itemsets(int slen,ItemsetPtr c_cur,int items[],int sub[],C_ItemsetPtr c_itemset_ll,int ilen,int istart,int sstart,ItemsetPtr c_prev) {
    int i,j,k,h,hval;
    int * ret_itemset;


    if (sstart == slen){//sub now contains an n-item subset; apply apriori to determine if candidate itemset
        ret_itemset = sub;
        if (infrequent_subset_found(slen-1,sub, slen, c_prev)) {
            return;
        }
        hval = hashval(sub,slen);
        insert_candidate_itemset(slen,hval,sub, c_cur);
    }
    else if (istart < ilen) {
        sub[sstart] = items[istart];
        r_apriori_generate_cand_n_itemsets(slen,c_cur,items,sub,c_itemset_ll,ilen,istart+1,sstart+1,c_prev);
        r_apriori_generate_cand_n_itemsets(slen,c_cur,items,sub,c_itemset_ll,ilen,istart+1,sstart,c_prev);
    }
}


int infrequent_subset_found(int subset_len,int items[],int basket_cnt,ItemsetPtr c_prev) {
    int * sub = calloc(subset_len,sizeof(int));    //we want subsequences of size itemset_cnt
    return r_infrequent_subset_found(subset_len,items,sub,basket_cnt,c_prev,0,0);
}

int r_infrequent_subset_found(int slen,int items[],int sub[],int ilen,ItemsetPtr c_prev,int istart,int sstart) {
    int i,j,k,h,hval;




    if (sstart == slen){//sub now contains an n-item subset; apply apriori to determine if candidate itemset
        //printf("££$$££$$££$$ %d",is_frequent_subset(c_prev,sub,slen));getchar();
        if (!is_frequent_subset(c_prev,sub,slen))
            return 1;
    }
    else if (istart < ilen) {
        sub[sstart] = items[istart];
        r_infrequent_subset_found(slen,items,sub,ilen,c_prev,istart+1,sstart+1);
        r_infrequent_subset_found(slen,items,sub,ilen,c_prev,istart+1,sstart);
    }
    return 0;
}


int hashval(int itemset[],int len) {
    int i,hval=0;
    for (i=0; i< len; i++)
        hval += itemset[i];
    hval = hval % OTH_ITEMSET_ARRAY_MAX;
    return hval;
}


int is_frequent_subset(ItemsetPtr c_prev, int sub[], int slen) {
    int i,j,found,hval;
    Itemsets tmp;

    //applies apriori to all n-1 subsequences of sub
    hval = hashval(sub,slen);
    tmp = c_prev[hval].itemset_ptr;
    found = 0;
    while (tmp != NULL) {
        for (i=0; i<slen; i++) {
            if (tmp->itemsets[i] != sub[i]) {
                break;
            }
            //we have found a frequent subset
            found = 1;
        }
        if (found)
            return 1;
        tmp = tmp -> next;
    }
    return 0;
}


Itemsets create_new_itemset_node(int itemset_cnt,int itemsets[]) {
    Itemsets its;
    int i;
    its = calloc(1,sizeof(struct itemsets));
    its -> cnt = 1;
    its -> next = NULL;
    its -> itemsets = calloc(itemset_cnt, sizeof(int));
    for (i=0;i<itemset_cnt;i++)
        its->itemsets[i] = itemsets[i];

    return its;
}


void insert_candidate_itemset(int itemset_cnt,int hval,int itemsets[],ItemsetPtr c_cur) {
    int i,itemset_processed;
    Itemsets its, tmp,tmp2;
    Itemsets tmp3;

    if (c_cur[hval].itemset_ptr == NULL) {
        c_cur[hval].itemsets_cnt = 1;
        c_cur[hval].distinct_itemsets = 1;
        c_cur[hval].itemset_ptr = create_new_itemset_node(itemset_cnt, itemsets);
        //printf("%s %d %d %d\n","£££££££",hval,c_cur[hval].itemset_ptr -> itemsets[0],c_cur[hval].itemset_ptr -> itemsets[1]);
    }
    else {

        //find out if itemset already on linked list
        tmp=tmp2=c_cur[hval].itemset_ptr;
        itemset_processed = 0;
        while (tmp != NULL) {
            for (i=0;i<itemset_cnt;i++) {
                if (itemsets[i] < tmp->itemsets[i]) {
                    //insert new item before current node
                    c_cur[hval].distinct_itemsets++;
                    c_cur[hval].itemsets_cnt++;
                    if (tmp == tmp2) { //current node is first node
                        c_cur[hval].itemset_ptr = create_new_itemset_node(itemset_cnt, itemsets);
                        tmp2 = c_cur[hval].itemset_ptr;
                        tmp2 -> next = tmp;
                        itemset_processed = 1;
                        break;
                    }
                    else { //current node is not first node
                        tmp2 -> next = create_new_itemset_node(itemset_cnt, itemsets);
                        tmp2 = tmp2 -> next;
                        tmp2 -> next = tmp;
                        itemset_processed = 1;
                        break;
                    }
                }
                else if (itemsets[i] > tmp->itemsets[i]) {
                    //try next node
                    itemset_processed = 0;
                    break;
                }
            }

            //any unprocessed itemset here is either on list or greater than current node
            if (i == itemset_cnt) {  //itemset on list
                //process
                tmp -> cnt++;
                itemset_processed = 1;
                break;
            }
            if (itemset_processed == 0) {
                if (tmp == tmp2)
                    tmp = tmp -> next;
                else {
                    tmp2 = tmp2 -> next;
                    tmp = tmp->next;
                }
                //are we past the last node
                if (tmp == NULL) {
                    c_cur[hval].distinct_itemsets++;
                    c_cur[hval].itemsets_cnt++;
                    tmp2->next = create_new_itemset_node(itemset_cnt, itemsets);
                }
            }
            else
                break;
        }
    }
}
