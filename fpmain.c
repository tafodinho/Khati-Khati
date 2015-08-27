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
	
	/** Read data to be mined from file */
	input_dataset();
	
	/** Reorder and prune input data according to frequency of single attr */	
	order_input_data();
	recast_data_prune_unsupported(); 
	gen_freq_one_itemsets();/** Prints freq-1 itemsets */


    /** Build initial FP-tree */
	create_fptree(fptree);
	out_fptree_storage(fptree->root);
	out_fptree(fptree);

	/** Mine FP-tree */
	start_mining(fptree);/** frequent sets are generated and stored to file here. */
	out_fptree_storage(fptree->root);

	return 0;
}
