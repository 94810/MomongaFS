#include "disk.h"

void main(){

	T_vdisk dsk;	
	char t=0xFF;
	
	if(disk_creat(50, "vdisk")==-1)
		printf("Disk exist !\n");

	disk_open("vdisk");
	
	disk_seek(80); //Should fail

	disk_seek(2); //Should sucess

	disk_write(&t, 1);

	disk_seek(49);

	disk_write(&t, 1);
	
	disk_seek(5);

	disk_read(&t, 1);

	printf("Test : %d\n", t);

	disk_close();
}
