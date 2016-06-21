#include "superBlock.h"
#include "disk.h"
#include "fs.h" 

void main(){

	T_inode inode;

	T_File file;	
	
	if(disk_creat(51200, "vdisk")==-1)
		printf("Disk exist !\n");

	disk_open("vdisk");

	sBk_creat(1024, 10);
	
	sBk_write();

	sBk_load();

	file.inode_nb = mfs_creat("/");		

	inode_load(&inode, file.inode_nb);
		
	file.inode = inode ;
	file.cursor_block= 0;
	file.cursor_byte = 0;
	file.mod = READ|WRITE ;

	seek_to_Bbitmap();
	change_bit(0);	
	seek_to_Ibitmap();
	change_bit(file.inode_nb);	

//	mfs_open("/", READ|WRITE, &file);

	seek_to_Bbitmap();
	printf("main : %d", first_free_bitmap());
		
	mfs_reload(&file, 0);
	
	char lel[6]={"HELLO"};

	mfs_write(&file, &lel, 6);
	
	char lal[6]={"WORLD"};

	mfs_write(&file, &lal, 6);
	
	file.inode_nb = mfs_creat("/");

	inode_load(&inode, file.inode_nb);
		
	file.inode = inode ;
	file.cursor_block= 0;
	file.cursor_byte = 0;
	file.mod = READ|WRITE ;
	
	seek_to_Ibitmap();
	change_bit(file.inode_nb);	

	seek_to_Bbitmap();
	printf("main : %d", first_free_bitmap());
		
	mfs_reload(&file, 0);
	
	char kel[6]={"kekek"};
	mfs_write(&file, &kel, 6);
	disk_close();
}
