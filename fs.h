#ifndef MFS
#define MFS

#include "sys.h"
#include "superBlock.h"
#include "inode.h"
#include "disk.h"
#define READ 0b10000000
#define WRITE 0b01000000

typedef struct FD{
	uint32_t inode_nb ;
	T_inode inode ;
	uint32_t cusor_block ;
	uint8_t cusor_byte ;
	uint8_t mod ;
} T_File;

int mfs_open(const char* Path, uint8_t mod, T_File* file) ; // 0 open sucess -1 fail
int mfs_block_seek(unsigned int block) ; //Goes to block number "block" 
int mfs_file_seek(unsigned int byte) ; //Goes to byte number "byte" in file
int mfs_creat(const char* Path) ;
int mfs_close(T_File* file) ; // Write T_File.inode on disk and change mod to 0
int mfs_read(T_File* file , void* buff, uint32_t byte) ;
int mfs_write(T_File* file, void* buff, uint32_t byte) ;
int mfs_mkdir(const char* Path) ; 
int mfs_rm(const char* Path) ; //Change the inode block table to all zero and change ibitmap
#endif
