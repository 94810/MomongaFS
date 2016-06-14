#ifndef INODE
#define INODE
#include "sys.h"
#include "disk.h"

#define INDIRECT_BLOCK 12
#define DOUBLE_INDIRECT_BLOCK 13
#define TRIPLE_INDIRECT_BLOCK 14
#define ROOT_DIRECTORY_INODE 0 //The inode of the root directory is the first Inode
#define INODE_SIZE sizeof(T_inode)

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

int inode_load(T_inode* dest, uint32_t nb) ; // Load the nb inode in desc 

int inode_write(T_inode* src, uint32_t nb) ; // write the inode src to inode nb

int seek_to_Ibitmap() ; //Place disk cursor on first bytes of inode bimap

unsigned int first_free_bitmap() ; //Return the number of the first free bit in bitmap

int change_bit(int n) ; //Perform a not on the bit n of the under cursor byte
#endif
