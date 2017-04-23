#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"
#include "drawings.h"
#include "menus.h"
#include "font.h"

gboolean on_timeout (gpointer data){
	Mydata *my = get_mydata(data);
	if((my->area != NULL) && (my->levelLabel != NULL) && (my->window != NULL)){
		my->count++;
		if(my->game.state != GS_HELLO){
			if(my->show_edit == FALSE){
				progress_game_next_step(&my->game,my->win_width,my->win_height);
				if(my->game.state != GS_LOST && my->levelLabel != NULL)
					update_Player_Frame(my);
			}
			if(my->game.state == GS_LOST){
				char str[100];
				sprintf(str,"You loose... Votre score : %d",my->game.score);
				set_status(my->status, str);
				if(my->game.track_list.tracks[0].marble_count == 0){
					game_over_message_dialog(my);
				}
			}else if (my->game.state == GS_WON){
				printf("change level\n");
				change_level(&my->game,my->win_height,my->win_width);	
			}
		}
		refresh_area (my->area);
	}
	return TRUE;
}	

void apply_image_transforms (Mydata *data) {	
	Mydata *my = get_mydata(data);	
	if (my->pixbuf == NULL) return;
	GdkPixbuf *tmp = my->pixbuf;	
	my->pixbuf = gdk_pixbuf_scale_simple (tmp, my->win_width, (my->win_height - 70), GDK_INTERP_BILINEAR);
	g_object_unref (tmp);
}

// Gestion DrawingArea : refresh, scale, rotation

void draw_control_polygons (cairo_t *cr, Curve_infos *ci) {
    Curve *curve;
    for (int i = 0; i < ci->curve_list.curve_count; ++i)    {
        curve = &ci->curve_list.curves[i];
        cairo_move_to (cr, curve->controls[0].x, curve->controls[0].y);
        for (int j = 0; j < curve->control_count; ++j) {
            cairo_set_line_width (cr, 3);
            if (i == ci->current_curve)
                cairo_set_source_rgb (cr, 1.0, 1.0, 0);
            else
                cairo_set_source_rgb (cr, 0.6, 0.6, 0.6);
            cairo_line_to (cr, curve->controls[j].x, curve->controls[j].y);        
        }
        cairo_stroke (cr);    
    }
    for (int i = 0; i < ci->curve_list.curve_count; ++i)    {
        curve = &ci->curve_list.curves[i];
        for (int j = 0; j < curve->control_count; ++j) {
            cairo_set_line_width (cr, 3);
            if (i == ci->current_curve && j == ci->current_control)
                cairo_set_source_rgb (cr, 1.0, 0, 0);
            else
                cairo_set_source_rgb (cr, 0, 0, 1.0);
            cairo_rectangle (cr, curve->controls[j].x - 3.0, 
                             curve->controls[j].y - 3.0, 6.0, 6.0);
            cairo_stroke (cr);            
        }
    }
}

// Callbacks area
gboolean on_area_key_press (GtkWidget *area, GdkEvent *event, gpointer data){
    Mydata *my = get_mydata(data);
    GdkEventKey *evk = &event->key;
    printf ("%s: GDK_KEY_%s\n",    __func__, gdk_keyval_name(evk->keyval));
    if(my->game.state != GS_HELLO){
		switch (evk->keyval) {
			case GDK_KEY_q : gtk_widget_destroy(my->window); break;
			case GDK_KEY_a : set_edit_mode (my, EDIT_ADD_CURVE); break;
			case GDK_KEY_z : set_edit_mode (my, EDIT_MOVE_CURVE); break;
			case GDK_KEY_e : set_edit_mode (my, EDIT_REMOVE_CURVE); break;
			case GDK_KEY_r : set_edit_mode (my, EDIT_ADD_CONTROL); break;
			case GDK_KEY_t : set_edit_mode (my, EDIT_MOVE_CONTROL); break;
			case GDK_KEY_y : set_edit_mode (my, EDIT_REMOVE_CONTROL); break;
			case GDK_KEY_p : game_pause(&my->game); break;
			case GDK_KEY_space : swap_ammo(&my->game); break;
		}
	}else{
		my->game.state = GS_PLAYING;
	}
    return TRUE;
}

