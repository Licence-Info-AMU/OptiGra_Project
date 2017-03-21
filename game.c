#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
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
	for(int i =0; i < game->shot_list.shot_count;++i){
		game->shot_list.shots[i].x += game->shot_list.shots[i].dx * SHOT_SPEED;
		game->shot_list.shots[i].y += game->shot_list.shots[i].dy * SHOT_SPEED;
	}
}

void shoot_ammo(Game * game){
	if(game->shot_list.shot_count < SHOT_MAX){
		Shot shot;
		shot.dx = cos(game->canon.angle);
		shot.dy = sin(game->canon.angle);
		shot.x =  game->canon.cx + shot.dx* 50;
		shot.y =  game->canon.cy + shot.dy * 50;
		shot.color = game->canon.ammo1;
		game->shot_list.shots[game->shot_list.shot_count] = shot;
		game->shot_list.shot_count++;
		prepare_ammo(game);
	}
}

void prepare_ammo(Game * game){
	game->canon.ammo1 = game->canon.ammo2;
	game->canon.ammo2 = rand() % LAST_COLOR;
}

void swap_ammo(Game * game){
	int tmp = game->canon.ammo1;
	game->canon.ammo1 = game->canon.ammo2;
	game->canon.ammo2 = tmp;
}

void suppress_far_shots(Game * game,int screen_width, int screen_height){
	for(int i =0; i < game->shot_list.shot_count;++i){
		if( (game->shot_list.shots[i].x < 0) || (game->shot_list.shots[i].y < 0) || (game->shot_list.shots[i].y > screen_height) || (game->shot_list.shots[i].x > screen_width) ){
			memmove (game->shot_list.shots+i, game->shot_list.shots+i+1, sizeof(Shot)*(game->shot_list.shot_count-1-i));
			game->shot_list.shot_count--;
		}
	}
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

void process_shots_collisions(Game * game){
	
}

void move_trains_one_step(Game * game){
	
}

void check_end_of_game(Game * game){
	
}

void progress_game_next_step(Game * game,int screen_width, int screen_height){
	move_shots_one_step(game);
	suppress_far_shots(game,screen_width, screen_height);
	process_shots_collisions(game);
	move_trains_one_step(game);
	check_end_of_game(game);
}

void init_game(Game * game,int height, int width){
	init_canon(game,height,width);
}
