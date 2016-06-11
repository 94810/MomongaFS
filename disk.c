#include <disk.h>

int create_disk(int size, const char* filepath){
    int fd_disk, fill=0,
     unsigned int voi=0;
    fd_disk=open(filepath,O_CREAT|O_EXCL,0640);
    if (fd_disk==-1){//Le disque existe déja
        return 12;
    }
    else
    
    for (fill=0;i<=size;i++){
    write(fd_disk,&voi,1);
    }
    close(fd_disk);
    return 0;
}

