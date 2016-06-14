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

int seek_to_Ibitmap(){
	return disk_seek(SUPER_BLOCK_SIZE+(INODE_SIZE*G_super_block.i_count));
}

int seek_to_Bbitmap(){
	return disk_seek(SUPER_BLOCK_SIZE+(INODE_SIZE*G_super_block.i_count)+sBk_calc_iBitmap);
}

unsigned int first_free_bitmap(){

	int byte_count=0, cont=1, i=0;
	uint8_t buff=0, mask;
	

	while(cont){
		mask=0b10000000; //Set the mask for testting the first bit of the byte
		disk_read(&buff, 1); //Read a byte
		for(i=0; i<8; i++){
			if((buff & mask)==0){
				/*
					Use of a bitwise AND. We test if the bit at 1 in mask is at 1
					in buff exemple :
						12345678
					mask   |00100000  
					buff   |11100101
					================= AND
					        00100000 != 0
					bit 3 is not 0, inode (byte_count*8)+3 is not free

						12345678
					mask   |01000000  
					buff   |10110101
					================= AND
					        00000000 == 0
					bit 2 is zero , inode (byte_count*8)+2 is free
				*/
				cont=0;
				break; //Out the for
			 }else
				mask = mask >> 1; //Shift to next one			
		}

		if(cont)
			byte_count++;
	}

	return (byte_count*8)+i
}

int change_bit(int n){
	
	
		
}
