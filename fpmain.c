/**
 * File:   apriori_main.c
 * Author: Bawe Emmanuel
 *
 */
#include "fptree.h"


/*
 * Main(): Executes FP Growth algorithm.
 */
int main(int argc, char** argv) {

	/** Create instance of class FPTree */
	fptreePtr fptree;
	create_fptree(fptree);
	
	/** Read data to be mined from file */
	input_dataset(fptree);
	
	/** Reorder and prune input data according to frequency of single attr */	
	order_input_data(fptree);
	recast_data_prune_unsupported(fptree); 
	gen_freq_one_itemsets(fptree);
	//out_data_array();


    /** Build initial FP-tree */
	create_fptree(fptree);
	out_fptree_storage(fptree.root);			
	out_fptree(fptree.root);

	/** Mine FP-tree */
	start_mining();
	out_fptree_storage(fptree.root);
	out_num_freq_itemsets(fptree);
	/* outputTtree() Frequent sets arec stored in this structure */

	return 0;
}
