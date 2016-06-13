#include "sys.h"
#include "superBlock.h"
#include "inode.h"
#include "disk.h"


int mfs_open() ;
int mfs_block_seek(unsigned int block) ; //Goes to block number "block" 
int mfs_file_seek(unsigned int byte); //Goes to byte number "byte" in file
int mfs_creat(const char* filePath) ;
int mfs_close() ;
int mfs_read() ;
int mfs_close() ;
int mfs_mkdir() ;
int mfs_rm() ;
