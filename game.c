#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "curve.h"
#include "game.h"


void sample_curve_to_track (Curve *curve, Track *track, double theta){
  Control bez_points[4];
  int ind = 0;

  if (curve->control_count < 3) return;

  compute_bezier_points_prolong_first (curve, bez_points);
  sample_bezier_curve (bez_points, theta, track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 1);

  for (int k = 0; k < curve->control_count-3; k++) {
    compute_bezier_points_open (curve, k, bez_points);
    sample_bezier_curve (bez_points, theta, track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 0);
  }

  compute_bezier_points_prolong_last (curve, bez_points);
  sample_bezier_curve (bez_points, theta, track->sample_x, track->sample_y, &ind, SAMPLE_MAX, 0);
  track->sample_count = ind;
}

void move_shots_one_step(Game * game){
	
}

void shoot_ammo(Game * game){
	Shot shot;
	shot.dx = cos(game->canon.angle);
	shot.dy = sin(game->canon.angle);
	shot.x = shot.dx + game->canon.cx * 50;
	shot.y = shot.dy + game->canon.cy * 50;
	shot.color = game->canon.ammo1;
	game->shot_list.shots[game->shot_list.shot_count+1] = shot;
	game->shot_list.shot_count++;
	prepare_ammo(game);
}

void prepare_ammo(Game * game){
	game->canon.ammo1 = game->canon.ammo2;
	game->canon.ammo2 = rand() % LAST_COLOR;
}

void suppress_far_shots(Game * game){
	
}

void update_canon_angle(Game * game, double sx, double sy){
	double vx = sx - game->canon.cx;
	double vy = sy - game->canon.cy;
	double n = sqrt(vx*vx + vy*vy);
	double alpha = acos(vx/n);
	if((vy/n) < 0){
		alpha=-alpha;
	}
	game->canon.angle = alpha;
}

void load_cannon_image(Game * game){
	game->canon.image = cairo_image_surface_create_from_png (GAME_CANNON);
}

void update_x_and_y_canon(Game * game,int height, int width){
	game->canon.cx = width/2.0;
	game->canon.cy = height/2.0;	
}

void init_canon(Game * game,int height, int width){
	update_x_and_y_canon(game,height,width);
	game->canon.angle = 0.0;
	game->canon.ammo1 = rand() % LAST_COLOR;
	game->canon.ammo2 = rand() % LAST_COLOR;
	load_cannon_image(game);
}

void init_game(Game * game,int height, int width){
	init_canon(game,height,width);
}