gboolean on_area_key_release (GtkWidget *area, GdkEvent *event, gpointer data){
    //GdkEventKey *evk = &event->key;
    return TRUE;
}

gboolean on_area_button_press (GtkWidget *area, GdkEvent *event, gpointer data){
    Mydata *my = get_mydata(data);     
    int n;
    GdkEventButton *evb = &event->button;
    my->click_y = evb->y;
    my->click_x = evb->x;
    my->click_n = evb->button;
    if((my->click_n == 1) && (my->show_edit == TRUE) ) {
			switch (my->edit_mode) {
				case EDIT_ADD_CURVE : 
					n = add_curve (&my->curve_infos);
					if(n < 0) break;
					set_edit_mode (my, EDIT_ADD_CONTROL);
					add_control (&my->curve_infos, my->click_x, my->click_y);
					break;
				case EDIT_MOVE_CURVE : 
					find_control (&my->curve_infos, my->click_x, my->click_y);                
					break;
				case EDIT_REMOVE_CURVE : 
					n = find_control (&my->curve_infos, my->click_x, my->click_y);
					if (n == 0) remove_curve (&my->curve_infos);

					break;
				case EDIT_ADD_CONTROL : 
					add_control (&my->curve_infos, my->click_x, my->click_y);
					break;
				case EDIT_MOVE_CONTROL : 
					find_control (&my->curve_infos, my->click_x, my->click_y);
					break;
				case EDIT_REMOVE_CONTROL : 
					n = find_control (&my->curve_infos, my->click_x, my->click_y);
					if (n == 0) remove_control (&my->curve_infos);            
					break;
			}
	}
    if ((my->click_n == 1) && (my->show_edit == FALSE) && (my->game.state == GS_PLAYING) && (evb->type == GDK_BUTTON_PRESS)){ 
		shoot_ammo(&my->game);
    }
    refresh_area(my->area);
    return TRUE;  //  ́ev ́enement trait ́e
}

gboolean on_area_button_release (GtkWidget *area, GdkEvent *event, gpointer data){
    Mydata *my = get_mydata(data);     
    //GdkEventButton *evb = &event->button;
    my->click_n = 0;
    refresh_area(my->area);
    return TRUE;  //  ́ev ́enement trait ́e
}

gboolean on_area_motion_notify (GtkWidget *area, GdkEvent *event, gpointer data){
    Mydata *my = get_mydata(data);     
    GdkEventMotion *evm = &event->motion;    
    my->last_y = my->click_y;
    my->last_x = my->click_x ;
    my->click_y = evm->y ;
    my->click_x = evm->x ;
    if (my->click_n == 1 && my->show_edit == TRUE) {
        switch (my->edit_mode) {
            case EDIT_ADD_CURVE : 
                break;
            case EDIT_MOVE_CURVE : 
                move_curve (&my->curve_infos, my->click_x - my->last_x, my->click_y - my->last_y);
                break;
            case EDIT_REMOVE_CURVE :
				break;
            case EDIT_ADD_CONTROL : 
                break;
            case EDIT_MOVE_CONTROL : 
                move_control (&my->curve_infos, my->click_x - my->last_x, my->click_y - my->last_y);
                break;
            case EDIT_REMOVE_CONTROL : 
				break;
        }
    }
    update_canon_angle(&my->game,my->click_x,my->click_y);
    refresh_area(my->area);
    return TRUE;  //  ́evenement traite
}

gboolean on_area_enter_notify (GtkWidget *area, GdkEvent *event, gpointer data){
    Mydata *my = get_mydata(data);
    gtk_widget_grab_focus (my->area);
    GdkEventCrossing *evc = &event->crossing;
    printf ("%s: %.1f %.1f\n", __func__, evc->x, evc->y);
    return TRUE;  //  ́ev ́enement trait ́e
}

gboolean on_area_leave_notify (GtkWidget *area, GdkEvent *event, gpointer data){
    GdkEventCrossing *evc = &event->crossing;
    printf ("%s: %.1f %.1f\n", __func__, evc->x, evc->y);
    return TRUE;  //  ́ev ́enement trait ́e
}

