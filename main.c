/*
 * File:   main.c
 * Author: root
 *
 * Created on April 17, 2015, 3:18 PM
 */

#include "apriori_joinless.h"


/*
 *
 */
int main(int argc, char** argv) {
    int i,j;
    Itemsets tmp1,tmp2;
    candidate_frequencies_prev = calloc(OTH_ITEMSET_ARRAY_MAX, sizeof(struct itemsetPtr));

    for (j=0;j<OTH_ITEMSET_ARRAY_MAX;j++)
        candidate_frequencies_prev[j].itemset_ptr = NULL;

    for (i=1;i<ITEMSET_MAX_SIZE;i++) {
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
/*Itemsets tmp;int j;
for (i=0;i<10;i++) {
    tmp = candidate_frequencies_prev[i].itemset_ptr;
    while (tmp != NULL) {
        for (j=0;j<2;j++)
            printf("%s %d %d  ","~#~#***********~#~#",i,tmp->itemsets[j]);
        printf("\n");getchar();
        tmp=tmp->next;
    }
    getchar();
}
*/

            //for (j=0;j<OTH_ITEMSET_ARRAY_MAX;j++)
              //  candidate_frequencies_prev[j].itemset_ptr = NULL;
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
}

