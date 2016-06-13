#include "sys.h"
#include "disk.h"
#include "inode.h"

#define SUPER_BLOCK_SIZE 24 //Super Block size in bytes


typedef struct SuperBlock {
	uint32_t i_count ; 		// Inodes Count
	uint32_t b_count ; 		// Blocks Count
	uint32_t f_block ; 		// Number of free blocks
	uint32_t f_inode ; 		// Number of free inodes 
	uint32_t b_size ; 		// Block size
	uint32_t first_data_block ; 	// Number of the first block of the data section
} T_SpBlk ;				// Total size 24B

void sBk_creat(uint32_t bSize, uint32_t nb_inode) ;	/* create a new super block in dest
			  				   bSize is Block size in Byte
			   				   nb_inode is the number of inode to create 
						       */
int sBk_load() ;  //Load the super block of dsk in dest Return 0 if sucess -1 else
int sBk_calc_iBitmap() ; //return the inode bitmap size in Byte
int sBk_calc_bBitmap() ; //return the block bitmap size in Byte

extern T_SpBlk G_super_block ;
