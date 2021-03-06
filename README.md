# Projet Billard #

Implémentation d'un jeu de billard en c++ : 

* Possibilité de contrôler la boule avec le clavier

* Boule invincible qui ne se détruit pas quand elle tombe dans un trou

* Boule tueuse qui détruit les boules qu'elle rencontre


Vidéo de démonstration : https://youtu.be/9lRThd8fRhs

![Capture d’écran 2015-05-25 à 20.50.33.png](https://bitbucket.org/repo/Ra4eo6/images/3459652271-Capture%20d%E2%80%99%C3%A9cran%202015-05-25%20%C3%A0%2020.50.33.png)


# Comment compiler ? #

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


# Hiérarchie du répertoire #

Nous avons découplé le moteur physique (billard/) de l’interface graphique (Qt/Billard_OpenGL/)

Les tests unitaires sont dans le même répertoire que la classe qu’ils testent.

Les makefile pour qmake sont tous dans le répertoire Qt/ ou ses sous-répertoires.

#Licence#

Le coeur physique de ce projet est publié sous *MIT Licence*. 

La partie qui utilise Qt est sous licence *LGPL*.


Les bibliothèques (répertoire /Billard/libraries/) ont chacune leur licence.

* Bandit Test C++ : https://github.com/joakimkarlsson/bandit

* Multiple Dispatch Wrapper : https://bitbucket.org/Gauss_/library-multiple-dispatch-in-cpp

* String Parser : https://bitbucket.org/Gauss_/library-string-parser