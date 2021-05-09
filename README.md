# DEVOIR MAISON D'IN603 - PRESENT24

Le but de ce devoir-maison est double. Il consiste d’abord en l’implantation en C d’un chiffrement par
bloc donné et en l’implantation ensuite de l’attaque par le milieu, vue en cours, sur la version double de ce chiffrement.

[![forthebadge](https://forthebadge.com/images/badges/built-by-developers.svg)](http://forthebadge.com) [![forthebadge](https://forthebadge.com/images/badges/powered-by-black-magic.svg)](https://forthebadge.com) [![forthebagde](https://forthebadge.com/images/badges/made-with-c.svg)](https://forthebadge.com) 

> «Si vous pensez que la cryptographie résoudra votre problème, soit vous ne comprenez pas la cryptographie, soit vous ne comprenez pas votre problème.» - Peter G. Neumann
# Pré-requis

Pour compiler et éxécuter ce programme, il vous faut deux ou trois petites choses :

- Un ordinateur.
- Une paire de mains.
- Des yeux.

# Compilation

Pour compiler ce programme il vous suffis de taper ``` make ``` dans un terminal.

Si tout ce passe comme souhaiter, vous devriez voir apparaitre le message suivant :

```gcc -O3 -Wall -Wextra -pedantic -pthread src/* -o main```

# Démarrage

Pour lancer ce programme, plusieurs choix vous sont offerts :
- ```./main -c -r``` : lance le chiffrement à partir d'un message clair et une clé maître.
- ```./main -c -d``` : lance le chiffrement en mode debug d'un message clair et d'une clé maître.

- ```./main -d -r ```: lance le dechiffrement à partir d'un message chiffré et une clé maître.
- ```./main -d -d ```: lance le dechiffrement en mode debug à partir d'un message chiffré et d'une clé maître.

- ```./main -a [1-2-4-8-16]```: lance l'attaque par le milieu sur 2PRESENT24 avec deux messages clairs et deux messages chiffrés. Pour le second argument, vous pouvez choisir entre les valeurs indiquées. Cela changera le nombre de coeurs qu'utilise notre programme (par defaut 4).

Vous pouvez bien sûr rajouter ```time``` devant chacunes des commandes pour en mesurer la vitesse.

Pour modifier les messages chiffrés, les messages clairs ou bien les clés maîtres, malheureusement, il faudra les modifier à la main dans ```main.c```.

# Auteurs

* **Quentin GRUCHET** _alias_ [@QGruchet](https://github.com/QGruchet) sur GitHub

# License

Ce projet est sous licence ``MIT``.

