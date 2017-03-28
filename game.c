#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "curve.h"
#include "game.h"

//Début GameUtils
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

void game_pause(Game * game){
	if(game->state == GS_PAUSE)
		game->state = GS_PLAYING;
	else if(game->state == GS_PLAYING)
		game->state = GS_PAUSE;
}
//Fin GameUtils

//Début progress_game_next_step
void move_shots_one_step(Game * game){
	for(int i =0; i < game->shot_list.shot_count;++i){
		game->shot_list.shots[i].x += game->shot_list.shots[i].dx * SHOT_SPEED;
		game->shot_list.shots[i].y += game->shot_list.shots[i].dy * SHOT_SPEED;
	}
}

void suppress_far_shots(Game * game,int screen_width, int screen_height){
	for(int i =0; i < game->shot_list.shot_count;++i){
		if( (game->shot_list.shots[i].x < 0) || (game->shot_list.shots[i].y < 0) || (game->shot_list.shots[i].y > screen_height) || (game->shot_list.shots[i].x > screen_width)){
			memmove (game->shot_list.shots+i, game->shot_list.shots+i+1, sizeof(Shot)*(game->shot_list.shot_count-1-i));
			game->shot_list.shot_count--;
		}
	}
}

void process_shots_collisions(Game * game){
	for(int i =0; i < game->shot_list.shot_count;++i){

	}
	/*
	   pour chaque shot :
            cherche la bille la plus proche sur tous les tracks
            dist^2 = distance^2 entre cette bille et le shot
            si dist^2 <= diamètre^2 :
                calcule si on doit insérer avant ou après
                insère la bille
                calcule taille groupe de même couleur
                si taille groupe >= 3 : supprime le groupe en tassant track_list
	 */
}

void move_trains_one_step(Game * game){
	Track * track = &game->track_list.tracks[game->current_level];
	double tb,xb,yb,dx,dy,dist;
	tb = compute_distant_point_forward (track->sample_x, track->sample_y, track->marbles[track->first_visible].t, track->sample_count, track->marbles_speed, &xb, &yb);
	if(tb >= 0){
		track->marbles[track->first_visible].x = xb;
		track->marbles[track->first_visible].y = yb;
		track->marbles[track->first_visible].t = tb;
	}else{
		game->state = GS_LOST;
	}
	dx = track->marbles[track->first_visible].x - track->sample_x[0];
	dy = track->marbles[track->first_visible].y - track->sample_y[0];
	dx *= dx;
	dy *= dy;
	dist = dx + dy;
	if(dist > MARBLE_SIZE2){
		track->first_visible--;
		tb = compute_distant_point_backward (track->sample_x, track->sample_y, track->marbles[track->first_visible].t, track->sample_count, track->marbles_speed, &xb, &yb);
		if(tb >= 0){
			track->marbles[track->first_visible].x = xb;
			track->marbles[track->first_visible].y = yb;
			track->marbles[track->first_visible].t = tb;
		}
	}
	for(int i = track->first_visible+1; i < track->marble_count;++i){
		tb = compute_distant_point_forward (track->sample_x, track->sample_y, track->marbles[i].t, track->sample_count, track->marbles_speed, &xb, &yb);
		if(tb >= 0){
			track->marbles[i].x = xb;
			track->marbles[i].y = yb;
			track->marbles[i].t = tb;
		}else{
			game->state = GS_LOST;
		}
	}
}

void check_end_of_game(Game * game){

}

void progress_game_next_step(Game * game,int screen_width, int screen_height){
	if( (game->state != GS_PAUSE) && (game->b_state != BS_TIME_STOP)){
		move_shots_one_step(game);
		suppress_far_shots(game,screen_width, screen_height);
		process_shots_collisions(game);
		move_trains_one_step(game);
		check_end_of_game(game);
	}
}
//Fin progress_game_next_step

//Début Initialisation
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

void create_marbles(Track * track){
	track->marble_count = 100;
	for(int i = 0; i < track->marble_count;++i){
		track->marbles[i].t = 0;
		track->marbles[i].x = track->sample_x[0];
		track->marbles[i].y = track->sample_y[0];
		track->marbles[i].color = rand() % LAST_COLOR;
	}
	track->first_visible = track->marble_count-1;
}

void init_Track(Game * game){
	Track * track = &game->track_list.tracks[game->current_level];
	Curve_infos * ci = &game->level_list.levels[game->current_level].curve_infos;
	init_curve_infos(ci);
	char level[30];
	sprintf(level,"%d%s",game->current_level,TRACK_EXTENSION);
	if(load_curve_from_file(ci,level)< 0)
		perror("Load Impossible");
	sample_curve_to_track (&ci->curve_list.curves[ci->current_curve],track, SAMPLE_THETA);
	create_marbles(track);
	track->marbles_speed = MARBLE_SPEED;
}

void init_game(Game * game,int height, int width){
	init_canon(game,height,width);
    init_Track(game);
	game->state = GS_PLAYING;
	game->b_state = BS_NONE;
}
//Fin Initialisation

//Début Gameplay
void time_stop(Game * game){
	if(game->b_state == BS_TIME_STOP)
		game->b_state = BS_NONE;
	else if(game->b_state == BS_NONE)
		game->b_state = BS_TIME_STOP;
}

void speed_change(Game * game){
	Track * track = &game->track_list.tracks[game->current_level];
	if(game->b_state == BS_TIME_SLOWER)
		track->marbles_speed /= 2;
	else if ((game->b_state == BS_TIME_FASTER))
		track->marbles_speed *= 2;
	else if(game->b_state == BS_NONE)
		track->marbles_speed = MARBLE_SPEED;
}
//Fin Gameplay
