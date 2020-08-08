/*
 * 		cpu_heap.c
 *
 * 		Student: Itai Antebi
 * 		ID: 204817498
 *
 *     This module implements functions that performs various operations at a single CPU heap level
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory_allocator.h"

/* remove a superblock from a given heap and sizeclass index and update heap level stats */

void removeSuperblockFromHeap(cpuheap_t *heap, int sizeClass_ix, superblock_t *pSb){
	//printf("removeSuperblockFromHeap\n");
	int nSBlk;
	superblock_t * currSBlk = heap->_sizeClasses[sizeClass_ix]._SBlkList._first;
	for(nSBlk = 0; nSBlk < heap->_sizeClasses[sizeClass_ix]._SBlkList._length; nSBlk++){
		if(currSBlk == pSb){
			heap->_sizeClasses[sizeClass_ix]._SBlkList._first = pSb->_meta._pNxtSBlk;
		}
		currSBlk = currSBlk->_meta._pNxtSBlk;
	}
	// heap statistics
	heap->_sizeClasses[sizeClass_ix]._SBlkList._length -= 1;
	heap->_bytesAvailable -= SUPERBLOCK_SIZE;
	heap->_bytesUsed -= (pSb->_meta._NoBlks - pSb->_meta._NoFreeBlks) * pSb->_meta._sizeClassBytes;
	return;
}

/* add a superblock to a given heap and sizeclass index and update heap level stats */
void addSuperblockToHeap(cpuheap_t *heap, int sizeClass_ix, superblock_t *pSb){
	//printf("addSuperblockToHeap\n");
	int nSBlk;
	// there are no SBs in sizeClass
	if(heap->_sizeClasses[sizeClass_ix]._SBlkList._length == 0){
		heap->_sizeClasses[sizeClass_ix]._SBlkList._first = pSb;
	}
	else{
		superblock_t * currSBlk = heap->_sizeClasses[sizeClass_ix]._SBlkList._first;
		// there is more than one SB in sizeClass
		if(heap->_sizeClasses[sizeClass_ix]._SBlkList._length > 1){
			for(nSBlk = 0; nSBlk < heap->_sizeClasses[sizeClass_ix]._SBlkList._length; nSBlk++){
				currSBlk = currSBlk->_meta._pNxtSBlk;
			}
		}
		currSBlk->_meta._pNxtSBlk = pSb;
		pSb->_meta._pPrvSblk = currSBlk;
	}
	pSb->_meta._pOwnerHeap = heap;
	// heap statistics
	heap->_sizeClasses[sizeClass_ix]._SBlkList._length += 1;
	heap->_bytesAvailable += SUPERBLOCK_SIZE;
	heap->_bytesUsed += (pSb->_meta._NoBlks - pSb->_meta._NoFreeBlks) * pSb->_meta._sizeClassBytes;
}


void *allocateBlockFromCurrentHeap( superblock_t *pSb){
	//printf("allocateBlockFromCurrentHeap\n");
	void * pMemToReturn;
	block_header_t *pBlock = pSb->_meta._pFreeBlkStack;
	pSb->_meta._pFreeBlkStack = pSb->_meta._pFreeBlkStack->_pNextBlk;
	pSb->_meta._NoFreeBlks -=1;
	pBlock->_pOwner = pSb;
	// return memory that comes after the header
	pMemToReturn = pBlock;
	pMemToReturn += sizeof(block_header_t);
	pSb->_meta._pOwnerHeap->_bytesUsed += pSb->_meta._sizeClassBytes;
	return pMemToReturn;
}

void freeBlockFromCurrentHeap( block_header_t *pBlock) {
	//printf("freeBlockFromCurrentHeap\n");
	superblock_t* SBlkOwner = pBlock->_pOwner;
	// OR MAYBE ->_pNextBlk?......................................................................................................................//
	pBlock->_pNextBlk = SBlkOwner->_meta._pFreeBlkStack;
	SBlkOwner->_meta._pFreeBlkStack = pBlock;
	SBlkOwner->_meta._NoFreeBlks +=1;
	SBlkOwner->_meta._pOwnerHeap->_bytesUsed -= SBlkOwner->_meta._sizeClassBytes;
}


//V
/* this is a boolean function to check the condition
 * to transfer superblocks to general heap
 */
char isHeapUnderUtilized(cpuheap_t *pHeap){
	//printf("isHeapUnderUtilized\n");
	// (u(i) < (1−f) * a(i))  &  (u(i) < a(i) − K∗S)
	return ((pHeap->_bytesUsed < ((1 - HOARD_EMPTY_FRACTION) * pHeap->_bytesAvailable)) &&
			(pHeap->_bytesUsed < (pHeap->_bytesAvailable - (HOARD_K * SUPERBLOCK_SIZE))));
}




superblock_t *findMostlyEmptySuperblock(cpuheap_t *pHeap){
	//printf("findMostlyEmptySuperblock\n");
	int nSizeClass = 0;
	int currSBlkIndex;
	double empty_ration;
	superblock_t * currSBlk;
	for (nSizeClass = 0; nSizeClass < NUMBER_OF_SIZE_CLASSES; nSizeClass ++){
		currSBlk = pHeap->_sizeClasses[nSizeClass]._SBlkList._first;
		for (currSBlkIndex = 0; currSBlkIndex < pHeap->_sizeClasses[nSizeClass]._SBlkList._length; currSBlkIndex ++){
			empty_ration = currSBlk->_meta._NoFreeBlks / currSBlk->_meta._NoBlks;
			// I AM NOT SURE ABOUT THIS...........................................................................................................//
			if (empty_ration < HOARD_EMPTY_FRACTION){
				return currSBlk;
			}
			currSBlk = currSBlk->_meta._pNxtSBlk;
		}
	}
	return NULL;
}
