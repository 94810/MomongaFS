#include "fs.h"

int mfs_open(const char* path, uint8_t mod, T_File * file){
    uint32_t  path_size;
    char ** processed_path;
    
    processed_path=mfs_path_process(path, &path_size);
    printf("OPEN : parsed path\n");
    printf("OPEN : String :%s, %s, %s", processed_path[0], processed_path[1], processed_path[2]);

    if (processed_path[0][0]!='/')
    {
	printf("OPEN : Get inode nb\n");
        file->inode_nb=mfs_get_inode(path_size,processed_path);
    }
    else 
    {
        file->inode_nb=ROOT_DIRECTORY_INODE;
    }

    mfs_const_fd(file, file->inode_nb, mod);
   
	printf("OPEN inode_nb: %d\n", file->inode_nb);
}
    //                          --- Part 1: Découpage du chemin ---

char ** mfs_path_process(const char* path, int* path_size){
    printf("Start_mfs_open()\n");
    uint32_t  n_words=0, i=0, j=0, size=0;
    char c, ** ret=NULL, ** new_ret=NULL, *word=NULL, *new_word=NULL;
    if (!(path[0]=='/' && path[1]=='\0')){
       // printf("lel\n");
        do{
             c=path[i];
            while (c!='\n' && c!='\0'&& c!='/')
            {
                 size++;
                new_word=(char*)malloc((size + 1)*sizeof(char));
                if(word==NULL)
                {
                     new_word[0]=c;
                    new_word[size]='\0';
                }
                else
                {
                     strcpy(new_word, word);
                    new_word[(size-1)]=c;
                    new_word[size]='\0';
                    free(word);
                }
                word=new_word;
                new_word=NULL;
                i++;
                c=path[i];
             }
            size=0;
            n_words++;
            new_ret=(char**)malloc(n_words*sizeof(char*));
            if (ret==NULL)
            {
                 new_ret[0]=word;
                word=NULL;
            }
            else
            {
                 for(j=0;j<(n_words-1);j++)
                {
                     new_ret[j]=ret[j];
                }  
                new_ret[(n_words-1)]=word;
                free(ret);
            }
            ret=new_ret;
            new_ret=NULL;
            i++;
        }while(c!='\0' && c!='\n');
    }
    else
    {
        //printf("lal1\n");
        word=(char*)malloc(2*sizeof(char));
       // printf("lal2\n");
        word[0]='/';
       // printf("lal3\n");
        word[1]='\0';
       // printf("lal4\n");
        ret=(char**)malloc(sizeof(char*));
       // printf("lal5\n");
        ret[0]=word;
        n_words=1;
       // printf("lal6\n");
    }
    printf("mfs_open() part 1 done\n");
    *path_size=n_words;
    return ret;
}

char* read_file_name(T_File* file){
	char *chaine=NULL, *buff=NULL;
	char actl=0, nb=0, i=0;
	printf("SEARCH File Name : ");
	do{
		mfs_read(file, &actl, 1);
		nb++;
		buff = malloc(nb*sizeof(char));
		for(i=0 ; i<nb-1 ; i++){
			buff[i]=chaine[i];
		}
		if(actl!='\n')
			buff[nb-1]=actl;
		else
			buff[nb-1]='\0';
		free(chaine);
		chaine=buff;
	}while(actl!='\n');

	printf("%s\n", chaine);
	return chaine;
}

uint32_t mfs_get_inode(uint32_t n_words, char** ret){
	T_File curr ;
	char* line=NULL;
	uint32_t i_nb=0;
	int i=0, k=1;
	
	mfs_const_fd(&curr, 0, READ);

	printf("SEARCH n_words: %d \n", n_words);

	for(i=0; i<n_words; i++){
		while(k!=0){
			if(line != NULL)
				free(line);
			k = mfs_read(&curr, &i_nb, sizeof(uint32_t)) ;
			printf("INODE SEACH :: %d\n") ;
			line = read_file_name(&curr) ;
			if(strcmp(line, ret[i])==0){
				break;
			}	

			if(k==0){
				return 0;
			}
		}

		mfs_const_fd(&curr, i_nb, READ);
	}
	printf("End file looking\n");
	return i_nb;
}

