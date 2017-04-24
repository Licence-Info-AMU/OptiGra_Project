/**
 * \file game.h
 * \brief Fonctions et objets principaux du jeu
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Fonctions et objets principaux du jeu
 *
 */

#ifndef GAME_H
#define GAME_H

/**
 * \enum Game_state
 * \brief Constantes d'état du jeu.
 *
 * Game_state est une série de constantes prédéfinie pour la gestion d'état du jeu.
 */
typedef enum {GS_HELLO, GS_PLAYING, GS_PAUSE, GS_WON, GS_LOST} Game_state;

/**
 * \enum Track_state
 * \brief Constantes d'état de la track.
 *
 * Track_state est une série de constantes prédéfinie pour la gestion d'état de la track.
 */
typedef enum {TS_INTRO, TS_NORMAL, TS_COMBO2, TS_COMBO3} Track_state;

/**
 * \enum AmmoColor
 * \brief Constantes des couleurs
 *
 * AmmoColor est une série de constantes prédéfinie pour la gestion des couleurs
 */
typedef enum {RED,GREEN,BLUE,YELLOW,PURPLE,INDIGO,LAST_COLOR} AmmoColor;

/**
 * \enum Bonus_state
 * \brief Constantes des bonus
 *
 * Bonus_state est une série de constantes prédéfinie pour la gestion des bonus
 */
typedef enum {BS_NONE,BS_TIME_STOP,BS_TIME_SLOWER,BS_TIME_FASTER,BS_MARBLE_EXPLOSIVE} Bonus_state;

#define SHOT_MAX       10
#define SHOT_SPEED      5
#define TRACK_MAX      10
#define MARBLE_MAX_AT_START    20
#define MARBLE_MAX    200
#define SAMPLE_MAX   1000 
#define LEVEL_MAX      10
#define SAMPLE_THETA    0.05
#define MARBLE_SPEED 2
#define MARBLE_SPEED_END_GAME 10
#define GAME_CANNON "canon.png"
#define BONUS_TIME 5 //en secondes
#define SCORE_PENALITY 5;
static const char RESOURCES_DIR[] = "Resources/";
static const char IMAGE_DIR[] = "Image/";
static const char LEVEL_DIR[] = "Level/";
static const char TRACK_DIR[] = "Track/";
static const char TRACK_EXTENSION[] = ".track";
static const char LOGO_HORIZON[] = "Logohorizon.jpg";
static const int MARBLE_RAYON = 20; //Rayon
static const int MARBLE_DIAMETRE = 40; //Diametre
static const int MARBLE_DIAMETRE2 = 1600; //Diametre au carré

/**
 * \struct Canon
 * \brief Objet contenant les informations pour un canon
 *
 * Objet contenant les informations pour un canon
 * double cx, cy;    // centre canon
 * double angle;     // en radians
 * int ammo1, ammo2;
 * cairo_surface_t *image;
 */
typedef struct {
  double cx;    			/*!< Centre canon cx. */ 
  double cy;				/*!< Centre canon cy. */ 
  double angle;     		/*!< Angle du canon. */ 
  int ammo1;				/*!< Couleur ammo1. */ 
  int ammo2;				/*!< Couleur ammo2. */ 
  cairo_surface_t *image;	/*!< Image du canon. */ 
} Canon;

/**
 * \struct Shot
 * \brief Objet contenant les informations pour un shot
 *
 * Objet contenant les informations pour un shot
 * double x, y;      // coordonnÃ©es centre
 * double dx, dy;    // vecteur dÃ©placement
 * int color;
 */
typedef struct {
  double x;      	/*!< coordonnées centre x. */ 
  double y;			/*!< coordonnées centre y. */ 
  double dx;    	/*!< vecteur déplacement dx. */ 
  double dy;		/*!< vecteur déplacement dy. */ 
  int color;		/*!< Couleur du shot. */ 
} Shot;

/**
 * \struct Shot_list
 * \brief Objet contenant les informations pour un Shot_list
 *
 * Objet contenant les informations pour un Shot_list
 * int shot_count;
 * Shot shots[SHOT_MAX];
 */
typedef struct {
  int shot_count;		/*!< Nombre de shot. */ 
  Shot shots[SHOT_MAX]; /*!< Tableau de shots. */ 
} Shot_list;

