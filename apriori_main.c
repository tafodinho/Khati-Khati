/*
 * File:   apriori_main.c
 *
 */
#include "apriori.h"


/*
 * Main(): Executes apriori algorithm.
 */
int main(int argc, char** argv) {
    int i,j;
    Itemsets tmp1,tmp2;
    candidate_frequencies_prev = calloc(OTH_ITEMSET_ARRAY_MAX, sizeof(struct itemsetPtr));

    for (j = 0;j < OTH_ITEMSET_ARRAY_MAX; j++)
        candidate_frequencies_prev[j].itemset_ptr = NULL;

    for (i = 1;i < ITEMSET_MAX_SIZE; i++) {
        printf("Generating Frequent %d itemsets\n",i);
        if (i == 1) {
            candidate_frequencies1 = calloc(ONE_ITEMSET_ARRAY_MAX, sizeof(int));   //1-itemset frequencies
            frequent_frequencies1 = calloc(ONE_ITEMSET_ARRAY_MAX, sizeof(int ));   //1-itemset frequencies
        } else if (i==2) {
            candidate_frequencies_cur = calloc(OTH_ITEMSET_ARRAY_MAX, sizeof(struct itemsetPtr));
            frequent_frequencies_cur = calloc(OTH_ITEMSET_ARRAY_MAX,  sizeof(struct itemsetPtr));
            for (j=0;j <OTH_ITEMSET_ARRAY_MAX;j++) {
                candidate_frequencies_cur[j].itemset_ptr = NULL;
                frequent_frequencies_cur[j].itemset_ptr = NULL;
            }
        }
        else {
            //deleted code here...
            candidate_frequencies_cur = calloc(OTH_ITEMSET_ARRAY_MAX, sizeof(struct itemsetPtr));
            frequent_frequencies_cur = calloc(OTH_ITEMSET_ARRAY_MAX,  sizeof(struct itemsetPtr));
            for (j=0;j<OTH_ITEMSET_ARRAY_MAX;j++) {
                candidate_frequencies_cur[j].itemset_ptr = NULL;
                frequent_frequencies_cur[j].itemset_ptr = NULL;
            }
        }
        scandb(i,candidate_frequencies1,frequent_frequencies1,candidate_frequencies_cur,frequent_frequencies_cur,
               candidate_frequencies_prev,frequent_frequencies_prev);
    }

     release_memory(frequent_frequencies_cur);
     free(frequent_frequencies1);
}

