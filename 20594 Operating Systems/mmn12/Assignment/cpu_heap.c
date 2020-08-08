/*
 *     This module implements functions that performs various operations at a single CPU heap level
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory_allocator.h"

/* remove a superblock from a given heap and sizeclass index and update heap level stats */
void removeSuperblockFromHeap(cpuheap_t *heap, int sizeClass_ix, superblock_t *pSb){



}

/* add a superblock to a given heap and sizeclass index and update heap level stats */
void addSuperblockToHeap(cpuheap_t *heap, int sizeClass_ix, superblock_t *pSb){


}

void *allocateBlockFromCurrentHeap( superblock_t *pSb){

	return NULL; // TODO replace to actual memory region

}

void freeBlockFromCurrentHeap( block_header_t *pBlock) {


}

/* this is a boolean function to check the condition
 * to transfer superblocks to general heap
 */
char isHeapUnderUtilized(cpuheap_t *pHeap){


	return 0; // TODO return the result of the utilization condition evaluation
}



superblock_t *findMostlyEmptySuperblock(cpuheap_t *pHeap){


	return NULL;
}