gboolean on_area_draw (GtkWidget *area, cairo_t *cr, gpointer data){    
    Mydata *my = get_mydata(data);
	PangoLayout *layout = pango_cairo_create_layout (cr);
	if((my->bsp_mode == BSP_PROLONG) && (my->show_edit == TRUE)){
		draw_control_polygons (cr, &my->curve_infos);
		draw_bezier_polygons_open(cr,&my->curve_infos);
		draw_control_labels(cr,layout,&my->curve_infos);
		draw_bezier_curves_prolong(cr,&my->curve_infos,0.1);
	}
	if(my->game.state != GS_HELLO){
		cairo_set_line_width (cr, 6);
		draw_canon(cr,&my->game);
		if (my->show_edit == FALSE){
			draw_shots(cr,&my->game);
			draw_track(cr,&my->game);
			draw_marbles(cr,&my->game);
			draw_marbles_bonus_labels(cr,&my->game,layout);
		}
	}else if(my->game.state == GS_HELLO){
		gtk_widget_hide (my->playerStatsFrame);
		if((my->pixbuf != NULL)){
			int pix_width = gdk_pixbuf_get_width(my->pixbuf);
			int pix_height = gdk_pixbuf_get_height(my->pixbuf);
			gdk_cairo_set_source_pixbuf(cr,my->pixbuf,0,0);
			cairo_rectangle (cr, 0.0, 0.0, pix_width, pix_height);
			cairo_fill (cr);
		}
		draw_title(my,cr);
	}
	g_object_unref (layout);
    return TRUE;
}

gboolean on_area_resize_window(GtkWidget *area, GdkEvent *event, gpointer data){
	Mydata *my = get_mydata(data);
	if(my->window != NULL){
		gtk_window_get_size (GTK_WINDOW(my->window), &my->win_width, &my->win_height);
		update_x_and_y_canon(&my->game,my->win_height,my->win_width);
		refresh_area(my->area);
	}
	return TRUE;
}

void draw_control_labels(cairo_t *cr, PangoLayout *layout, Curve_infos *ci){
	font_set_name(layout,"Sans, 8");
	cairo_set_source_rgb(cr,0.3,0.3,0.3);
	Curve * curve;
	for(int i = 0; i < ci->curve_list.curve_count;i++){
		curve = &ci->curve_list.curves[i];
		for(int j = 0; j < curve->control_count;j++){
			font_draw_text (cr, layout, FONT_TL,curve->controls[j].x - 3, curve->controls[j].y - 20, "%d",j);
		}
	}
	
}

void draw_bezier_polygons_open (cairo_t *cr, Curve_infos *ci)
{
    int i, j;
    Control bez_points[4];
    cairo_set_source_rgb (cr, 0, 1.0, 0);
    for (j = 0; j < ci->curve_list.curve_count ; j++){
        for (i = 0; i < (ci->curve_list.curves[j].control_count - 3) ; i++)
        {
            compute_bezier_points_open (&ci->curve_list.curves[j], i, bez_points);
            cairo_move_to (cr, bez_points[0].x, bez_points[0].y);
            cairo_line_to (cr, bez_points[1].x, bez_points[1].y);
            cairo_stroke (cr);
            cairo_move_to (cr, bez_points[2].x, bez_points[2].y);
            cairo_line_to (cr, bez_points[3].x, bez_points[3].y);
            cairo_stroke (cr);
        }
	}
}

void draw_bezier_curve(cairo_t *cr, Control bez_points[4], double theta){
    double bx[4], by[4];
    int i;
    double t;
    for (i = 0; i < 4; i++){
        bx[i] = bez_points[i].x;
        by[i] = bez_points[i].y;
	}
	cairo_move_to (cr, compute_bezier_cubic (bx, 0.0), compute_bezier_cubic (by, 0.0));
	for (t = theta; t < 1.0; t += theta){
		cairo_line_to (cr, compute_bezier_cubic (bx, t), compute_bezier_cubic (by, t));    
	}
	cairo_stroke (cr);
}

void draw_bezier_curves_open (cairo_t *cr, Curve_infos *ci, double theta){
    int i, j;
    Control bez_points[4];
    cairo_set_source_rgb (cr, 0.5, 0.0, 0.5);
    for (j = 0; j < ci->curve_list.curve_count ; j++){
		Curve *curve = &ci->curve_list.curves[j];
        for (i = 0; i < (curve->control_count - 3) ; i++)
        {
            compute_bezier_points_open (curve, i, bez_points);
            draw_bezier_curve (cr, bez_points, theta);
        }
    }
}

