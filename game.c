#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "curve.h"
#include "game.h"
#include "util.h"

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
		shot.x =  game->canon.cx + game->shot_list.shots[game->shot_list.shot_count].dx* 50;
		shot.y =  game->canon.cy + game->shot_list.shots[game->shot_list.shot_count].dy * 50;
		shot.color = game->canon.ammo1;
		game->shot_list.shots[game->shot_list.shot_count] = shot;
		game->shot_list.shot_count++;
		prepare_ammo(game);
	}
}

int is_color_on_track(Track * track, int color){
	for (int i = 0; i < track->marble_count; i++) {
		if(track->marbles[i].color == color)
			return 1;
	}
	return 0;
}

void prepare_ammo(Game * game){
	Track * track = &game->track_list.tracks[0];
	game->canon.ammo1 = game->canon.ammo2;
	do{
		game->canon.ammo2 = rand() % LAST_COLOR;
	}while((is_color_on_track(track,game->canon.ammo2) == 0));
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

int calcule_score_with_marble_group_size(Game * game,Track * track, int marble_id_start,int score,int combo){
	printf("marble_id : %d\n",marble_id_start);
	int cpt = marble_id_start + 1;
	int group_size = 1;
	int group_color = track->marbles[marble_id_start].color;
	while ((track->marbles[cpt].color == group_color) && (cpt < track->marble_count) && (track->marble_count >= 0)) {
		group_size++;
		cpt++;
	}
	cpt = marble_id_start -1;
	while ((track->marbles[cpt].color == group_color) && (cpt >= track->first_visible) && (track->marble_count >= 0) && (cpt >= 0)) {
		group_size++;
		cpt--;
	}
	if(group_size >= 3){
		if(combo == 2){
			track->state = TS_COMBO2;
		}else if(combo >= 3){
			track->state = TS_COMBO3;
		}
		printf("Boom ! %d\n", group_size);
		for(int m = (cpt + 1); m <= (cpt+group_size+1);m++){
			check_bonus(game,track,m);
		}
		memmove (track->marbles+cpt+1, track->marbles+cpt+group_size+1, sizeof(Marble)*(track->marble_count-group_size-cpt));       
		track->marble_count -= group_size;
		score += group_size * 10 * pow(2,combo);
		return calcule_score_with_marble_group_size(game,track,cpt+1,score,combo++);
	}
	return score;
}

void check_bonus(Game * game,Track * track,int marble){
	if(track->marbles[marble].bonus == BS_TIME_STOP){
		time_stop(game);
	}else if((track->marbles[marble].bonus == BS_TIME_SLOWER) || (track->marbles[marble].bonus ==BS_TIME_FASTER)){
		speed_change(game,track->marbles[marble].bonus);
	}
}

void check_bonus_end(Game * game){
	Track * track = &game->track_list.tracks[0];
	if(game->bonus.seconds <= recupTime()){
		game->bonus.b_state = BS_NONE;
		track->marbles_speed = MARBLE_SPEED;
		game->bonus.seconds = 0;
	}
}

int * get_10_best_scores(){
    FILE* file = NULL;
    int * score = (int*)calloc(10, sizeof(int)); // Tableau des 10 meilleurs scores
    file = fopen("Resources/score.sav", "r");
    if (file != NULL){
        if(fscanf(file, "%d %d %d %d %d %d %d %d %d %d", &score[0], &score[1], &score[2], &score[3], &score[4], &score[5], &score[6], &score[7], &score[8],&score[9]) == 10)
			printf("Les meilleurs scores sont : %d, %d, %d, %d, %d, %d, %d, %d, %d et %d", score[0], score[1], score[2], score[3], score[4], score[5], score[6], score[7], score[8],score[9]); 
        fclose(file);
    }
    return score;	
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
    Track * track = &game->track_list.tracks[0];
    double tb, xb , yb; 
    if ((test_collision(game, &shot_id, &marble_id, 0)) && (marble_id >= 0)) {
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
					marble_id++;
                    if (tb >= 0) {
                        track->marble_count++;
                        track->marbles[marble_id].x = xb;
                        track->marbles[marble_id].y = yb;
                        track->marbles[marble_id].t = tb;
                    }
                    track->marbles[marble_id].color = game->shot_list.shots[shot_id].color;
                    track->marbles[marble_id].bonus = BS_NONE;
                }else {
					marble_id++;
                    Marble m = track->marbles[marble_id];
                    for (int i = marble_id; i < track->marble_count; ++i) {
                            tb = compute_distant_point_forward (track->sample_x, track->sample_y, 
                                track->marbles[i].t, track->sample_count, MARBLE_DIAMETRE, &xb, &yb);
                            if (tb >= 0) {
                                track->marbles[i].x = xb;
                                track->marbles[i].y = yb;
                                track->marbles[i].t = tb;
                            }
					}
                    memmove (track->marbles+marble_id+1, track->marbles+marble_id, 
                             sizeof(Marble)*(track->marble_count-marble_id));    
                    track->marble_count++;
                    m.color = game->shot_list.shots[shot_id].color;
                    m.bonus = BS_NONE;
                    track->marbles[marble_id] = m; 
                }
            }else { //insérer derrière                   
                Marble m = track->marbles[marble_id];
                m.color = game->shot_list.shots[shot_id].color;
                m.bonus = BS_NONE;
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
			int scoreTmp = 0;
			int comboTmp = 1;
			//calcule taille groupe de même couleur et renvoie le score
			game->score += calcule_score_with_marble_group_size(game,track, marble_id,scoreTmp,comboTmp);
        }        
    }
}


