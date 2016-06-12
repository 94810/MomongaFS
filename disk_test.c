#include "disk.h"

void main(){

	T_vdisk dsk;	
	char t=0xFF;
	
	if(create_disk(50, "vdisk")==-1)
		printf("Disk exist !\n");

	open_disk("vdisk", &dsk);
	
	disk_seek(dsk, 80); //Should fail

	disk_seek(dsk, 2); //Should sucess

	write(dsk.fd, &t, 1);

	disk_seek(dsk, 49);

	write(dsk.fd, &t, 1);

	close(dsk.fd);
}
