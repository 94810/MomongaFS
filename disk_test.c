#include "superBlock.h"
#include "disk.h"

void main(){
	
	if(disk_creat(500, "vdisk")==-1)
		printf("Disk exist !\n");

	disk_open("vdisk");

	sBk_creat(10, 5);
	
	printf("test : %d, %d, %d, %d, %d, %d\n", G_super_block.i_count, G_super_block.b_count, G_super_block.f_block, G_super_block.f_inode, G_super_block.b_size, G_super_block.first_data_block);

	sBk_write();

	sBk_load();
	
	printf("test : %d, %d, %d, %d, %d, %d\n", G_super_block.i_count, G_super_block.b_count, G_super_block.f_block, G_super_block.f_inode, G_super_block.b_size, G_super_block.first_data_block);
	
	disk_close();
}