/**
 * \struct Marble
 * \brief Objet contenant les informations pour un Marble
 *
 * Objet contenant les informations pour un Marble
 * double x, y;      // coordonnÃ©es centre
 * double t;         // paramÃ¨tre dans l'Ã©chantillonnage
 * int color;
 * int is_combo_end; // ou encore, facteur vitesse et direction ?
 * int step_explode;
 * int bonus;
 */
typedef struct {
  double x;			/*!< coordonnÃ©es centre x. */ 
  double y;      	/*!< coordonnÃ©es centre y. */ 
  double t;         /*!< paramÃ¨tre dans l'échantillonnage. */ 
  int color;		/*!<Couleur de la bille. */		
  int is_combo_end; // ou encore, facteur vitesse et direction ?
  int step_explode;	
  int bonus;		/*!< Bonus de la bille. */
} Marble;

/**
 * \struct Track
 * \brief Objet contenant les informations pour un Track
 *
 * Objet contenant les informations pour un Track
 * int    sample_count;          // Ã©chantillonnage courbe
 * double sample_x[SAMPLE_MAX], 
 * sample_y[SAMPLE_MAX];
 * int marble_count;
 * int first_visible;
 * double marbles_speed;
 * Marble marbles[MARBLE_MAX];
 * Track_state state;
 */
typedef struct {
  int    sample_count;          	/*!< Échantillonnage courbe. */
  double sample_x[SAMPLE_MAX];		/*!< Sample_x. */
  double sample_y[SAMPLE_MAX];		/*!< Sample_y. */
  int marble_count;					/*!< Nombre de marble. */
  int first_visible;				/*!< Première bille visible. */
  double marbles_speed;				/*!< Vitesse du train de bille. */
  Marble marbles[MARBLE_MAX];		/*!< Tableau de marble. */
  Track_state state;				/*!< État de la track. */
} Track;

/**
 * \struct Track_list
 * \brief Objet contenant les informations pour un Track_list
 *
 * Objet contenant les informations pour un Track_list, actuellement une seules track est utilisé mais cette objet a été gardé afin de pouvoir gérer le multi-track facilement plus tard
 * int track_count;
 * Track tracks[TRACK_MAX];
 */
typedef struct {
  int track_count;				/*!< Nombre de track actuellement. */
  Track tracks[TRACK_MAX];		/*!< Liste des tracks. */
} Track_list;

/**
 * \struct Level
 * \brief Objet contenant les informations pour un Level
 *
 * Objet contenant les informations pour un Level, non utilisé pour le moment car le nombre de bille d'intro et total est calculé avec une fonction quant à la position du canon elle est fixe
 * Curve_infos curve_infos;
 * double canon_x, canon_y;
 * int marbles_intro, marbles_total;
 */
typedef struct {
  Curve_infos curve_infos;				/*!< La track du level sous forme de curve. */
  double canon_x;						/*!< Position x du canon. */
  double canon_y;						/*!< Position y du canon. */
  int marbles_intro;					/*!< Nombre de marble à l'intro. */
  int  marbles_total;					/*!< Nombre de marble total. */	
} Level;

/**
 * \struct Level_list
 * \brief Objet contenant les informations pour un Level_list
 *
 * Objet contenant les informations pour un Level_list, non utilisé pour le moment car le level est chargé au changement et non au début du jeu
 * int level_count;
 * Level levels[LEVEL_MAX];
 */
typedef struct {
	int level_count;			/*!< Nombre de level max actuellement. */
	Level levels[LEVEL_MAX];	/*!< Liste des levels. */
} Level_list;

/**
 * \struct Bonus
 * \brief Objet contenant les informations pour un Bonus
 *
 * Objet contenant les informations pour un Bonus
 * Bonus_state b_state;
 * int seconds; //temps restant du bonus en seconde
 */
typedef struct {
	Bonus_state b_state;	/*!< État du bonus. */
	int seconds; 			/*!< Temps de bonus restant. */
} Bonus;

/**
 * \struct Game
 * \brief Objet contenant les informations pour un Game
 *
 * Objet contenant les informations pour un Game
 * Game_state state;
 * Bonus bonus;
 * int current_level;
 * int score;
 * int score_level_before;
 * int levelSupSpeedMalus;
 * Canon canon;
 * Shot_list shot_list;
 * Track_list track_list;
 * Level_list level_list;
 */