void draw_bezier_curves_close (cairo_t *cr, Curve_infos *ci, double theta){
    int i, j;
    Control bez_points[4];
    cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
    for (j = 0; j < ci->curve_list.curve_count ; j++){
		Curve *curve = &ci->curve_list.curves[j];
        for (i = 0; i < (curve->control_count) ; i++)
        {
            compute_bezier_points_close (curve, i, bez_points);
            draw_bezier_curve (cr, bez_points, theta);
        }
    }
}

void draw_bezier_curves_prolong (cairo_t *cr, Curve_infos *ci, double theta){
		Control bez_points[4];
		cairo_set_source_rgb (cr, 0.0, 0.5, 0.5);		
		for (int j = 0; j < ci->curve_list.curve_count; j++){
			Curve *curve = &ci->curve_list.curves[j];
			if (curve->control_count <= 3) continue;
			
		    compute_bezier_points_prolong_first (curve, bez_points);
		    draw_bezier_curve (cr, bez_points, theta);
			
			for (int i = 0; i < (curve->control_count - 3) ; i++){
				compute_bezier_points_open (curve, i, bez_points);
				draw_bezier_curve (cr, bez_points, theta);
			}
		    compute_bezier_points_prolong_last (curve, bez_points);
		    draw_bezier_curve (cr, bez_points, theta);
		}	
}

void generate_bezier_path(cairo_t *cr,Control bez_points[4],double theta,int is_first){
	double bx[4], by[4];
    int i;
    double t;
    for (i = 0; i < 4; i++){
        bx[i] = bez_points[i].x;
        by[i] = bez_points[i].y;
	}
	if(is_first){
		cairo_move_to (cr, compute_bezier_cubic (bx, 0.0), compute_bezier_cubic (by, 0.0));
	}
	for (t = theta; t < 1.0; t += theta){
		cairo_line_to (cr, compute_bezier_cubic (bx, t), compute_bezier_cubic (by, t));    
	}
}

void draw_bezier_curves_fill(cairo_t *cr, Curve_infos *ci, double theta){
	int i, j;
    Control bez_points[4];
    cairo_set_source_rgb (cr, 0.02, 0.9, 1.0);
    for (j = 0; j < ci->curve_list.curve_count ; j++){
		Curve *curve = &ci->curve_list.curves[j];
        for (i = 0; i < (curve->control_count) ; i++)
        {
            compute_bezier_points_close (curve, i, bez_points);
            if(i==0){
				generate_bezier_path (cr, bez_points, theta, TRUE);
			}else{
				generate_bezier_path (cr, bez_points, theta, FALSE);
			}
        }
        cairo_fill (cr);
    }
}

void draw_bezier_curves_clip(cairo_t *cr, Curve_infos *ci, double theta, Mydata *my){
	int i, j;
    Control bez_points[4];
    for (j = 0; j < ci->curve_list.curve_count ; j++){
		Curve *curve = &ci->curve_list.curves[j];
        for (i = 0; i < (curve->control_count) ; i++)
        {
			cairo_set_source_rgb (cr, 0.83, 0.74, 0.48);
            compute_bezier_points_close (curve, i, bez_points);
            if(i==0){
				generate_bezier_path (cr, bez_points, theta, TRUE);
			}else{
				generate_bezier_path (cr, bez_points, theta, FALSE);
			}
        }
        cairo_fill (cr);
    }
} 

void switch_shot_color(cairo_t *cr,int color){
	switch (color) {
		case RED : 
			cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
			break;
		case GREEN : 
			cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
			break;
		case BLUE : 
			cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);            
			break;
		case INDIGO : 
			cairo_set_source_rgb (cr, 0.0, 1.0, 1.0);
			break;
		case PURPLE : 
			cairo_set_source_rgb (cr, 1.0, 0.0, 1.0);
			break;
		case YELLOW : 
			cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
			break;
	}	
}

