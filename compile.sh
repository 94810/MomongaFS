#!/bin/sh
#Fichier de  compile vite fait
gcc disk.c fs.c inode.c shell.c superBlock.c -g -o shell
gcc disk.c fs.c inode.c disk_test.c superBlock.c -g -o disk_test
