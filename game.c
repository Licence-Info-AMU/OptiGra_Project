#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
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
		game->shot_list.shots[game->shot_list.shot_count].dx = cos(game->canon.angle);
		game->shot_list.shots[game->shot_list.shot_count].dy = sin(game->canon.angle);
		game->shot_list.shots[game->shot_list.shot_count].x =  game->canon.cx + game->shot_list.shots[game->shot_list.shot_count].dx* 50;
		game->shot_list.shots[game->shot_list.shot_count].y =  game->canon.cy + game->shot_list.shots[game->shot_list.shot_count].dy * 50;
		game->shot_list.shots[game->shot_list.shot_count].color = game->canon.ammo1;
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

void do_vector_product(double xu,double yu,double zu,double xv,double yv,double zv, double *x, double *y,double *z){
	*x = yu*zv-yv*zu;
	*y = xv*zu-xu*zv;
	*z = xu*yv-xv*yu;
}

int test_collision (Game * game, int *shot_num, int *marble_num, int track_num) {
    int shot = 0, marble = 0;
    double dx, dy, dist2;
    for (shot = 0; shot < game->shot_list.shot_count; ++shot){    
        for (marble = game->track_list.tracks[track_num].first_visible; marble < game->track_list.tracks[track_num].marble_count; ++marble) {
            dx = (game->track_list.tracks[track_num].marbles[marble].x
                         - game->shot_list.shots[shot].x);
            dy = (game->track_list.tracks[track_num].marbles[marble].y
                         - game->shot_list.shots[shot].y);
            dx *= dx;
            dy *= dy;
            dist2 = dx + dy;    
            if (dist2 < MARBLE_DIAMETRE2) {
                *shot_num = shot;
                *marble_num = marble;
                return 1;
            }        
        }
    }
    return 0;
}

void process_shots_collisions (Game * game) {
    int shot_id = 0, marble_id = 0 ;
    Track * track = &game->track_list.tracks[game->current_level];
    double tb, xb , yb; 
    if (test_collision(game, &shot_id, &marble_id, game->current_level)) {
        double sx, sy, sxb, syb, sa, sb, sc;
        double mx, my, mxb, myb, ma, mb, mc;
        double ia, ib, ic, ix, iy, p;
        sx = game->shot_list.shots[shot_id].x;
        sy = game->shot_list.shots[shot_id].y;
        sxb = sx + game->shot_list.shots[shot_id].dx * SHOT_SPEED;
        syb = sy + game->shot_list.shots[shot_id].dy * SHOT_SPEED;
        mx = track->marbles[marble_id].x;
        my = track->marbles[marble_id].y;
        tb = compute_distant_point_forward (track->sample_x, track->sample_y, 
            track->marbles[marble_id].t, track->sample_count, 1, &mxb, &myb);
        do_vector_product (sx, sy, 1, sxb, syb, 1, &sa, &sb, &sc);            
        do_vector_product (mx, my, 1, mxb, myb, 1, &ma, &mb, &mc);
        do_vector_product (sa, sb, sc, ma, mb, mc, &ia, &ib, &ic);
        if (ic != 0) {
            ix = ia / ic;
            iy = ib / ic;
            if (mx != mxb)
                p = (ix - mx) / (mxb - mx);
            else
                p = (iy - my) / (myb - my);
            if (p >= 0) { // insérer devant
                if (marble_id == track->marble_count - 1) {
                    tb = compute_distant_point_forward (track->sample_x, track->sample_y, 
                        track->marbles[marble_id].t, track->sample_count, MARBLE_DIAMETRE, &xb, &yb);
                    if (tb >= 0) {
                        track->marble_count++;
                        track->marbles[marble_id + 1].x = xb;
                        track->marbles[marble_id + 1].y = yb;
                        track->marbles[marble_id + 1].t = tb;
                    }
                    track->marbles[marble_id + 1].color = game->shot_list.shots[shot_id].color;
                }else {
                    Marble m = track->marbles[marble_id + 1];

                    for (int i = marble_id + 1; i < track->marble_count; ++i) {
                            tb = compute_distant_point_forward (track->sample_x, track->sample_y, 
                                track->marbles[i].t, track->sample_count, MARBLE_DIAMETRE, &xb, &yb);
                            if (tb >= 0) {
                                track->marbles[i].x = xb;
                                track->marbles[i].y = yb;
                                track->marbles[i].t = tb;
                            }                    
                        }
                    memmove (track->marbles+marble_id+2, track->marbles+marble_id+1, 
                             sizeof(Marble)*(track->marble_count-marble_id));    
                    track->marble_count++;
                    m.color = game->shot_list.shots[shot_id].color;
                    track->marbles[marble_id + 1] = m; 
                }
            }else { //insérer derrière                    
                Marble m = track->marbles[marble_id];
                m.color = game->shot_list.shots[shot_id].color;
                for (int i = marble_id; i < track->marble_count; ++i) {
                    tb = compute_distant_point_forward (track->sample_x, track->sample_y, 
                        track->marbles[i].t, track->sample_count, MARBLE_DIAMETRE, &xb, &yb);
                    if (tb >= 0) {
                        track->marbles[i].x = xb;
                        track->marbles[i].y = yb;
                        track->marbles[i].t = tb;
                    }                    
                }
                track->marble_count++;
                memmove (track->marbles+marble_id+1, track->marbles+marble_id, 
                         sizeof(Marble)*(track->marble_count-1-marble_id));    
                track->marbles[marble_id] = m; 
            }
			memmove (game->shot_list.shots+shot_id, game->shot_list.shots+shot_id+1, sizeof(Shot)*(game->shot_list.shot_count-1-shot_id));
			game->shot_list.shot_count--;
            //calcule taille groupe de même couleur
			int cpt = marble_id + 1;
			int group_size = 1;
			int group_color = track->marbles[marble_id].color;
			while (track->marbles[cpt].color == group_color && cpt < track->marble_count) {
				group_size++;
				cpt++;
			}        
			cpt = marble_id - 1;
			while (track->marbles[cpt].color == group_color && cpt >= track->first_visible) {
				group_size++;
				cpt--;
			}
			//Fin calcule taille groupe de même couleur
			printf("GroupSize ! %d\n", group_size);
			if(group_size >= 3){
				printf("Boom ! %d\n", group_size);
				memmove (track->marbles+cpt+1, track->marbles+cpt+group_size+1, sizeof(Marble)*(track->marble_count-group_size-cpt));        
				track->marble_count -= group_size;
				game->score += group_size * 10;
			}
        }        
    }
}

