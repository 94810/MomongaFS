#include "sys.h"

typedef struct Disk {
	unsigned int size; //Size of the Disk in Bytes
	int fd;		   //file descriptor of the vdisk	
} T_vdisk;


int disk_creat(int size, const char* filePath) ; 	/* 	Create a vdisk with file_name of size in byte
								Return 0 if sucess -1 else 
                                                                Returns -1 if file alredy exists
						 	*/

int disk_open(const char* filePath) ;	/*	Open the vdisk at filePath in desc
						Return 0 if sucess -1 else
					*/

int disk_seek(unsigned int byte) ;	/*
						Goto "bytes" in dsk 
					*/

int disk_read(void* buffer, unsigned int bytes) ; /* Attempt to read "bytes" bytes in disk
						     and place it in buffer
						  */

int disk_write(void* buffer, unsigned int bytes) ; // Attempt to write "bytes" bytes in disk from buffer

void  disk_close(); //close the disk

extern T_vdisk G_vdisk ; //Use Global for the disk
