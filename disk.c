#include "disk.h"

int create_disk(int size, const char* filepath){
	int fd_disk, i=0 ;
	unsigned int voi=0 ;
    
	fd_disk=open(filepath,O_CREAT|O_EXCL,0640);

    	if (fd_disk==EEXIST){//Le disque existe déja
        	return -1;
    	}else{
		for (i=0;i<=size;i++){ //On rempli le fichier de 0
    			write(fd_disk, &voi, 1) ;
   		 }
	}
	
	close(fd_disk) ;
	
	return 0 ;
}


int open_disk(const char* filePath, T_vdisk* desc){
	
	int fd ;
	
	struct stat disk ;

	fd=open(filePath, O_RDWR | O_SYNC ) ; /*Opening in Read/Write and wait on writing acess that
						 all data has been writen before return */
	if(fd == -1){
		printf("Can't open vDisk at %s\n", filePath) ;
		return -1 ; // Quit if opening fail
	}
 
	if(!fstat(fd, &disk )){ //We get the stats to get the vdisk size, if we can't get stats -> Quit	
		printf("Can't get vDisk stats of %s\n", filePath) ;
		return -1 ; 
	}

	desc->size = disk.st_size ;
	desc->fd = fd ;

	return 0;
}

int disk_seek(T_vdisk dsk, unsigned int byte){

	if(byte > dsk.size){ //Check if dest byte is in the disk
		printf("Seeking outside Disk !\n");
		return -1;
	}else{
		lseek(dsk.fd, byte, SEEK_SET ) ; //File offset is set to byte
	}

	return 0;
}
