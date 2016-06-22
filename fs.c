#include "fs.h"

int mfs_open(const char* path, uint8_t mod, T_File * file){
    uint32_t  path_size;
    char ** processed_path;
    
    processed_path=mfs_path_process(path, &path_size);

    if (processed_path[0][0]!='/')
    {
        file->inode_nb=mfs_get_inode(path_size,processed_path);
    }
    else 
    {
        file->inode_nb=ROOT_DIRECTORY_INODE;
    }

    inode_load(&(file->inode),file->inode_nb);
    file->mod=mod;
    mfs_reload(file, 0);
    file->cursor_block=0;
    file->cursor_byte=0;
    mfs_reload(&file_descriptor,0);
}
    //                          --- Part 1: Découpage du chemin ---
char ** mfs_path_process(const char* path, int* path_size){
    printf("Start_mfs_open()\n");
    uint32_t  n_words=0, i, j, size=0;
    char c, ** ret=NULL, ** new_ret=NULL, *word=NULL, *new_word=NULL;
    if (path[0]!='/'){
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
uint32_t mfs_get_inode(uint32_t n_words, char** ret){
//                      --- Partie 2: Recherche du fichier ---
// n_words correspond a la Profondeur du fichier.
uint32_t line_inode, size_to_read, block_size, current_block,i=0,j=0;
    char * next, *line=NULL, *new_line=NULL, c;
    int found=0, k=0, m=0, l=0,size=0;
    T_inode* inode_current;
    printf("yolo1\n");
    inode_load(inode_current, ROOT_DIRECTORY_INODE);
    block_size=G_super_block.b_size;
    printf("%d, %d,\n",i,n_words);
    for (i=0;i<n_words;i++)                     //Chemin
    {
        printf("yolo3\n");
        next=ret[i];
        printf("yolo %s\n",ret[i]);
        current_block=inode_current->d_block[0];
        size_to_read=inode_current->file_size;
        printf("yolo5\n");
        do                                      //Blocks
        {
            mfs_block_seek(current_block);
            printf("yolo6\n");
            do
            {
                disk_read(&line_inode,4);
                block_size-=4;
                size_to_read-=4;
                disk_read(&c,1);
                size=1;
                printf("yolo%c\n",c);
                while (c!='\n' && block_size!=0)
                {
                    new_line=(char*)malloc((size + 1)*sizeof(char));
                    if(line==NULL)
                    {
                        new_line[0]=c;
                        new_line[1]='\0';
                    }
                    else
                    {
                        strcpy(new_line, line);
                        new_line[(size-1)]=c;
                        new_line[size]='\0';
                        free(line);
                    }
                    line=new_line;
                    new_line=NULL;
                    block_size--;
                    size_to_read--;
                    disk_read(&c,1);
                }
                if (strcmp(next,line)==0)
                {
                    found=1;
                }
                else
                {
                    found=0;
                    free(line);
                }
            }while (!found && block_size!=0 && size_to_read!=0);//On récupère la ligne d'après si ce n'est pas la bonne
            if (!found)
            {                   //On change de block
                if (j==14)
                {
                    disk_seek((inode_current->d_block[14])+4*k);
                    disk_read(&current_block,4);
                    disk_seek(current_block+4*l);
                    disk_read(&current_block,4);
                    disk_seek(current_block+4*m);
                    disk_read(&current_block,4);
                    m++;
                    if (m==256)
                    {
                        l++;
                        m=0;
                    }
                    if (l==256)
                    {
                        k++;
                        l=0;
                    }
                    if (k==256)
                    {
                        j++;
                        k=0;
                    }
                }
                if (j==13)// les blocks doublements indirects
                {
                    disk_seek((inode_current->d_block[13])+4*k);
                    disk_read(&current_block,4);
                    disk_seek(current_block+4*l);
                    disk_read(&current_block,4);
                    l++;
                    if (l==256)
                    {
                        k++;
                        l=0;
                    }
                    if (k==256)
                    {
                        j++;
                        k=0;
                    }
                }
                if (j==12)// les blocks indirects
                {
                     disk_seek(inode_current->d_block[12]+4*k);
                     disk_read(&current_block,4);
                     if (k==256)
                     {
                         k=0;
                         j++;
                     }
                }
                if (j<12)
                {
                    j++;
                    current_block=inode_current->d_block[j];
                 }
             }
                block_size=G_super_block.b_size;
        }while(!found && size_to_read!=0);
        if (found)
        {
            i++;
            j=0;
            return line_inode;
        }
        else
        {
            return ROOT_DIRECTORY_INODE; //fichier non trouvé
        }
    }//Bon techniquement là on doit avoir l'inode du fichier recherché.
}
/*                                 --- Partie 3: Construction du tableau de blocs ---

    free (line);
    file->inode_nb=line_inode;
    file->inode=*inode_current;
    file->cursor_byte=0;
    file->cursor_block=0;
    file->mod=mod;
    int block_n=((inode_current->file_size)/G_super_block.b_size)+(1*inode_current->file_size % G_super_block.b_size!=0);
    file->blocks=(uint32_t *)malloc(block_n*sizeof(uint32_t));
    for (i=0;i<block_n;i++)
    {
        if (i<12)//direct
        {
            file->blocks[i]=inode_current->d_block[i];
        }
        else if (i<268)//indirect
        {
            j=i-12;
            disk_seek(inode_current->d_block[12]+4*j);
            disk_read(&(file->blocks[i]),4);
        }
        else if (i<65804)//indirect double
        {
            j=(i-268)%256;
            k=((i-268)-k)/256;
            disk_seek(inode_current->d_block[13]+4*k);
            disk_read(&current_block,4);
            disk_seek(current_block+4*j);
            disk_read(&(file->blocks[i]),4);
        }
        else if (i<16843020)//indirect triple
        {
            j=(i-65804)%256;
            l=(i-65804-j)/65536;
            k=(i-65804-j-(l*65536))/256;
            disk_seek(inode_current->d_block[14]+4*l);
            disk_read(&current_block,4);
            disk_seek(current_block+4*k);
            disk_read(&current_block,4);
            disk_seek(current_block+4*j);
            disk_read(&(file->blocks[i]),4);
        }
    }
    return 0;
}*/

int mfs_close(T_File* file){

	file->mod = 0 ; //Can't read or writei
	free(file->blocks);
	return inode_write(&(file->inode), file->inode_nb) ;

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

	if(file->mod & READ == 0)
		return -1;

	if(file->cursor_block*G_super_block.b_size + file->cursor_byte + byte > file->inode.file_size)
		byte =  file->inode.file_size - file->cursor_block*G_super_block.b_size + file->cursor_byte ; 
	
	if(f_buff==NULL)
		return -1;

	
	disk_seek(G_super_block.b_size*file->blocks[file->cursor_block]+file->cursor_byte) ;
	
	if(byte > (G_super_block.b_size-file->cursor_byte)){
		to_read=G_super_block.b_size-file->cursor_byte ;
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
		printf("coucou\n");	
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

int mfs_creat(const char* Path, char* name){

	uint32_t i=0, j=0;
	T_inode inode;
	T_File dir;
	
	mfs_open(Path, WRITE, &dir);

	printf("Creat :dir: %d, %d", dir.inode_nb, dir.inode.file_size);

	mfs_file_seek(&dir, dir.inode.file_size);

	inode.file_size=0;
	inode.file_mode=0b00000000;
	
	for(i=0; i<=15; i++){
		inode.d_block[i]=0;
	}	

	seek_to_Ibitmap();	
	i = first_free_bitmap();

	printf("Creat %d\n", i);

	change_inode_status(i);
		
	inode_write(&inode, i);

	mfs_write(&dir, &i, sizeof(uint32_t));
	
	j = strlen(name);
	mfs_write(&dir, name, j);

	mfs_close(&dir);

	return j;
}
