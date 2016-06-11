#include "sys.h"

typedef struct Disk {
	unsigned int size; //Size of the Disk in Bytes
	int fd;		   //file descriptor of the vdisk	
} T_vdisk;

int create_disk(int size, const char* filePath) ; 	/* 	Create a vdisk with file_name of size in byte
								Return 0 if sucess -1 else 
                                                                Returns -1 if file alredy exists
						 	*/
int open_disk(const char* filePath, T_vdisk* desc) ;	/*	Open the vdisk at filePath in desc
								Return 0 if sucess -1 else
						 	*/
int disk_seek(T_vdisk dsk, unsigned int byte) ;		/*
								Goto "bytes" in dsk 
							*/