typedef struct {
  Game_state state; 			/*!< État du jeu. */
  Bonus bonus; 					/*!< État du jeu. */
  int current_level;			/*!< Level courant. */
  int score;					/*!< Score. */
  int score_level_before;		/*!< État du jeu. */
  int levelSupSpeedMalus;		/*!< Malus vitesse après retour au level 0 après avoir gagné les de 0 à 10 levels. */
  Canon canon;					/*!< Objet Canon. */
  Shot_list shot_list;			/*!< Liste des shots tirés. */
  Track_list track_list;		/*!< Liste des tracks. */	
  Level_list level_list;		/*!< Liste des levels. */
} Game;

/**
 * \fn void sample_curve_to_track (Curve *curve, Track *track, double theta)
 * \brief Fonction de conversion d'une curve en une track
 *
 * \param self Objet Curve *curve, Track *track, double theta
 * \return void
 */
void sample_curve_to_track (Curve *curve, Track *track, double theta);

/**
 * \fn int load_curve_from_file(Curve_infos *ci,char *filename)
 * \brief Fonction pour tirer un shot
 *
 * \param self Objet Game
 * \return void
 */
void shoot_ammo(Game * game);

/**
 * \fn int is_color_on_track(Track * track, int color)
 * \brief Fonction pour chercher si une couleur est dans la track
 *
 * \param self Objet Track * track, int color
 * \return retourne 1 si la couleur est déjà dans la track et 0 si elle ne l'ait pas
 */
int is_color_on_track(Track * track, int color);

/**
 * \fn void prepare_ammo(Game * game)
 * \brief Fonction pour préparer les balles du canon après un tir
 *
 * \param self Objet Game
 * \return void
 */
void prepare_ammo(Game * game);

/**
 * \fn void swap_ammo(Game * game)
 * \brief Fonction pour echanger les balles du canon
 *
 * \param self Objet Game
 * \return void
 */
void swap_ammo(Game * game);

/**
 * \fn void game_pause(Game * game)
 * \brief Fonction pour mettre en pause le jeu si il ne l'est pas sinon le remet en mode playing
 *
 * \param self Objet Game
 * \return void
 */
void game_pause(Game * game);

/**
 * \fn int calcule_score_with_marble_group_size(Game * game,Track * track, int marble_id_start,int score,int bonus)
 * \brief Fonction pour calculer le score obtenu après un tir
 *
 * \param self Objet Game * game,Track * track, int marble_id_start,int score,int bonus
 * \return int
 */
int calcule_score_with_marble_group_size(Game * game,Track * track, int marble_id_start,int score,int bonus);

/**
 * \fn void check_bonus(Game * game,Track * track,int marble)
 * \brief Fonction pour vérifier si il y a un bonus
 *
 * \param self Objet Game * game,Track * track,int marble
 * \return void
 */
void check_bonus(Game * game,Track * track,int marble);

/**
 * \fn void check_bonus_end(Game * game)
 * \brief Fonction pour vérifier un bonus est terminé
 *
 * \param self Objet Game
 * \return void
 */
void check_bonus_end(Game * game);

/**
 * \fn void move_shots_one_step(Game * game)
 * \brief Fonction pour avancer le shot
 *
 * \param self Objet Game
 * \return void
 */
void move_shots_one_step(Game * game);

/**
 * \fn void suppress_far_shots(Game * game,int screen_width, int screen_height)
 * \brief Fonction de suppression des tirs lointains
 *
 * \param self Objet Game * game,int screen_width, int screen_height
 * \return void
 */
void suppress_far_shots(Game * game,int screen_width, int screen_height);

/**
 * \fn void do_vector_product(double xu,double yu,double zu,double xv,double yv,double zv, double *x, double *y,double *z
 * \brief 
 *
 * \param self double xu,double yu,double zu,double xv,double yv,double zv, double *x, double *y,double *z)
 * \return void
 */
void do_vector_product(double xu,double yu,double zu,double xv,double yv,double zv, double *x, double *y,double *z);

/**
 * \fn int test_collision (Game * game, int *shot_num, int *marble_num, int track_num)
 * \brief fonctione pour tester si il y a une colision entre un shot et une bille du train
 *
 * \param self Game * game, int *shot_num, int *marble_num, int track_num
 * \return int renvoie 1 si il y a une colision et 0 sinon
 */
int test_collision (Game * game, int *shot_num, int *marble_num, int track_num);

