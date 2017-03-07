# OptiGra_Project
Projet d'Interface Graphique de licence 2

Projet Zuma's revenge

1) Déroulement du jeu

Après un écran d'accueil, apparaît un rail en forme de courbe, qui commence par
une source et finit par un trou noir.

Un "train" de 40 billes de couleurs sort rapidement de la source, puis le train 
passe à une vitesse faible et le joueur peut commencer à tirer.

La queue du train continue à avancer avec l'apparition continuelle de nouvelles 
billes de la source, qui pousse le train en avant. Ce train cesse d'être alimenté
après apparition de 60 billes supplémentaires.

La partie est perdue si la tête du train pénètre dans le trou noir.
La partie est gagnée si toutes les billes sont supprimées par le joueur.

Vers le centre de la fenêtre il y a un canon, qui tourne en direction de la souris.
Chaque fois que le joueur clique, une bille de couleur est tirée du canon.
La balle part tout droit, et soit elle touche une bille du train, soit elle sort
de la fenêtre et disparaît. Si la bille tirée touche une bille du train, elle
vient s'insérer avant ou après dans le train.

Lors de l'insertion, on compte le nombre n de billes adjacentes de même couleur 
que la bille insérée : si n >= 3, alors le groupe est supprimé du rail.

Dans ce cas, on compte ensuite le nombre p de billes de la même couleur de part
et d'autre du groupe supprimé :
 - si p < 3, la partie avant reste immobile, tandis que la partie arrière
   continue à avancer ;
 - sinon c'est un "combo 2" : la partie avant recule jusqu'à toucher la
   partie arrière (qui a continué à avancer), puis le groupe de p billes est
   supprimé.

À la fin d'un combo 2 on compte ensuite le nombre q de billes de la même couleur
de part et d'autre du groupe supprimé : si q >= 3 alors c'est un "combo 3" : la
partie avant recule jusqu'à toucher la partie arrière (qui a continué à avancer)
puis pousse tout le train en arrière, qui rentre à reculons dans la source, de
n+p+q billes.

Et ainsi de suite pour les combos supérieurs (mais sans autre effet sur le train).


2) Calcul du score :

Lorsqu'on supprime un groupe de k boules, le score est augmenté de k*10.

Lors de chaque combo de niveau i pour k boules, le score est augmenté de 
k*10 * 2^i. Exemple :

  EAABCCCCD + D + DDCBBAF
  EAABCCCC + CBBAF         s += 4*10
  EAAB + BBAF              s += 5*10 * 2^2  (combo 2)
  EAA + AF                 s += 3*10 * 2^3  (combo 3)
  E + F                    s += 3*10 * 2^4  (combo 4)
  EF
 

3) Cahier des charges

- Au lancement du programme il y a un écran d'accueil avec : titre, auteurs,
image fond, "pressez une touche".

- barre menu : (à améliorer)
  game / start, pause, new, quit
  level / edit, new, save
  help / about

- fenêtre de jeu : 
  en haut : titre, niveau, score
  canon au milieu (pas forcément, dépend du niveau)
  un rail ou plusieurs
  chaque rail commence par une source et est terminé par un trou noir
  image fond selon niveau

- les rails sont des B-splines cubiques uniformes avec extrémités prolongées,
  comme vu en cours. Ils sont mémorisés avec la structure de données du TP5.
  Ils sont dessinés avec une certaine épaisseur, voire avec plusieurs épaisseur 
  et un dégradé de couleurs.

- le canon tourne avec la souris
  clic souris envoie une balle de couleur
  on peut envoyer des balles à la suite (jusqu'à 10 par exemple)
  chaque balle continue tout droit jusqu'à rencontrer un obstacle ou une bille
  du train.

- Munitions : la couleur de la bille du canon est choisie au hasard, parmi les
  couleurs présentes dans le train.
  on voit sur le canon la bille qui va être tirée, et celle d'après
  La barre d'espace permute les 2 billes.
  Ces munitions sont mises à jour si des couleurs disparaissent des rails.

- affichage meilleurs scores en fin de partie, et sauvegarde

- niveaux
  lorsqu'un niveau est gagné, passage automatique niveau par niveau
  si perdu, on recommence le niveau courant
  si dernier niveau atteint, retour au premier avec des boules en plus et
  une circulation 50% plus rapide

- éditeur de niveau : avec l'interface du TP6 apurée
  boutons radio add/move/remove curve et control
  chaque niveau : image fond, position canon, courbes, 
    champs de saisie pour : nb billes début et total, vitesse
  on peut charger/sauver un niveau.
  niveau = fichier texte dans le répertoire des niveaux Levels/

- effets supplémentaires :
    mise à l'échelle générale selon taille fenêtre, en conservant ratio 
     (à prévoir dès le début)
    billes = cercle ou marbre ; rotation billes
    disparition billes éclatées
    animation échange billes dans canon
    accélération/freinage du train

- bonus :
    bille spéciale qui permet de tirer d'autres billes (flèche ?)
    bille qui fait reculer le train 
    ...


4) Réalisation

États : du jeu, du canon, du train, des combos ?

État du jeu game_state : GS_HELLO, GS_PLAYING, GS_PAUSE, GS_WON, GS_LOST.
Le timer est actif pendant l'état GS_PLAYING.

Modules ? canon, tracks, score / un seul module game

Shot_list : tableau de billes tirées shots, nombre de billes shot_count
Pour chaque bille :
  vecteur déplacement, coordonnée, numéro couleur

Track_list : tableau de voies tracks, nombre de voies track_count
ne pas stocker dans la structure des courbes et points de contrôle,
mais avec numéro des courbes correspondantes.

Pour chaque voie, stoque échantillonnage + tableau de billes + first_visible

Pour chaque bille :
  numéro couleur, coordonnées, étape éclatement
On ne stocke pas les tronçons adjacents, on teste les distances au moment voulu

Calcul position des billes sur les rails :
- échantillonnage et stockage
- paramètre t et interpolation
- recherche incrémentale puis dichotomique à distance fixée, dans les 2 sens
- utilisation : distance = diamètre pour pousser, ou = pas pour la vitesse
  selon framerate (par exemple diamètre / framerate, soit 1 bille/seconde)

Calcul collisions
- recherche bille A la plus proche (distance au carré)
- si d <= diametre + epsilon alors bingo
- point d'insertion : calculer positions des billes virtuelles B avant et après A,
  on insère à l'endroit de la plus proche, dans la position virtuelle. 
  Cas particulier : extrémité de rail (dans ce cas, position symétrique).

Attention éliminer multi clics : 
lorsque 2 clics rapprochés, on_area_button press appelé 3 fois, avec evb->type = 
  GDK_BUTTON_PRESS (1er clic)
  GDK_BUTTON_PRESS (2e clic)
  GDK_2BUTTON_PRESS (double clic détecté)
idem triples clics et GDK_3BUTTON_PRESS.
-> ne traiter que lorsque evb->type ==  GDK_BUTTON_PRESS.
