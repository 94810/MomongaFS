#include "fs.h"

int mfs_close(T_File* file){

	file->mod = 0 ; //Can't read or write
	return inode_write(&(file->inode), file->inode_nb) ;

}

int mfs_block_seek(unsigned int block){
	return disk_seek(block*G_super_block.b_size) ;	
}

void mfs_file_seek(T_File* file, unsigned int byte){
	file->cursor_block = byte / G_super_block.b_size ;
	file->cursor_byte = byte % G_super_block.b_size ;
}

int mfs_read(T_File* file , void* buff, uint32_t byte){
	
	uint8_t* f_buff = buff;
	
	unsigned int nb_bitread = 0, to_read = 0, i=0 ;

	if(file->mod & READ == 0)
		return -1;

	if(file->cursor_block*G_super_block.b_size + file->cursor_byte + byte > file->inode.file_size)
		byte =  file->inode.file_size - file->cursor_block*G_super_block.b_size + file->cursor_byte ; 
	
	if(f_buff==NULL)
		return -1;

	
	disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte) ;
	
	if(byte > (8-file->cursor_byte)){
		to_read=8-file->cursor_byte ;
		disk_read(f_buff, to_read);
		nb_bitread=to_read;
		file->cursor_block++;
		file->cursor_byte=0;            //On se calle sur le prochain block

		to_read = byte-nb_bitread / 8 ; //On regarde le nombre de block a lire

		for(i=0 ; i<to_read ; i++){
			disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte);
			
			disk_read(f_buff+nb_bitread, G_super_block.b_size);

			file->cursor_block ++ ;
		
			nb_bitread+= G_super_block.b_size ;
		} //On lis les block

		if(byte-nb_bitread!=0){
			disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte);
			
			file->cursor_byte = disk_read(f_buff+nb_bitread, byte-nb_bitread) ;
	 		
			nb_bitread += file->cursor_byte;
		} //On lit le reste forcément < 1 block
		
	}else{
		nb_bitread += disk_read(buff, byte);
		
		file->cursor_byte+=byte ;

		if(file->cursor_byte > 7){
			file->cursor_block++;
			file->cursor_byte=0;
		}
	}		

	return nb_bitread ;
}


