# `turing`

Implémentation ASCII de la fourmi de Langton, développée dans le cadre de l'article **La fourmi de Langton** de la revue du Palais de la Découverte.

## Description

La [fourmi de Langton(en)](https://en.wikipedia.org/wiki/Langton's_ant) est un automate cellulaire décrit par Christopher Langton, obéissant à des règles très simples, mais affichant un comportement complexe.

Ce programme permet de visualiser le comportement de la fourmi de Langton dans une région limitée ; le module `langton` fourni permet d'ajouter des fourmis supplémentaires obéissant à différents jeux d'instruction. Il permet de mettre en pause, d'accélérer, de ralentir, ou de revenir en arrière tout au long du déplacement.

# Déploiement

Le programme fourni est conçu pour un système GNU/Linux ([Debian(fr)](https://www.debian.org/index.fr.html)), mais peut être porté sur tout système supportant la bibliothèque `ncurses`.

Le fichier `Makefile` fourni permet de compiler le programme C.

## Paquetages nécessaires (Debian Stretch)
- `gcc`
- `make`
- `libncurses5-dev`

## Usage
```
git clone https://github.com/universcience/langton
cd langton
make
./langton
```

Le programme démarre en plaçant une fourmi de Langton au centre du terrain. La fourmi et son sens de déplacement sont représentés par un chevron (`^`/`<`/`v`/`>`), et les cases par des `.` (la couleur des cases noires étant inversée). Aussitôt démarré, le programe simule le déplacement de la fourmi, au rythme de 16 pas par seconde.

Les touches fléchées permettent d'ajuster la vitesse (haut pour doubler la vitesse, bas pour la diviser par deux) et la direction de la simulation (gauche pour revenir en arrière, droite pour aller de l'avant). La barre d'espace (ou la touche `p` pour *Pause*) permet de mettre en pause le calcul (indiqué par un marqueur `[PAUSE]`). Les touches `n` (*Next*) et `b` (*Back*) permettent de dérouler le calcul pas à pas, vers l'avant ou l'arrière. (En raison du comportement déterministe de la fourmi de Langton, il est possible de remonter les étapes jusqu'à avant l'instant zéro !)

Enfin, la touche `q` (*Quit*) permet de quitter le programme à tout moment.

# Licence

Ceci est un [logiciel libre(fr)](https://www.gnu.org/philosophy/free-sw.fr.html), distribué sous les termes de la licence GNU GPL Affero. Les termes exacts de cette licence sont disponibles dans le fichier `LICENSE`.

En résumé, vous êtes libres de télécharger ce logiciel, de l'utiliser et de le modifier sans restrictions, ainsi que de le redistribuer sous les mêmes termes, avec ou sans modifications. 
