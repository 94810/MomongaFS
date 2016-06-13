#include "disk.h"

T_vdisk G_vdisk ; //Use Global fot he disk

int disk_creat(int size, const char* filepath){
	int fd_disk, i=0, check=0 ;
	uint8_t voi=0 ;
    
	fd_disk=open(filepath,O_CREAT|O_EXCL|O_WRONLY,0640);

    	if (fd_disk==-1){//Le disque existe déja
        	return -1;
    	}else{
		for (i=0;i<=size;i++){ //On rempli le fichier de 0
    			check=write(fd_disk, &voi, 1) ;
   		 }
	}
	
	close(fd_disk) ;
	
	return 0 ;
}

int disk_open(const char* filePath){
	
	int fd ;
	
	struct stat disk ;

	fd=open(filePath, O_RDWR | O_SYNC ) ; /*Opening in Read/Write and wait on writing acess that
						 all data has been writen before return */
	if(fd == -1){
		printf("Can't open vDisk at %s\n", filePath) ;
		return -1 ; // Quit if opening fail
	}
 
	if(fstat(fd, &disk )==-1){ //We get the stats to get the vdisk size, if we can't get stats -> Quit
		printf("Can't get vDisk stats of %s\n", filePath) ;
		return -1 ; 
	}

	G_vdisk.size = disk.st_size ;
	G_vdisk.fd = fd ;

	return 0;
}

int disk_seek(unsigned int byte){

	if(byte > G_vdisk.size){ //Check if dest byte is in the disk
		printf("Seeking outside Disk !\n");
		return -1;
	}else{
		lseek(G_vdisk.fd, byte, SEEK_SET ) ; //File offset is set to byte
	}

	return 0;
}

int disk_read(void* buffer, unsigned int bytes){
	return read(G_vdisk.fd, buffer, bytes) ;
}

int disk_write(void* buffer, unsigned int bytes){
	return write(G_vdisk.fd, buffer, bytes) ;
}

void disk_close(){
	close(G_vdisk.fd);
}


