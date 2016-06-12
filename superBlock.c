#include "superBlock.h"

int create_sBk(uint32_t bSize, uint32_t nb_inode, unsigned int disk_size, T_SpBlk* dest){
	
	dest->b_size = bSize ;
	dest->i_count = nb_inode ;
	dest->b_count = disk_size / b_size ;
	dest->f_inode = nb_inode ;
	
	
	
	
	
}
