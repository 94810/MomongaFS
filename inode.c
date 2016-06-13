#include "inode.h"
#include "superBlock.h"

int inode_load(T_inode* dest, uint32_t nb){
	
	int res=0;
	
	disk_seek(SUPER_BLOCK_SIZE+(nb*INODE_SIZE));
	
	res = res + disk_read(&(dest->file_size), 4);
	res = res + disk_read(&(dest->file_mode), 1);
	res = res + disk_read(&(dest->d_block), 60);
	res = res + disk_read(&(dest->last_mod), 4);
	res = res + disk_read(&(dest->link_count), 1);

	if(res ! = INODE_SIZE)
		res=-1;
	else
		res=0;
	
	return res;
}

int inode_write(T_inode* src, uint32_t nb){
	
	int res=0;
	
	disk_seek(SUPER_BLOCK_SIZE+(nb*INODE_SIZE));
	
	res = res + disk_write(&(dest->file_size), 4);
	res = res + disk_write(&(dest->file_mode), 1);
	res = res + disk_write(&(dest->d_block), 60);
	res = res + disk_write(&(dest->last_mod), 4);
	res = res + disk_write(&(dest->link_count), 1);

	if(res ! = INODE_SIZE)
		res=-1;
	else
		res=0;
	
	return res;
}