void draw_canon(cairo_t *cr, Game * game){
	cairo_save(cr);
	//Canon
	int ima_w = cairo_image_surface_get_width (game->canon.image);
	int ima_h = cairo_image_surface_get_height (game->canon.image);
	int cx = game->canon.cx - ima_w/2;
	int cy = game->canon.cy - ima_h/2;	
	cairo_translate (cr, cx + ima_w/2, cy + ima_h/2);
	cairo_rotate (cr, game->canon.angle);
	//cairo_scale (cr, 0.4, 0.4);
	cairo_translate (cr, -cx - ima_w/2, -cy - ima_h/2);
	cairo_set_source_surface (cr, game->canon.image, cx, cy);
	cairo_rectangle (cr, cx, cy, ima_w, ima_h);
	cairo_fill (cr);
	//Ammo1
	switch_shot_color(cr,game->canon.ammo1);
	cairo_arc(cr,cx + ima_w/2,cy+ ima_h/2,MARBLE_RAYON,0,2*G_PI);
	cairo_fill (cr);
	//Ammo2
	switch_shot_color(cr,game->canon.ammo2);
	cairo_arc(cr,cx + ima_w/12,cy+ ima_h/2,MARBLE_RAYON,0,2*G_PI); // divise par 12 pour positioner ammo2 sur la ceinture du canon
	cairo_fill (cr);
	cairo_restore(cr);
}

void draw_shots(cairo_t *cr, Game * game){
	for(int i =0; i < game->shot_list.shot_count;++i){
		switch_shot_color(cr,game->shot_list.shots[i].color);
		cairo_arc(cr,game->shot_list.shots[i].x,game->shot_list.shots[i].y,MARBLE_RAYON,0,2*G_PI);
		cairo_fill (cr);
	}
}

void draw_track(cairo_t *cr, Game * game){
	//Début track
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_set_line_width(cr,5);
	Track * track = &game->track_list.tracks[0];
	cairo_move_to(cr,track->sample_x[0],track->sample_y[0]);
	for(int i = 1;i < track->sample_count;++i){
		cairo_line_to(cr,track->sample_x[i],track->sample_y[i]);
	}
	cairo_stroke(cr);
	// Trou noir
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_arc(cr,track->sample_x[track->sample_count-1],track->sample_y[track->sample_count-1],BLACK_HOLE,0,2*G_PI);
	cairo_fill (cr);
}

void draw_marbles_bonus_labels(cairo_t *cr, Game * game,PangoLayout *layout){
	font_set_name(layout,"Sans, 12");
	cairo_set_source_rgb(cr,1.0,1.0,1.0);
	Track * track = &game->track_list.tracks[0];
	for(int i = track->first_visible; i < track->marble_count;++i){
		if(i >= 0)
		font_draw_text (cr, layout, FONT_TL,track->marbles[i].x-MARBLE_RAYON/2,track->marbles[i].y-MARBLE_RAYON/2, "B%d",track->marbles[i].bonus);
	}
}

void draw_marble(cairo_t *cr, Marble * marble){
	switch_shot_color(cr,marble->color);
	cairo_arc(cr,marble->x,marble->y,MARBLE_RAYON,0,2*G_PI);
	cairo_fill (cr);
}

void draw_marbles(cairo_t *cr, Game * game){
	Track * track = &game->track_list.tracks[0];
	for(int i = track->first_visible; i < track->marble_count;++i){
		if(i >= 0)
			draw_marble(cr,&track->marbles[i]);
	}
}

void draw_title (Mydata * my, cairo_t *cr){
	// Background
	if (my->pixbuf == NULL){
		char logo_Horizon[50];
		sprintf(logo_Horizon,"%s%s%s",RESOURCES_DIR,IMAGE_DIR,LOGO_HORIZON);
		g_clear_object(&my->pixbuf);
		my->pixbuf = gdk_pixbuf_new_from_file(logo_Horizon, NULL);
		if (my->pixbuf == NULL) { 
			set_status(my->status, "Loading failed : not an image.");
		}else{
			apply_image_transforms (my);
			refresh_area (my->area);
		}
	}	
	//titre-nom
	PangoLayout *layout = pango_cairo_create_layout (cr);
	font_set_name(layout,"Courier New, 30");
	font_draw_text (cr,layout,FONT_TC,(my->win_width/2),(my->win_height * ((double)1/4)), "Marble Horizon\n Gaëtan Perrot\nFLorian Duarte");
}

