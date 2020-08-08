/*
 * 		size_class.c
 *
 * 		Student: Itai Antebi
 * 		ID: 204817498
 *
 *
 *      This module implements functions to perform various operations at size class level
 *      such as insertions, removal and searches for superblocks from a given sizeclass
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "memory_allocator.h"




/*
 * insert a new superblock into the list
 *
 */
void insertSuperBlock(size_class_t *sizeClass, superblock_t *superBlock) {
	return;
}

/* find available superblock */

superblock_t *findAvailableSuperblock(size_class_t *sizeClass) {
	//printf("findAvailableSuperblock\n");
	superblock_t * currSBlk;
	int nSBlk = 0;
	// no SB's in sizeClass
	currSBlk = sizeClass->_SBlkList._first;
	// Iterate over SuperBlocks in the sizeClass; Look for one with free Blocks
	for(nSBlk = 0; nSBlk < sizeClass->_SBlkList._length; nSBlk++){
		if(currSBlk->_meta._NoFreeBlks > 0){
			return currSBlk;
		}
		currSBlk = currSBlk->_meta._pNxtSBlk;
	}
	// No free Blocks were found in any SuperBlock
	return NULL;
}

void printSizeClass(size_class_t *sizeClass){
	int i;
	superblock_t *p=sizeClass->_SBlkList._first;
	printf("SizeClass [%d] # superblocks [%d]\n",sizeClass->_sizeClassBytes, sizeClass->_SBlkList._length);

	for(i=0;i< sizeClass->_SBlkList._length; i++, p=p->_meta._pNxtSBlk){
		printf("\n %d)  ",i);
		printSuperblock(p);
	}


}




size_t getSizeClassIndex(size_t size){
	double l=log(size)/log(2);
	return ceil(l);

}

size_class_t *getSizeClassForSuperblock(superblock_t *pSb){

	size_t i=getSizeClassIndex(pSb->_meta._sizeClassBytes);
	return &(pSb->_meta._pOwnerHeap->_sizeClasses[i]);
}
