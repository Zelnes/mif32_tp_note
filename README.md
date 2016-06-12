# mif32_tp_note
Tp noté de MIF32 Parallélisme

Projet réalisé par :

* **LIU ZHUYING** 11306849
* **GHESH MEHDI** 11209574

Les sources des programmes demandés se trouvent dans le répertoire ./test. Chacun contient un main.
Pour compiler les fichiers présents dans le répertoire test, il faut taper :

	$ make nomFichierMain
Ou nomFichierMain est le nom du fichier contenant le main à compiler, sans l'extension.

Exemple :
On veut compiler le fichier test/toto.c, on tape :

	$ make toto
Si la règle toto n'existe pas, il faut la créer. 

Créer une telle règle revient à ajouter
dans le makefile les lignes suivantes :

	# Regles pour le programme toto
	toto: CC=compilerAUtiliser
	toto: toto.t
Pour faire tous les tests et avoir un fichier log généré, il faut taper:

	$ make bench
Cela a pour effet de compiler les 4 programmes puis de les lancer suivant certaines configurations. Tous les résultats sont loggés dans le fichiers ./res_temps.txt