void update_Player_Frame(Mydata * my){
	gchar* sUtf8;
	char str[100];
	gdouble dFraction;
	Track * track = &my->game.track_list.tracks[0];
	//Début levelLabel
	sprintf(str,"<span face=\"Courier New\"><big>Level : %d</big></span>\n",my->game.current_level);
	sUtf8 = g_locale_to_utf8(str,-1, NULL, NULL, NULL);
	gtk_label_set_markup(GTK_LABEL(my->levelLabel), sUtf8);
	g_free(sUtf8);
	/* On centre le texte */
	gtk_label_set_justify(GTK_LABEL(my->levelLabel), GTK_JUSTIFY_CENTER);
	//Fin levelLabel

	//Début scoreLabel
	sprintf(str,"<span face=\"Courier New\"><big>Score : %d</big></span>\n",my->game.score);
	sUtf8 = g_locale_to_utf8(str,-1, NULL, NULL, NULL);
	gtk_label_set_markup(GTK_LABEL(my->scoreLabel), sUtf8);
	g_free(sUtf8);
	/* On centre le texte */
	gtk_label_set_justify(GTK_LABEL(my->scoreLabel), GTK_JUSTIFY_CENTER);
	//Fin scoreLabel

	dFraction = (double)track->marble_count/(MARBLE_MAX_AT_START);
	
	/* Modification de la valeur de la barre de progression */
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(my->playerProgress), dFraction);

	//Début BonusLabel
	if(my->game.bonus.b_state == BS_TIME_STOP)
		sprintf(str,"<span face=\"Courier New\"><big>Bonus : %s</big></span>\n","Time Stop");
	else if(my->game.bonus.b_state == BS_TIME_SLOWER)
		sprintf(str,"<span face=\"Courier New\"><big>Bonus : %s</big></span>\n","Time Slower");
	else
		sprintf(str,"<span face=\"Courier New\"><big>Bonus : %s</big></span>\n","Aucun");
	sUtf8 = g_locale_to_utf8(str,-1, NULL, NULL, NULL);
	gtk_label_set_markup(GTK_LABEL(my->bonusLabel), sUtf8);
	g_free(sUtf8);
	/* On centre le texte */
	gtk_label_set_justify(GTK_LABEL(my->bonusLabel), GTK_JUSTIFY_CENTER);
	//Fin BonusLabel
}

void game_over_message_dialog(gpointer data){
	Mydata *my = get_mydata(data);
    GtkWidget *pQuestion;
    pQuestion = gtk_message_dialog_new (GTK_WINDOW(my->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Game Over !\nVotre score : %d !\nVoulez vous reprendre au début du niveau ?",my->game.score);
    gtk_window_set_title(GTK_WINDOW(pQuestion),"Game Over !");
    switch(gtk_dialog_run(GTK_DIALOG(pQuestion))){
        case GTK_RESPONSE_YES:
            restart_game(&my->game,my->win_height,my->win_width);
            gtk_widget_destroy(pQuestion);
            break;
        case GTK_RESPONSE_NO:
			my->game.state = GS_HELLO;
            gtk_widget_destroy(pQuestion);
            break;
    }
} 

void area_init (gpointer user_data){
    Mydata *my = get_mydata(user_data);                                    
    my->area = gtk_drawing_area_new ();    
	g_signal_connect (my->area, "draw", G_CALLBACK (on_area_draw), my);    
    g_signal_connect (my->area, "key-press-event", G_CALLBACK (on_area_key_press), my);
    g_signal_connect (my->area, "key-release-event", G_CALLBACK (on_area_key_release), my);
    g_signal_connect (my->area, "button-press-event", G_CALLBACK (on_area_button_press), my);
    g_signal_connect (my->area, "button-release-event", G_CALLBACK (on_area_button_release), my);
    g_signal_connect (my->area, "motion-notify-event", G_CALLBACK (on_area_motion_notify), my);
    g_signal_connect (my->area, "enter-notify-event", G_CALLBACK (on_area_enter_notify), my);
    g_signal_connect (my->area, "leave-notify-event", G_CALLBACK (on_area_leave_notify), my);
    g_signal_connect(my->area, "size-allocate",G_CALLBACK(on_area_resize_window), my); 


    gtk_widget_add_events  (my->area,GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
                            GDK_FOCUS_CHANGE_MASK |
                            GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
                            GDK_POINTER_MOTION_MASK |
                            GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK );
    
    gtk_widget_set_can_focus (my->area, TRUE);
}
