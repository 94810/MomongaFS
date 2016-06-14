#include "superBlock.h"

T_SpBlk G_super_block;

void sBk_creat(uint32_t bSize, uint32_t nb_inode){
	
	G_super_block.b_size = bSize ;
	G_super_block.i_count = nb_inode ;
	G_super_block.b_count = G_vdisk.size / G_super_block.b_size ;
	G_super_block.f_inode = nb_inode;
	G_super_block.first_data_block = ( sizeof(T_SpBlk) + sBk_calc_iBitmap() + sBk_calc_bBitmap() + sizeof(T_inode)*G_super_block.i_count )/ 8 + 1;
	G_super_block.f_block=G_super_block.b_count - G_super_block.first_data_block ;
}

int sBk_calc_iBitmap(){
	return G_super_block.i_count/8 + 1 ;
}

int sBk_calc_bBitmap(){
	return G_super_block.b_count/8 + 1 ;
}

int sBk_load(){
	int res=0;
	
	disk_seek(0);
	res = res + disk_read(&(G_super_block.i_count), sizeof(uint32_t));
	res = res + disk_read(&(G_super_block.b_count), sizeof(uint32_t));
	res = res + disk_read(&(G_super_block.f_block), sizeof(uint32_t));
	res = res + disk_read(&(G_super_block.f_inode), sizeof(uint32_t));
	res = res + disk_read(&(G_super_block.b_size), sizeof(uint32_t));
	res = res + disk_read(&(G_super_block.first_data_block), sizeof(uint32_t));

	if(res!=sizeof(T_SpBlk))
		res=-1;
	else
		res=0;
	
	return res;
}

int sBk_write(){
	int res=0;
	
	disk_seek(0);
	res = res + disk_write(&(G_super_block.i_count), sizeof(uint32_t));
	res = res + disk_write(&(G_super_block.b_count), sizeof(uint32_t));
	res = res + disk_write(&(G_super_block.f_block), sizeof(uint32_t));
	res = res + disk_write(&(G_super_block.f_inode), sizeof(uint32_t));
	res = res + disk_write(&(G_super_block.b_size), sizeof(uint32_t));
	res = res + disk_write(&(G_super_block.first_data_block), sizeof(uint32_t));

	if(res!=sizeof(T_SpBlk))
		res=-1;
	else
		res=0;
	
	return res;
}
