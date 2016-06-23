#include "shell.h"

//Ne cause pas de fuite mémoire-- Il faut juste bien penser a free tout les mots ainsi que le tableau!
char** user_input (int* p_size){

    //Phase 1: récupérer l'input avec les espaces.
    char command[255]; //Les commandes sont limitées a 255 caractères...
    fgets(command, 254, stdin); //La fonction du fun!
   // printf("%s \n",command);
    //Phase 2: Préparer le tableau dynamique a retourner.
    int  n_words=0, i=0, j=0, size=0;
    char c, ** ret=NULL, ** new_ret=NULL, *word=NULL, *new_word=NULL;
    if (command[0]!='\n'){
    do{                                                         //                              ---LA BOUCLE QUI TACHE---
        c=command[i];                                           // J'avais pas envie de me faire chier a faire baucoup de tours dans le tableau, donc,
        while (c!='\n' && c!='\0'&& c!=' ')                     // Je fais des allocations succésives de tableaux, par ex, le tableau qui contient les mots
        {                                                       // est adressé plus grand d'une case, a chaque nouveau mot. Pareil pour les mots,
            size++;                                             // a chaque nouveau cractère, on copie dans un tableau plus grand... (J'espère que je vais pas oublier de free :/)
            new_word=(char*)malloc((size + 1)*sizeof(char));
            if(word==NULL)
            {
                new_word[0]=c;
                new_word[1]='\0';
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
            c=command[i];
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
	//	printf("user_input: %s\n", new_ret[j]);
            }
            new_ret[(n_words-1)]=word;
	//	printf("user_input: %s\n", new_ret[n_words-1]);
            free(ret);
        }
        ret=new_ret;
        new_ret=NULL;
        i++;
	word=NULL;
    }while(c!='\0' && c!='\n');
    *p_size=n_words;
    return ret;
    }
    else
    {
    word=(char*)malloc(2*sizeof(char));
    word[0]='\n';
    word[1]='\0';
    ret=(char**)malloc(sizeof(char*));
    ret[0]=word;
    return ret;
    }
}

int main (void){
    char** user_command=NULL;
    int command_length=0, i=0;
    T_File file;
    mfs_init();
    //Récup des commnade + exit
    do{
	printf("READY > \n");
        if (user_command!=NULL)
        {
            for (i=0;i<command_length;i++)
            {
                free (user_command[i]);
            }
            free(user_command);
        }
        user_command=user_input(&command_length);
	/*for (i=0;i<command_length;i++)
            {
                printf("%s ",user_command[i]);
            }
		printf("\n");*/
	if (strcmp(COMMAND_LS,user_command[0])==0)
        {
            if (command_length>1)
            {
                mfs_ls(user_command[1]);
		printf("DONE\n");
            }
        }
        else if(strcmp(COMMAND_CAT,user_command[0])==0)
        {
            if (command_length>1)
            {
                mfs_cat(user_command[1]);
            }
        }
        else if (strcmp(COMMAND_ASSERT,user_command[0])==0)
        {
            if (command_length>2)
            {
                mfs_assert(user_command[1],user_command[2]);
            }
        }

    }while (strcmp(COMMAND_EXIT,user_command[0])!=0);

    disk_close();

    return 0;
}