/**
 * \fn void process_shots_collisions(Game * game)
 * \brief insertion de la bille si il y a eu colision
 *
 * \param self Game * game
 * \return void
 */
void process_shots_collisions(Game * game);

/**
 * \fn void move_trains_one_step(Game * game)
 * \brief Fonction pour faire avancer le train
 *
 * \param self Game * game
 * \return void
 */
void move_trains_one_step(Game * game);

/**
 * \fn void check_end_of_game(Game * game)
 * \brief Fonction pour vérifier si la partie est terminé
 *
 * \param self Game * game
 * \return void
 */
void check_end_of_game(Game * game);

/**
 * \fn void progress_game_next_step(Game * game,int screen_width, int screen_height)
 * \brief Fonction d'étape du jeu (boucle principale)
 *
 * \param self Game * game,int screen_width, int screen_height
 * \return void
 */
void progress_game_next_step(Game * game,int screen_width, int screen_height);

/**
 * \fn void update_x_and_y_canon(Game * game,int height, int width)
 * \brief Fonction de mise à jour de la postion du canon par rapport à l'écran pour le centrer
 *
 * \param self Game * game,int screen_width, int screen_height
 * \return void
 */
void update_x_and_y_canon(Game * game,int height, int width);

/**
 * \fn void load_cannon_image(Game * game)
 * \brief Fonction pour charger l'image du canon
 *
 * \param self Game * game
 * \return void
 */
void load_cannon_image(Game * game);

/**
 * \fn void update_canon_angle(Game * game, double sx, double sy)
 * \brief Fonction pour mettre à jour l'angle du canon
 *
 * \param self Game * game,double sx, double sy
 * \return void
 */
void update_canon_angle(Game * game, double sx, double sy);

/**
 * \fn void init_canon(Game * game, int height, int width)
 * \brief Fonction pour l'initialisation du canon
 *
 * \param self Game * game,int height, int width
 * \return void
 */
void init_canon(Game * game, int height, int width);

/**
 * \fn void init_ammo(Game * game)
 * \brief Fonction pour l'initialisation des munitions du canon
 *
 * \param self Game * game
 * \return void
 */
void init_ammo(Game * game);

/**
 * \fn void init_shots(Game * game)
 * \brief Fonction pour l'initialisation des shots
 *
 * \param self Game * game
 * \return void
 */
void init_shots(Game * game);

/**
 * \fn int init_marble_bonus()
 * \brief Fonction pour la génération du bonus aléatoirement
 *
 * \param self 
 * \return int le bonus généré aléatoirement
 */
int init_marble_bonus();

/**
 * \fn void create_marbles(Track * track,int current_level)
 * \brief Fonction pour créer les billes
 *
 * \param self Track * track,int current_level
 * \return void
 */
void create_marbles(Track * track,int current_level);

/**
 * \fn void init_track(Game * game)
 * \brief Fonction pour l'initialisation de la track
 *
 * \param self Game * game
 * \return void
 */
void init_track(Game * game);

/**
 * \fn void init_game(Game * game, int height, int width)
 * \brief Fonction pour l'initialisation du jeu
 *
 * \param self Game * game, int height, int width
 * \return void
 */
void init_game(Game * game, int height, int width);

/**
 * \fn void reset_game(Game * game,int height, int width)
 * \brief Fonction de réinitialisation du jeu
 *
 * \param self Game * game, int height, int width
 * \return void
 */
void reset_game(Game * game,int height, int width);

/**
 * \fn void restart_game(Game * game,int height, int width)
 * \brief Fonction pour recommencer un level
 *
 * \param self Game * game, int height, int width
 * \return void
 */
void restart_game(Game * game,int height, int width);

/**
 * \fn void change_level(Game * game,int height, int width)
 * \brief Fonction pour changer de level
 *
 * \param self Game * game, int height, int width
 * \return void
 */
void change_level(Game * game,int height, int width);

/**
 * \fn void time_stop(Game * game)
 * \brief Fonction pour arrêter le temps ( le train)
 *
 * \param self Game * game
 * \return void
 */
void time_stop(Game * game);

/**
 * \fn void speed_change(Game * game,int bonus)
 * \brief Fonction pour changer la vitesse du train, accélérer ou bien ralentir
 *
 * \param self Game * game,int bonus
 * \return void
 */
void speed_change(Game * game,int bonus);

#endif /* GAME_H */
