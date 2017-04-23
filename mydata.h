/**
 * \file mydata.h
 * \brief Mydata structure avec les objets principaux
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Mydata structure avec les objets principaux
 *
 */

#ifndef MYDATA_H
#define MYDATA_H

enum const_edit{EDIT_NONE,EDIT_ADD_CURVE,EDIT_MOVE_CURVE,EDIT_REMOVE_CURVE,EDIT_ADD_CONTROL,EDIT_MOVE_CONTROL,EDIT_REMOVE_CONTROL,EDIT_LAST};
enum const_mode{BSP_FIRST,BSP_PROLONG,BSP_LAST};

typedef struct {
    GtkWidget *window;
    GtkWidget *status;
    GtkWidget *vbox;
    GtkWidget *area;
    GtkWidget *scroll;
    GtkWidget *menu_bar;
    GtkWidget *frame;
    GtkWidget *playerStatsFrame;
	GtkWidget *levelLabel;
    GtkWidget *scoreLabel;
    GtkWidget *playerProgress;
    GtkWidget *bonusLabel;
    GtkWidget *hbox;
    GtkWidget * add_curve;
	GtkWidget * move_curve;
	GtkWidget * remove_curve;
	GtkWidget * add_control;
	GtkWidget * move_control;
	GtkWidget * remove_control;
	GtkWidget *vertical;
	GtkWidget *edit_radios[EDIT_LAST];
	GtkWidget *bsp_radios[BSP_LAST];
	GdkPixbuf *pixbuf;
	double click_x,click_y,last_x, last_y;
    int win_width,win_height;
    int show_edit;
    int edit_mode;
	int click_n;
    int bsp_mode;
    int count;
    unsigned int magic;
    char * current_folder;
    char *title;
    Curve_infos curve_infos;
    Game game;
} Mydata;

Mydata *get_mydata (gpointer data);

void init_mydata(Mydata *my);

void set_edit_mode (Mydata *data, int mode);

#endif /* MYDATA_H */
