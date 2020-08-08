/*
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


}

/* find available superblock */

superblock_t *findAvailableSuperblock(size_class_t *sizeClass) {


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
