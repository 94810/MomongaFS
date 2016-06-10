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

int create_sBk(); //create a new super block
int load_sBk(); //Load the super block