void mfs_const_fd(T_File* file, uint32_t i_nb, uint32_t mod){
	file->inode_nb = i_nb; //Charge inode nb
	inode_load(&(file->inode), file->inode_nb); //Load inode
	file->cursor_block= 0; //Place cursor
	file->cursor_byte = 0;
	file->mod = mod; // Set mod
	mfs_reload(file, 0); //Reload block
}
int mfs_close(T_File* file){

	file->mod = 0 ; //Can't read or write
	free(file->blocks);
	return inode_write(&(file->inode), file->inode_nb);
}

int mfs_block_seek(uint32_t block){
	return disk_seek(block*G_super_block.b_size) ;	
}

void mfs_file_seek(T_File* file, unsigned int byte){
	file->cursor_block = byte / G_super_block.b_size ;
	file->cursor_byte = byte % G_super_block.b_size ;
}

int mfs_read(T_File* file , void* buff, uint32_t byte){
	
	uint8_t* f_buff = buff;
	
	unsigned int nb_bitread = 0, to_read = 0, i=0 ;
	
	int temp=0;

	if(file->mod & READ == 0)
		return -1;

	if(file->cursor_block*G_super_block.b_size + file->cursor_byte + byte > file->inode.file_size){
		if(file->inode.file_size < file->cursor_block*G_super_block.b_size + file->cursor_byte){
			printf("EOF\n");
			return 0;
		}else
			byte = file->inode.file_size - file->cursor_block*G_super_block.b_size + file->cursor_byte;
		
	}
	if(f_buff==NULL)
		return -1;

	disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte) ;
	
	if(byte > (G_super_block.b_size-file->cursor_byte)){
		to_read = G_super_block.b_size - file->cursor_byte ;
		disk_read(f_buff, to_read);
		nb_bitread=to_read;
		file->cursor_block++;
		file->cursor_byte=0;            //On se calle sur le prochain block

		to_read = byte-nb_bitread / G_super_block.b_size ; //On regarde le nombre de block a lire

		for(i=0 ; i<to_read ; i++){
			disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte);
			
			disk_read(f_buff+nb_bitread, G_super_block.b_size);

			file->cursor_block ++ ;
		
			nb_bitread+= G_super_block.b_size ;
		} //On lis les block

		if(byte-nb_bitread!=0){
			disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte);
			
			file->cursor_byte = disk_read(f_buff+nb_bitread, byte-nb_bitread) ;
	 		
			nb_bitread += file->cursor_byte;
		} //On lit le reste forcément < 1 block
		
	}else{
		nb_bitread += disk_read(buff, byte);
		
		file->cursor_byte+=byte ;

		if(file->cursor_byte > G_super_block.b_size){
			file->cursor_block++;
			file->cursor_byte=0;
		}
	}		

	return nb_bitread ;
}

