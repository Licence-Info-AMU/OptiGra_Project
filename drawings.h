/**
 * \file drawings.h
 * \brief Fonctions de dessin dans area avec cairo
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Fonctions de dessin dans area avec cairo
 *
 */

#ifndef DRAWINGS_H
#define DRAWINGS_H

static const int BLACK_HOLE = 30;

gboolean on_timeout (gpointer data);

void apply_image_transforms (Mydata *data);

void draw_curves (cairo_t *cr, Curve_infos *ci);

void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci);

void draw_bezier_polygons_open(cairo_t *cr,Curve_infos *ci);

void area_init (gpointer user_data);

void draw_bezier_curve(cairo_t *cr, Control bez_points[4], double theta);

void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta);

void draw_bezier_curves_close (cairo_t *cr, Curve_infos *ci, double theta);

void draw_bezier_curves_prolong (cairo_t *cr, Curve_infos *ci, double theta);

void generate_bezier_path(cairo_t *cr,Control bez_points[4],double theta,int is_first);

/**
 * \fn void switch_shot_color(cairo_t *cr,int color)
 * \brief Fonction pour choisir la couleur de dessin du cairo en fonction de la couleur de la bille
 *
 * \param self cairo_t *cr,int color
 * \return void
 */
void switch_shot_color(cairo_t *cr,int color);

/**
 * \fn void draw_canon(cairo_t *cr, Game * game)
 * \brief Fonction d'affichage du canon
 *
 * \param self cairo_t *cr, Game * game
 * \return void
 */
void draw_canon(cairo_t *cr, Game * game);

/**
 * \fn void draw_shots(cairo_t *cr, Game * game)
 * \brief Fonction d'affichage des shots
 *
 * \param self cairo_t *cr, Game * game
 * \return void
 */
void draw_shots(cairo_t *cr, Game * game);

/**
 * \fn void draw_track(cairo_t *cr, Game * game)
 * \brief Fonction d'affichage de la track
 *
 * \param self cairo_t *cr, Game * game
 * \return void
 */
void draw_track(cairo_t *cr, Game * game);

/**
 * \fn void draw_marbles_bonus_labels(cairo_t *cr, Game * game,PangoLayout *layout)
 * \brief Fonction d'affichage du label du bonus de la bille
 *
 * \param self cairo_t *cr, Game * game,PangoLayout *layout
 * \return void
 */
void draw_marbles_bonus_labels(cairo_t *cr, Game * game,PangoLayout *layout);

/**
 * \fn void draw_marble(cairo_t *cr, Marble * marble)
 * \brief Fonction d'affichage d'une bille
 *
 * \param self cairo_t *cr, Marble * marble
 * \return void
 */
void draw_marble(cairo_t *cr, Marble * marble);

/**
 * \fn void draw_marbles(cairo_t *cr, Game * game)
 * \brief Fonction d'affichage des billes
 *
 * \param self cairo_t *cr, Game * game
 * \return void
 */
void draw_marbles(cairo_t *cr, Game * game);

/**
 * \fn void draw_title (Mydata * my, cairo_t *cr)
 * \brief Fonction d'affichage de l'écran titre
 *
 * \param self Mydata * my, cairo_t *cr
 * \return void
 */
void draw_title (Mydata * my, cairo_t *cr);

/**
 * \fn void update_Player_Frame(Mydata * my)
 * \brief Fonction de mise à jour du frame affichant les statistiques du joueur
 *
 * \param self Mydata * my
 * \return void
 */
void update_Player_Frame(Mydata * my);

/**
 * \fn void game_over_message_dialog(gpointer data)
 * \brief Fonction de la fenêtre du gameover
 *
 * \param self gpointer objet Mydata
 * \return void
 */
void game_over_message_dialog(gpointer data);

#endif // Fin DRAWINGS_H 
