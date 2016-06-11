#include "sys.h"
#include "disk.h"

#define SUPER_BLOCK_SIZE 24 //Super Block size in bytes


typedef struct SuperBlock {
	uint32_t i_count ; 		// Inodes Count
	uint32_t b_count ; 		// Blocks Count
	uint32_t f_block ; 		// Number of free blocks
	uint32_t f_inode ; 		// Number of free inodes 
	uint32_t b_size ; 		// Block size
	uint32_t first_data_block; 	// Number of the first block of the data section
} T_SpBlk;				// Total size 24B

int create_sBk(uint32_t bSize, uint32_t nb_inode, unsigned int disk_size, T_SpBlk* desc);	/* create a new super block in desc
								  				   bSize is Block size in Byte
								   				   nb_inode is the number of inode to create
												   disk_size is in Bytes
												   Return 0 if sucess -1 else
						 	        				*/
int load_sBk(T_SpBlk* desc); 		//Load the super block in desc Return 0 if sucess -1 else

