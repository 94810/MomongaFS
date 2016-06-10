#include "sys.h"

#define SUPER_BLOCK_SIZE 24 //Super Block size in bytes


typedef struct SuperBlock {
	uint32_t i_count ; 		// Inodes Count
	uint32_t b_count ; 		// Blocks Count
	uint32_t f_block ; 		// Number of free blocks
	uint32_t f_inode ; 		// Number of free inodes 
	uint32_t b_size ; 		// Block size
	uint32_t first_data_block; 	// Number of the first block of the data section
} T_SpBlk;				// Total size 24B

int create_sBk(uint32_t bSize, uint32_t nb_inode);	/* create a new super block
								 bSize is Block size in Byte
								 nb_inode is the number of inode to create
						 	*/
int load_sBk(T_SpBlk* desc); //Load the super block in desc