void move_trains_one_step(Game * game){
	Track * track = &game->track_list.tracks[game->current_level];
	double tb,xb,yb,dx,dy,dist;
	if(game->state == GS_PLAYING){
		tb = compute_distant_point_forward (track->sample_x, track->sample_y, track->marbles[track->first_visible].t, track->sample_count, track->marbles_speed, &xb, &yb);
		if(tb >= 0){
			track->marbles[track->first_visible].x = xb;
			track->marbles[track->first_visible].y = yb;
			track->marbles[track->first_visible].t = tb;
		}
		dx = track->marbles[track->first_visible].x - track->sample_x[0];
		dy = track->marbles[track->first_visible].y - track->sample_y[0];
		dx *= dx;
		dy *= dy;
		dist = dx + dy;
		if((dist > (MARBLE_DIAMETRE2))){
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
			dx = (track->marbles[i].x - track->marbles[i - 1].x);            
            dy = (track->marbles[i].y - track->marbles[i - 1].y);            
            dx *= dx;
            dy *= dy;
            dist = dx + dy; 
			if((tb >= 0) && (dist < MARBLE_DIAMETRE2)){
				track->marbles[i].x = xb;
				track->marbles[i].y = yb;
				track->marbles[i].t = tb;
			}else{
				break;
			}
		}
	}else if(game->state == GS_LOST){
		track->marbles_speed = MARBLE_SPEED_END_GAME;
		for(int i = track->first_visible; i < track->marble_count;++i){
			tb = compute_distant_point_forward (track->sample_x, track->sample_y, track->marbles[i].t, track->sample_count, track->marbles_speed, &xb, &yb);
			if(tb >= 0){
				track->marbles[i].x = xb;
				track->marbles[i].y = yb;
				track->marbles[i].t = tb;
			}else{
				track->marble_count--;
			}
		}
	}
}

void check_end_of_game(Game * game){
	Track * track = &game->track_list.tracks[game->current_level];
    double dx, dy, dist; 
    if(track->marble_count > 0){
		dx = (track->marbles[track->marble_count - 1].x - track->sample_x[track->sample_count - 1]);
		dy = (track->marbles[track->marble_count - 1].y - track->sample_y[track->sample_count - 1]);
		dx *= dx;
		dy *= dy;
		dist = dx + dy;    
		if (dist <= MARBLE_DIAMETRE2) game->state = GS_LOST;
	}else{
		game->state = GS_WON;
	}
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
	track->marble_count = 60;
	for(int i = 0; i < track->marble_count;++i){
		track->marbles[i].t = 0;
		track->marbles[i].x = track->sample_x[0];
		track->marbles[i].y = track->sample_y[0];
		track->marbles[i].color = rand() % LAST_COLOR;
	}
	track->first_visible = track->marble_count-1;
}

void init_track(Game * game){
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

void init_shots(Game * game){
	game->shot_list.shot_count = 0;
}

void init_game(Game * game,int height, int width){
	srand(time(NULL));
	init_canon(game,height,width);
	init_shots(game);
    init_track(game);
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