int mfs_write(T_File* file, void* buff, uint32_t byte){
	unsigned int init_block_size, final_block_size, nb_byteWrite=0, to_write=0, i=0;

	uint8_t *f_buff = buff;

	//Calc File size in block
	init_block_size = file->inode.file_size / G_super_block.b_size;
	if(file->inode.file_size % G_super_block.b_size != 0)
		init_block_size++;	
			
	//Calc File size in block AFTER wirting
	final_block_size = file->cursor_block + ((file->cursor_byte + byte) / G_super_block.b_size);
	if((file->cursor_byte + byte) % G_super_block.b_size != 0)
		final_block_size++;
	
	//If we need more space give it
	if(init_block_size<final_block_size){
		mfs_alloc_block(file, final_block_size-init_block_size);
	}
	
	//On écrit 
	
	disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte) ;
	
	if(byte > (G_super_block.b_size-file->cursor_byte)){
		to_write=G_super_block.b_size-file->cursor_byte ;
		disk_write(f_buff, to_write);
		nb_byteWrite=to_write;
		file->cursor_block++;
		file->cursor_byte=0;            //On se calle sur le prochain block

		to_write = byte-nb_byteWrite / G_super_block.b_size ; //On regarde le nombre de block a écrire

		for(i=0 ; i<to_write ; i++){
			disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte);
			
			disk_write(f_buff+nb_byteWrite, G_super_block.b_size);

			file->cursor_block ++ ;
		
			nb_byteWrite+= G_super_block.b_size ;
		} //On écrit les blocks

		if(byte-nb_byteWrite!=0){
			disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte);
			
			file->cursor_byte = disk_write(f_buff+nb_byteWrite, byte-nb_byteWrite) ;
	 		
			nb_byteWrite += file->cursor_byte;
		} //On écrit le reste forcément < 1 block
		
	}else{
		nb_byteWrite += disk_write(buff, byte);
		
		file->cursor_byte+=byte ;

		if(file->cursor_byte > G_super_block.b_size){
			file->cursor_block++;
			file->cursor_byte=0;
		}
	}		

	file->inode.file_size += nb_byteWrite ;
	return nb_byteWrite ;
	
	return 0;
}

int mfs_alloc_block(T_File *file, uint32_t block){
	int fileSizeBlk = 0;
	uint32_t tmp=0, kek=block;

	fileSizeBlk = file->inode.file_size / G_super_block.b_size ;
	if(file->inode.file_size % G_super_block.b_size != 0 ) 
		fileSizeBlk ++;
	
	while(block!=0){
		if(fileSizeBlk<12){ //Direct Block
			seek_to_Bbitmap();
			file->inode.d_block[fileSizeBlk] = first_free_bitmap();
			change_block_status(file->inode.d_block[fileSizeBlk]);
			fileSizeBlk++;

		}else if(fileSizeBlk < ADR_BLK_SIZE + 12){ //Indirect Block
			if(fileSizeBlk==12){
				seek_to_Bbitmap();
				file->inode.d_block[INDIRECT_BLOCK] = first_free_bitmap();
				change_block_status(file->inode.d_block[fileSizeBlk]);
			}

			disk_seek(file->inode.d_block[INDIRECT_BLOCK]+(fileSizeBlk-12));
			seek_to_Bbitmap();
			tmp = first_free_bitmap();
			disk_write(&tmp, 4);
			change_block_status(tmp);
		}/*else if(fileSizeBlk < ( ADR_BLK_SIZE * ADR_BLK_SIZE ) + 12 ){ //Double Indirect Block
				
		}else if(fileSizeBlk < ( ADR_BLK_SIZE * ADR_BLK_SIZE * ADR_BLK_SIZE )+ 12){ //Triple INdirect block

		}*/else{ //Error file too large
			return -1;			
		}

		block--;

	}

	inode_write(&(file->inode), file->inode_nb) ;
	
	mfs_reload(file, kek);

	return 0;
}

void mfs_reload (T_File* file, uint32_t block){
	uint32_t virtual_size=0, i=0, j=0; 
	
	
	inode_load(&(file->inode), file->inode_nb);
	
	virtual_size = file->inode.file_size + block*G_super_block.b_size ;

	file->blocks = malloc(virtual_size);
	
	i=virtual_size / G_super_block.b_size;	
	if(virtual_size % G_super_block.b_size != 0)
		i ++;

	
	for(j=0;j<i;j++){
		if(j<12){
			file->blocks[j]=file->inode.d_block[j];
		}else if(j==12){
			disk_seek(file->inode.d_block[INDIRECT_BLOCK]);
			disk_read(&(file->blocks[j]),4);
		}
		else if(j < ADR_BLK_SIZE + 12){	
			disk_read(&(file->blocks[j]),4);
		}else{
			printf("System Error ! File too big\n");
		}
	}
	
}

