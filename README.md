# README #

## Projet Billard ##

Implémentation d'un jeu de billard en c++
- Possibilité de contrôler la boule avec le clavier
- Boule invincible qui ne se détruit pas quand elle tombe dans un trou
- Boule tueuse qui détruit les boules qu'elle rencontre

## Comment compiler ? ##

### Pour compiler l’application finale (mode graphique et mode texte) ###

Voici les commandes à exécuter pour compiler l’application graphique :

D’abord nous vous recommandons de vous placer dans le sous dossier bin/ :


```
#!bash

$ cd bin/
$ qmake ../Qt/application_seule.pro
$ make
```


Les exécutables se trouvent dans les sous-dossiers du répertoire courant (ici bin/).



###Pour compiler les exercices et les tests unitaires###

Voici les commandes à exécuter pour compiler l’application graphique :

D’abord nous vous recommandons de vous placer dans le sous dossier bin/ :


```
#!bash

$ cd bin/
$ qmake ../Qt/tests_et_exercices.pro
$ make
```


Les exécutables se trouvent dans les sous-dossiers du répertoire courant (ici bin/).