#ifndef GAME_H
#define GAME_H

typedef enum {GS_HELLO, GS_PLAYING, GS_PAUSE, GS_WON, GS_LOST} Game_state;
typedef enum {TS_INTRO, TS_NORMAL, TS_COMBO2, TS_COMBO3} Track_state;
typedef enum {RED,GREEN,BLUE,YELLOW,PURPLE,INDIGO,LAST_COLOR} AmmoColor;
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

typedef struct {
  double cx, cy;    // centre canon
  double angle;     // en radians
  int ammo1, ammo2;
  cairo_surface_t *image;
} Canon;

typedef struct {
  double x, y;      // coordonnÃ©es centre
  double dx, dy;    // vecteur dÃ©placement
  int color;
} Shot;

typedef struct {
  int shot_count;
  Shot shots[SHOT_MAX];
} Shot_list;

typedef struct {
  double x, y;      // coordonnÃ©es centre
  double t;         // paramÃ¨tre dans l'Ã©chantillonnage
  int color;
  int is_combo_end; // ou encore, facteur vitesse et direction ?
  int step_explode;
  int bonus;
} Marble;

typedef struct {
  int    sample_count;          // Ã©chantillonnage courbe
  double sample_x[SAMPLE_MAX], 
         sample_y[SAMPLE_MAX];
  int marble_count;
  int first_visible;
  double marbles_speed;
  Marble marbles[MARBLE_MAX];
  Track_state state;
} Track;

typedef struct {
  int track_count;
  Track tracks[TRACK_MAX];
} Track_list;

typedef struct {
  Curve_infos curve_infos;
  double canon_x, canon_y;
  int marbles_intro, marbles_total;
} Level;

typedef struct {
  int level_count;
  Level levels[LEVEL_MAX];
} Level_list;

typedef struct {
	Bonus_state b_state;
	int seconds; //temps restant du bonus en seconde
} Bonus;

typedef struct {
  Game_state state;
  Bonus bonus;
  int current_level;
  int score;
  int score_level_before;
  int levelSupSpeedMalus;
  Canon canon;
  Shot_list shot_list;
  Track_list track_list;
  Level_list level_list;
} Game;

void sample_curve_to_track (Curve *curve, Track *track, double theta);

void shoot_ammo(Game * game);

int is_color_on_track(Track * track, int color);

void prepare_ammo(Game * game);

void swap_ammo(Game * game);

void game_pause(Game * game);

int calcule_score_with_marble_group_size(Game * game,Track * track, int marble_id_start,int score,int bonus);

void check_bonus(Game * game,Track * track,int marble);

void check_bonus_end(Game * game);

void move_shots_one_step(Game * game);

void suppress_far_shots(Game * game,int screen_width, int screen_height);

int test_collision (Game * game, int *shot_num, int *marble_num, int track_num);

void process_shots_collisions(Game * game);

void move_trains_one_step(Game * game);

void check_end_of_game(Game * game);

void progress_game_next_step(Game * game,int screen_width, int screen_height);

void update_x_and_y_canon(Game * game,int height, int width);

void update_canon_angle(Game * game, double sx, double sy);

void init_canon(Game * game, int height, int width);

void init_ammo(Game * game);

void init_shots(Game * game);

int init_marble_bonus();

void create_marbles(Track * track,int current_level);

void init_track(Game * game);

void init_game(Game * game, int height, int width);

void reset_game(Game * game,int height, int width);

void restart_game(Game * game,int height, int width);

void change_level(Game * game,int height, int width);

void time_stop(Game * game);

void speed_change(Game * game,int bonus);

#endif /* GAME_H */