void move_trains_one_step(Game * game){
	Track * track = &game->track_list.tracks[0];
	double tb,xb,yb,dx,dy,dist;
	if(game->state == GS_PLAYING){
		if(track->state == TS_COMBO2){
			
		}else if(track->state == TS_COMBO3){
			if(track->first_visible < track->marble_count){
				tb = compute_distant_point_backward (track->sample_x, track->sample_y, track->marbles[track->first_visible].t, track->sample_count, track->marbles_speed, &xb, &yb); 
				if (tb >= 0){ 
					track->marbles[track->first_visible].x = xb; 
					track->marbles[track->first_visible].y = yb; 
					track->marbles[track->first_visible].t = tb; 
				}
				dx = (track->marbles[track->first_visible].x - track->sample_x[0]); 
				dy = (track->marbles[track->first_visible].y - track->sample_y[0]);
				dx *= dx; 
				dy *= dy; 
				dist = dx + dy;
				if(dist < MARBLE_DIAMETRE2){
					track->marbles[track->first_visible].x = track->sample_x[0];
					track->marbles[track->first_visible].y = track->sample_y[0];
					track->marbles[track->first_visible].t = 0;
					track->first_visible++;
				}else{
					for(int i = track->first_visible + 1; i < track->marble_count; ++i){ 
						tb = compute_distant_point_backward (track->sample_x, track->sample_y, track->marbles[i].t, track->sample_count, track->marbles_speed, &xb, &yb); 
						if (tb >= 0){ 
							track->marbles[i].x = xb; 
							track->marbles[i].y = yb; 
							track->marbles[i].t = tb;
						}
					}
				}
			}else{
				track->state = TS_NORMAL;
			}
		}else{
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
			if(dist > MARBLE_DIAMETRE2){
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
		for (int i = 0; i < game->shot_list.shot_count; ++i){
			memmove (game->shot_list.shots+i, game->shot_list.shots+i+1, sizeof(Shot)*(game->shot_list.shot_count-1-i));
			game->shot_list.shot_count--;
        }
	}
}

void check_end_of_game(Game * game){
	if(game->state == GS_PLAYING){
		Track * track = &game->track_list.tracks[0];
		double dx, dy, dist; 
		if(track->marble_count > 0){
			dx = (track->marbles[track->marble_count - 1].x - track->sample_x[track->sample_count - 1]);
			dy = (track->marbles[track->marble_count - 1].y - track->sample_y[track->sample_count - 1]);
			dx *= dx;
			dy *= dy;
			dist = dx + dy;    
			if (dist <= MARBLE_DIAMETRE2) game->state = GS_LOST;
		}
		if((track->marble_count <= 0)&&(game->state != GS_LOST))game->state = GS_WON;
	}
		
}

void progress_game_next_step(Game * game,int screen_width, int screen_height){
	if( (game->state != GS_PAUSE)){
		check_end_of_game(game);
		check_bonus_end(game);
		move_shots_one_step(game);
		suppress_far_shots(game,screen_width, screen_height);
		if(game->bonus.b_state != BS_TIME_STOP){
			move_trains_one_step(game);
		}
		process_shots_collisions(game);
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
	load_cannon_image(game);
}

void init_ammo(Game * game){
	Track * track = &game->track_list.tracks[0];
	do{
		game->canon.ammo1 = rand() % LAST_COLOR;
	}while((is_color_on_track(track,game->canon.ammo1) == 0));
	do{
		game->canon.ammo2 = rand() % LAST_COLOR;
	}while((is_color_on_track(track,game->canon.ammo2) == 0) && (game->canon.ammo1 != game->canon.ammo2));	
}

int init_marble_bonus(){
	int isBonus = rand() % 100;
	if(isBonus < 3) // 3 pourcent de chance d'avoir un bonus d'arrêt du temps
		return BS_TIME_STOP;
	else if(isBonus < 15)
		return BS_TIME_SLOWER; // 15 pourcent de chance d'avoir un bonus de ralentissement du train
	else if(isBonus < 50)	//3 pourcent de chance d'avoir un bonus de marble explosive
		return BS_MARBLE_EXPLOSIVE;
	else
		return BS_NONE;
}

void create_marbles(Track * track,int current_level){
	track->marble_count = ( MARBLE_MAX_AT_START + (current_level * 10) ); // MARBLE_MAX_AT_START + ( niveau * 10) = nb bille par partie
	for(int i = 0; i < track->marble_count;++i){
		Marble m;
		m.t = 0;
		m.x = track->sample_x[0];
		m.y = track->sample_y[0];
		m.color = rand() % LAST_COLOR;
		m.bonus = init_marble_bonus();
		track->marbles[i] = m;
	}
	track->first_visible = track->marble_count-1;
}

void init_track(Game * game){
	Track trackInit;
	game->track_list.tracks[0] = trackInit;
	Track * track = &game->track_list.tracks[0];
	Curve_infos ci;
	init_curve_infos(&ci);
	char level[30];
	sprintf(level,"%s%s%d%s",LEVEL_DIR,TRACK_DIR,game->current_level,TRACK_EXTENSION);
	if(load_curve_from_file(&ci,level)< 0)
		perror("Load Impossible");
	sample_curve_to_track (&ci.curve_list.curves[ci.current_curve],track, SAMPLE_THETA);
	game->level_list.levels[game->current_level].curve_infos = ci;
	create_marbles(track,game->current_level);
	track->state = TS_INTRO;
	track->marbles_speed = MARBLE_SPEED;
}

void init_shots(Game * game){
	game->shot_list.shot_count = 0;
	memset(game->shot_list.shots,0,sizeof(Shot)*SHOT_MAX);
}

void init_game(Game * game,int height, int width){
	srand(time(NULL));
	init_canon(game,height,width);
	init_shots(game);
    init_track(game);
    init_ammo(game);
	game->state = GS_HELLO;
	game->bonus.b_state = BS_NONE;
	game->bonus.seconds = 0;
	game->score = 0;
	game->score_level_before = 0;
}

void reset_game(Game * game,int height, int width){
	game->current_level = 0;
	init_game(game,height,width);
	game->state = GS_PLAYING;
}

void restart_game(Game * game,int height, int width){
	int score = game->score_level_before;
	init_game(game,height,width);
	game->score = score;
	game->state = GS_PLAYING;
}

void change_level(Game * game,int height, int width){
	int score = game->score;
	if(game->current_level < LEVEL_MAX){
		game->current_level++;
	}else{
		game->current_level = 0;
	}
	init_game(game,height,width);
	game->score = score;
	game->score_level_before = score;
	game->state = GS_PLAYING;
}
//Fin Initialisation

//Début Gameplay
void time_stop(Game * game){
	game->bonus.b_state = BS_TIME_STOP;
	game->bonus.seconds = recupTime() + BONUS_TIME;
}

void speed_change(Game * game,int bonus){
	Track * track = &game->track_list.tracks[0];
	if(bonus == BS_TIME_SLOWER){
		track->marbles_speed /= 2;
		game->bonus.b_state = BS_TIME_SLOWER;
		game->bonus.seconds = recupTime() + BONUS_TIME;
	}else if ((bonus == BS_TIME_FASTER)){
		track->marbles_speed *= 2;
		game->bonus.b_state = BS_TIME_FASTER;
		game->bonus.seconds = recupTime() + BONUS_TIME;
	}
}
//Fin Gameplay
