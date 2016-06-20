#include "fs.h"

int mfs_open(const char* path, uint8_t mod, T_File * file){

    //                          --- Part 1: Découpage du chemin ---
    int  n_words=0, i, j, size=0;
    char c, ** ret=NULL, ** new_ret=NULL, *word=NULL, *new_word=NULL;
    do{
         c=path[i];
         while (c!='\n' && c!='\0'&& c!='/')
         {
             size++;
             new_word=(char*)malloc((size + 1)*sizeof(char));
             if(word==NULL)
             {
                 new_word[0]=c;
                 
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
//                      --- Partie 2: Recherche du fichier ---
// n_words correspond a la Profondeur du fichier.
    uint32_t line_inode, size_to_read, block_size, current_block;
    char * next, *line=NULL, *new_line=NULL;
    int found=0, k=0, m=0, l=0;
    T_inode* inode_current;
    i=0;
    j=0;
    load_inode(inode_current, ROOT_DIRECTORY_INODE);
    block_size=G_super_block.b_size;
    for (i=0;i<n_words;i++)                     //Chemin
    {
        next=ret[i];
        current_block=inode_current->d_block[0];
        size_to_read=inode_current->file_size;
        do                                      //Blocks
        {
            mfs_block_seek(current_block);
            do
            {
                disk_read(&line_inode,4);
                block_size-=4;
                size_to_read-=4;
                disk_read(&c,1);
                size=1;
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
                    disk_seek((inode_current->d_block[12])+4*k);
                    disk_read(&current_block,4);
                    k++;
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
            inode_load(inode_current, line_inode);
            i++;
            j=0;
        }
        else
        {
            return -1; //fichier non trouvé
        }
    }//Bon techniquement là on doit avoir l'inode du fichier recherché.

//                                  --- Partie 3: Construction du tableau de blocs ---
    
}

int mfs_close(T_File* file){

	file->mod = 0 ; //Can't read or writei
	free(file->block);
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
	
	if(byte > (8-file->cursor_byte)){
		to_read=8-file->cursor_byte ;
		disk_read(f_buff, to_read);
		nb_bitread=to_read;
		file->cursor_block++;
		file->cursor_byte=0;            //On se calle sur le prochain block

		to_read = byte-nb_bitread / 8 ; //On regarde le nombre de block a lire

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

		if(file->cursor_byte > 7){
			file->cursor_block++;
			file->cursor_byte=0;
		}
	}		

	return nb_bitread ;
}


