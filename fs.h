#ifndef MFS
#define MFS

#include "sys.h"
#include "superBlock.h"
#include "inode.h"
#include "disk.h"
#define READ 0b10000000
#define WRITE 0b01000000
#define ADR_BLK_SIZE G_super_block.b_size/sizeof(uint32_t)

typedef struct FD{
	uint32_t inode_nb ;
	T_inode inode ;
	uint32_t cursor_block ;
	uint8_t cursor_byte ;
	uint8_t mod ;
	uint32_t*  blocks ; 
} T_File;
void mfs_init();
int mfs_open(const char* Path, uint8_t mod, T_File* file) ; // 0 open sucess -1 fail
char** mfs_path_process(const char* Path, int* path_size) ; // cuts the path
uint32_t mfs_get_inode(uint32_t path_size, char** path);
int mfs_block_seek(uint32_t block) ; //Goes to block number "block" 
void mfs_file_seek(T_File* file, unsigned int byte) ; //move cursor to byte number "byte" in file
int mfs_creat(const char* Path, char* name) ;
int mfs_close(T_File* file) ; // Write T_File.inode on disk and change mod to 0
int mfs_read(T_File* file , void* buff, uint32_t byte) ;
int mfs_write(T_File* file, void* buff, uint32_t byte) ;
int mfs_mkdir(const char* Path) ;
int mfs_rm(const char* Path) ; //Change the inode block table to all zero and change ibitmap
int mfs_alloc_block(T_File *file, uint32_t block); /* Allocate "block" block for file
							Maj the inode and the T_File */
void mfs_cat(const char* path,T_File * file);
void mfs_ls(const char* path);
void mfs_assert(const char* path,char* buff,T_File * file);

void mfs_reload(T_File* file, uint32_t block);
char* read_file_name(T_File* file); //Warning  cursor need to in place
void mfs_const_fd(T_File* file, uint32_t i_nb, uint32_t mod);
#endif
