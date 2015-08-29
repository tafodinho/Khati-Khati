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

	num_freq_sets = size = colcnt = num_oflines = num_updates = 0;

	/** Create instance of class FPTree */
	fptreePtr fptree;
	
	/** Read data to be mined from file */
	input_dataset();
	
	/** Reorder and prune input data according to frequency of single attr */	
	order_input_data();
	recast_data_prune_unsupported();
	printf("\nPrinting frequent-1 itemsets");
	gen_freq_one_itemsets();/** Prints freq-1 itemsets */


    /** Build initial FP-tree */
    printf("\nBuilding FP tree");
	create_fptree(fptree);/* skip invalid elements(-1) */
	//out_fptree_storage(fptree->root);
	out_fptree(fptree);

	/** Mine FP-tree */
	printf("\nMining FP tree");
	start_mining(fptree);/** frequent sets are generated and stored to file here. */
	printf("\nReleasing memory consumption of tree.");
	//out_fptree_storage(fptree->root);
	release_memory(fptree); /** Frees all used memory */

	return 0;
}
