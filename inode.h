#include "sys.h"

#define INDIRECT_BLOCK 12
#define DOUBLE_INDIRECT_BLOCK 13
#define TRIPLE_INDIRECT_BLOCK 14
#define ROOT_DIRECTORY_INODE 0 //The inode of the root directory is the first Inode

typedef struct Inode {
	uint32_t file_size ;	 //	Size of the file in bytes
	uint8_t  file_mode ;	 /*
					Acess and file type ( normal or directories )
			  	    	MSB at 1 mean file is a directory, 0 mean it's a regular file
				  	Other bit are reserved for Acess gestion
				 */
	uint32_t d_block[15] ;	 /*
				 	0-11 : Direct data blocks
					12 : Fisrt indirect blocks
					13 : Double indirect blocks
					14 : Triple indirect blocks 
					
					A Zero value mean no further block
				 */
	uint32_t last_mod ;	 //	Last time file was modified
	uint8_t	 link_count ;	 //	Number of link on this file 
} T_inode;			 //	Total size 70B

int load_inode(T_inode* desc, uint32_t nb) ; // Load the nb inode in desc 
