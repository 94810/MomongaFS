#include "fs.h"

int mfs_close(T_File* file){

	file->mod = 0 ; //Can't read or write
	return inode_write(&(file->inode), file->inode_nb);

}

int mfs_block_seek(unsigned int block){
	return disk_seek(block*G_super_block.b_size);	
}

int mfs_file_seek(){
	return 0 ;
}
