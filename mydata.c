#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "curve.h"
#include "game.h"
#include "mydata.h"

#define MYDATA_MAGIC 0x46EA7E05

// Cette fonction permet de tester si le data que l’on a recuperé dans
// une callback contient bien my ; sinon, cela veut dire que :
//  - soit on a oublié de transmettre my dans g_signal_connect,
//  - soit on s’est trompé dans le nombre de paramètres de la callback.

Mydata *get_mydata (gpointer data){
    if (data == NULL) {
        fprintf (stderr, "get_mydata: NULL data\n"); return NULL;
    }
    if (((Mydata *)data)->magic != MYDATA_MAGIC) {
        fprintf (stderr, "get_mydata: bad magic number\n"); return NULL;
    }
    return data;
}

void init_mydata(Mydata *my){
    my->title = "Marble Horizon";
    my->win_width = 1280;
    my->win_height = 920;
    my->magic = MYDATA_MAGIC;
    my->current_folder = NULL;
    my->pixbuf = NULL;
    my->scale_horizon_value = 1.0;
    my->scale_horizon = NULL;
	my->pixbuf2 = NULL;
    my->rotate_angle = 0.0;
	my->click_x = 0.0f;
    my->click_y = 0.0f;
    my->click_n = 0;
	my->last_x = 0.0f;
    my->last_y = 0.0f;    
    my->show_edit = FALSE;
    my->edit_mode = EDIT_ADD_CURVE;
    my->bsp_mode = BSP_PROLONG;
    init_curve_infos(&my->curve_infos);
    init_game(&my->game,my->win_height,my->win_width);
    init_Track(&my->curve_infos,&my->game);
    my->count = 0;
}

// Gestion de l'édition : Curves / Controls
void set_edit_mode (Mydata *data, int mode) {
    Mydata *my = get_mydata(data);
    if (mode > EDIT_NONE || mode < EDIT_LAST) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (my->edit_radios[mode-1]), TRUE);
    }
}