int mfs_mkdir(const char* Path, char* name){
	
	uint32_t i=0, j=0;
	T_inode inode;
	T_File dir;
	char n='\n';
	
	mfs_open(Path, WRITE, &dir);

	printf("Creat :dir: i_nb : %d size : %d \n", dir.inode_nb, dir.inode.file_size);

	mfs_file_seek(&dir, dir.inode.file_size);

	inode.file_size=0;
	inode.file_mode=0b10000000;
	inode.link_count=1;
	
	for(i=0; i<=15; i++){
		inode.d_block[i]=0;
	}

	seek_to_Ibitmap();	
	i = first_free_bitmap();

	printf("Creat :inode free: %d \n", i);

	change_inode_status(i);
		
	inode_write(&inode, i);

	mfs_write(&dir, &i, sizeof(uint32_t));
	
	j = strlen(name);

	mfs_write(&dir, name, j);
	mfs_write(&dir, &n, 1);

	mfs_close(&dir);

	return j;

}

int mfs_creat(const char* Path, char* name){

	uint32_t i=0, j=0;
	T_inode inode;
	T_File dir;
	char n='\n';
	
	mfs_open(Path, WRITE, &dir);

	printf("Creat :dir: i_nb : %d size : %d \n", dir.inode_nb, dir.inode.file_size);

	mfs_file_seek(&dir, dir.inode.file_size);

	inode.file_size=0;
	inode.file_mode=0b00000000;
	inode.link_count=1;
	
	for(i=0; i<=15; i++){
		inode.d_block[i]=0;
	}

	seek_to_Ibitmap();	
	i = first_free_bitmap();

	printf("Creat :inode free: %d \n", i);

	change_inode_status(i);
		
	inode_write(&inode, i);

	mfs_write(&dir, &i, sizeof(uint32_t));
	
	j = strlen(name);

	mfs_write(&dir, name, j);
	mfs_write(&dir, &n, 1);

	mfs_close(&dir);

	return j;
}
void mfs_init(){
    T_inode inode;

    T_File file;
            
    int i=0;
        
    if(disk_creat(51200, "vdisk")==-1)
    {
        printf("Disk exist !\n");
        disk_open("vdisk");                                
        sBk_load();
    }
    else
    {
        disk_open("vdisk");                                                              
        sBk_creat(1024, 10);
        sBk_write();
        sBk_load();
        seek_to_Bbitmap();
        change_bit(0);
        inode.file_size=0;
        inode.file_mode=0b00000000;
        for(i=0; i<=15; i++)
        {
            inode.d_block[i]=0;
        }
        file.inode_nb = 0 ;
        file.inode = inode ;
        file.cursor_block= 0;
        file.cursor_byte = 0;
        file.mod = READ|WRITE ;
        inode_write(&inode, 0);
        change_inode_status(0);
    }  
}

void mfs_cat(const char* path,T_File * file){
    
    char cat_buff;
    int lu;
    
    if(file->inode.file_mode & 0b10000000 == 0)
    {
        mfs_open(path, READ, file);
        do
        {
            lu=mfs_read(file, &cat_buff, 1);
            if(lu!=0)
                printf("%c",cat_buff);
        }while(lu!=0);
        
        mfs_close(file);
    }
    
   else
   {    
       printf("Ce n'est pas un fichier !\n"); 
   }
   
}


void mfs_ls(const char* path){
    
    char ls_buff='\0';
    int lu,i, trig=1;
	uint32_t kek;
    
    T_File file;
    
    mfs_open(path, READ, &file);
   
	printf("Contenu de %s\n", path);
 
     if(file.inode.file_mode & 0b10000000 != 0)
    {
        printf("Ce n'est pas un répertoire\n");
    }
    
    else
    {
        do
        {
       	    lu=mfs_read(&file, &kek,4);
	    while(ls_buff!='\n'){
			lu=mfs_read(&file, &ls_buff, 1);
			printf("%c", ls_buff);
			if(lu==0)
				break;
            }
	    if(lu!=0)
	          ls_buff='\0';

        }while(lu!=0);
        mfs_close(&file);
    }

	printf("FIN DE CONTENU\n");
}

void mfs_assert(const char* path,char* buff,T_File * file){
    
    mfs_file_seek(file, file->inode.file_size);
    
    mfs_write(file, buff, strlen(buff));
    
}

