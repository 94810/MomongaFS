#include "shell.h"

int main (void){
return 0;

}
char** user_input (int max_length){

    //Phase 1: récupérer l'input avec les espaces.
    char command[255]; //Les commandes sont limitées a 255 caractères...
    fgets(command, 254, stdin); //La fonction du fun!

    //Phase 2: Préparer le tableau dynamique a retourner.
    int  n_words=0, i, j, size=0;
    char c, ** ret=NULL, ** new_ret=NULL, *word=NULL, *new_word=NULL;
    do{                                                         //                              ---LA BOUCLE QUI TACHE---
        c=command[i];                                           // J'avais pas envie de me faire chier a faire baucoup de tours dans le tableau, donc,
        while (c!=' ' || c!='\0')                               // Je fais des allocations succésives de tableaux, par ex, le tableau qui contient les mots
        {                                                       // est adressé plus grand d'une case, a chaque nouveau mot. Pareil pour les mots,
            size++;                                             // a chaque nouveau cractère, on copie dans un tableau plus grand... (J'espère que je vais pas oublier de free :/)
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
    }while(c!='\0');
    return ret;
}
