#include "shell.h"
int main (void){
    char** wololo;
    int n, i;
    wololo=user_input(&n);
    printf ("nombre de mots: %d\n", n);
    for (i=0;i<n;i++){ // Ne pas oublier de free!!
        free (wololo[i]);
    }
    free(wololo);
    return 0;
}
