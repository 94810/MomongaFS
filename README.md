MomongaFS
=========
### Intro

MomongaFS est un système de fichiers crée dans le cadre d'un projet de première année de cycle ingénieur à l'Institut des Sciences et Techniques des Yvelines.
Le but est la création d'un système de fichier simple et fonctionel, pour pouvoir le tester il sera acompagné d'un MiniShell.

###Structure du Disque

Le Disque est organisé de la manière suivante :

* Le Superblock
* L'Inode Bitmap
* Les Inodes
* La Block Bitmap
* Les Data

L'inode 0 désigne le dossier racine.

###Structure d'un Dossier

Un Dossier est un fichier spécial permetant d'organiser les fichiers. Il est structuer en "lignes" chaque ligne indique un fichier, une ligne est structrer ainsi :
	
	* 4 bytes (un uint32_t) représentant le numéro de l'inode du fichier
	* Une série de caractéres ASCII (a l'exeption de '/', '>', '\n')  
	* Un symbole de fin de ligne '\n'

Le dossier racine est désigner par un '/' seul.
Pour acceder à un fichier on donne la suite de dossier a suivre pour le trouver depuis le dossier racine. Exemple de chemin '/animal/oiseaux/pelican'

---

###Référence
extX :
* [Cours sur l'extX](https://www.fbi.h-da.de/fileadmin/personal/h.baier/Lectures-winter-11/WS-11-Forensics/vorlesung_forensik_ws11-12_kap07-ext-handout.pdf)
* [Description de l'ext2](http://www.nongnu.org/ext2-doc/ext2.html)



