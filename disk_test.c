#include "superBlock.h"
#include "disk.h"
#include "fs.h" 

void main(){

	T_inode inode;

	T_File file;
		
	int i=0;
	
	if(disk_creat(51200, "vdisk")==-1){
		printf("Disk exist !\n");
		
		disk_open("vdisk");
	
		sBk_load();
	}else{
		
		disk_open("vdisk");
		
		sBk_creat(1024, 10);
	
		sBk_write();
		
		sBk_load();
		
		seek_to_Bbitmap();
		change_bit(0);
	
		inode.file_size=0;
		inode.file_mode=0b00000000;
	
		for(i=0; i<=15; i++){
			inode.d_block[i]=0;
		}	

		file.inode_nb = 0 ;
		file.inode = inode ;
		file.cursor_block= 1;
		file.cursor_byte = 0;
		file.mod = READ|WRITE ;
	
		mfs_reload(&file, 0);
	
		change_inode_status(0);
	
		mfs_close(&file);

	}
	
	mfs_creat("/", "Eustache");	
	mfs_creat("/", "Nobunaga");	
	
	printf("TEST : Creat Eustache Monte\n");
	mfs_creat("/Eustache", "Montre");

	printf("TEST : Open Montre\n");	
	open("/Eustache/Montre", WRITE, &file);
	printf("TEST test :: %d", file.inode_nb);

	char test[4]={"Eva"};
	mfs_write(&file, test, 3);
	inode_write(&(file.inode), file.inode_nb);	

	mfs_creat("/", 	"Doha");

	disk_close();
}
