#ifndef DRAWINGS_H
#define DRAWINGS_H

static const int BLACK_HOLE = 30;

gboolean on_timeout (gpointer data);

void draw_curves (cairo_t *cr, Curve_infos *ci);

void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci);

void draw_bezier_polygons_open(cairo_t *cr,Curve_infos *ci);

void area_init (gpointer user_data);

void draw_bezier_curve(cairo_t *cr, Control bez_points[4], double theta);

void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta);

void draw_bezier_curves_close (cairo_t *cr, Curve_infos *ci, double theta);

void draw_bezier_curves_prolong (cairo_t *cr, Curve_infos *ci, double theta);

void generate_bezier_path(cairo_t *cr,Control bez_points[4],double theta,int is_first);

void draw_canon(cairo_t *cr, Game * game);

void draw_shots(cairo_t *cr, Game * game);

void draw_track(cairo_t *cr, Game * game);

void draw_marbles_bonus_labels(cairo_t *cr, Game * game,PangoLayout *layout);

void draw_marble(cairo_t *cr, Marble * marble);

void draw_marbles(cairo_t *cr, Game * game);

void switch_shot_color(cairo_t *cr,int color);

void update_Player_Frame(Mydata * my);

#endif // Fin DRAWINGS_H 
