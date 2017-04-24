Gaëtan Perrot et Florian Duarte

gaetan.perrot@etu.univ-amu.fr
florian.duarte@etu.univ-amu.fr

Titre du jeu : Marble Horizon

Compilation :

Dans un terminal, se mettre dans le dossier contenant les fichiers de code, puis taper make.

Execution :

Toujours dans le dossier, taper ./MarbleHorizon

Utilisation du programme :

Comment fonctionne les bonus :
	B0 sur une bille = aucun bonus
	B1 sur une bille = arrêt du temps
	B2 sur une bille = ralentissement du temps
	B4 sur une bille = bille explosive
	B3 n'est pas utilisé mais serait l'accélération du train mais nous n'avons pas voulu complexifié la tâche du joueur et avons pensé à cela en cas d'un mode online.

A l'écran titre, il faut appuyer sur n'importe quelle touche ou se servir du menu pour lancer le jeu.

Le jeu commence au niveau 0. Pour passer au niveau suivant, il faut remporter la partie.

Une fois la partie gagnée, on passe automatiquement au niveau suivant.

Une fois le dernier niveau atteint, le jeu reprend au niveau 0 avec la vitesse augmentée de 50%.

Lors de la partie, on peut appuyer sur Espace pour échanger les munitions du canon, P pour mettre le jeu en pause ou Q pour quitter le jeu.

On peut recommencer le niveau en cliquant sur restart dans le menu game du jeu, gérer la pause avec start et pause qui mettent tout deux en pause si le jeu ne l'est pas ou enlêve pause
si le jeu est en pause.

On peut éditer un nouveau niveau avec le menu level->new ou bien directement level->edit.
On peut remetre à zero notre niveau en faisant level->new.
L'édition est une simple édition de niveau, le nombre de bille et la vitesse sont calculé dans le programme par uen fonction propre.
Une fois le niveau modifié/créé, il faut sauvegarder le niveau avec Level->Save.
On peut toutefois importer un niveau existant (fichier .track) avec le menu load ( level-load) afin de le modifié et ainsi créer un nouveau niveau à partir d'un ancien.
Une fois la sauvegarde effectuée, vous pouvez continuer d'utiliser le logiciel.
Vous remarquerez que l'on ne peut pas charger un niveau seul mais cependant nous pouvons sauvegarder les nouveaux niveaux dans le dossier Resources/Level/track/x.track allant jusqu'à 6 niveaux
de 0 à 5.
Cette fonctionnalité est dans le but de pousser le joueur à créer des niveaux qui puissent se suivre en terme de difficultée afin d'apprécier au mieux le jeu.

Ce que j'ai fait par rapport à l'énoncé :

L'écran d'accueil avec fond d'écran mais pas titre.
Le train de n billes, avec sortie rapide des premières billes et l'impossibilité de tirer tant que le train est en mode intro (3 /4 des billes).

Victoire si le train est vide, défaite si une bille touche le trou noir d'arrivée.

Gestion du canon et des tirs, et insertion en cas de collision avec le train.

Suppression si la bille insérée appartient à un groupe de taille >= 3 de la même couleur.

Gestion des combos s'il y a un groupe de taille >= 3 de la même couleur autour du premier groupe supprimé. 
La partie avant recule et la partie arrière contnue à avancer pour le combo2.
En cas de combo supplémentaire (combo3), la partie avant recule jusqu'à collision, le groupe est supprimé et tout le train rentre dans la source de n billes (taille des trois groupes supprimés).

Le train reprend noralement après ça, s'il y a à nouveau un combo le groupe est supprimé et ainsi de suite.

Gestion du score (affiché en permanence pendant la partie) en fonction de la taille des groupes supprimés et du combo actuel.

La barre de menu, en trois parties :

 - Game :  New qui recommence au niveau 0, Restart qui recharge le niveau actuel,start et pause poru gèrer la pause et Quit pour quitter le jeu.

 - Level : New pour créer un niveau(remet à zero), Editing pour éditer un niveau vide, Save pour sauvegarder le niveau crée ou modifié et load pour charger un niveau pour pouvoir le modifier.

 - Help : About

Les rails du trains (seulement 1 track par niveau)

Augmentation de vitesse à chaque fois que l'on réussit le dernier niveau.


Ce qui me semble intéressant dans le code :

Principalement la gestion des combos, que ce soit l'insertion de la bille dans le train, la vérification du groupe de couleurs, 
la suppression du groupe en question et la gestion des combos supplémentaires avec les mouvements du train qui en résultent. 
(Partie avant immobile à la base qui doti reculer s'il y a un combo supplémentaire, ou tout le train qui rentre dans la source pour un combo 3).
Les bonus sur les billes rajouter par rapport au cahier des charges : bonus arrête du temps(le train s'arrête), ralentissement du temps (le train ralentit) pour 5 secondes chacun
ainsi que la bille explosive qui détruit la bille à sa gauche et à sa droite si elle ne doivent pas être détruite, elle déclenche un combo explosive si elle rencontre une bille explosive
et l'active à son tour.
L'edition de niveau qui est pratique mais mériterait d'être encore améliorer pour le nombre de bille, vitesse et modifier directement le niveau courant.
Une documentation fourni. 
