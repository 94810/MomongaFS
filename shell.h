#ifndef SHELL_H
#define SHELL_H
#include "fs.h"
//MACROS SHELL

#define COMMAND_EXIT "exit"
#define COMMAND_CAT "cat"
#define COMMAND_LS "ls"
#define COMMAND_ASSERT "append"

//Prototypes//

char ** user_input (int* p_size);    // Retourne un tableau de chaînes de caractères, 
                                     //contenant les chaînes entrées par l'utilisateur, 
                                     //séparées par des espaces, 
                                     //ex: "touch mytralala" retorunera {{t,o,u,c,h,\0},{m,y,t,r,a,l,a,l,a,\0}}
                                     //Retourne également le nombre de mots, dans la variable pointée par p_size





#endif
