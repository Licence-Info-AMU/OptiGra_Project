/**
 * \file gui.c
 * \brief Interface graphique
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Interface graphique
 *
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"
#include "drawings.h"

void on_radio_toggled (GtkWidget *widget, gpointer data) {
    Mydata *my = get_mydata(data);
    gint mode= GPOINTER_TO_INT(g_object_get_data (G_OBJECT(widget), "mode"));
    my->edit_mode = mode;
}

void on_bsp_radio_toggled(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
	gint mode= GPOINTER_TO_INT(g_object_get_data (G_OBJECT(widget), "mode"));
	my->bsp_mode = mode;
	refresh_area (my->area);
}

/**
 * \fn void window_init (GtkApplication* app, gpointer user_data)
 * \brief Fonction d'initialisation de la fenêtre
 *
 * \param self GtkApplication L'application, gpointer objet Mydata
 * \return void
 */
void window_init (GtkApplication* app, gpointer user_data){
    Mydata *my = get_mydata(user_data);
    my->window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (my->window), my->title);
    gtk_window_set_default_size (GTK_WINDOW (my->window), my->win_width, my->win_height);
}

/**
 * \fn void editing_init (Mydata *data)
 * \brief Fonction d'initialisation de la frame d'edition
 *
 * \param self Objet Mydata
 * \return void
 */
void editing_init (Mydata *data) {
    Mydata *my = get_mydata(data);
    GtkWidget *vbox2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 1);
	GtkWidget *separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
    my->frame = gtk_frame_new ("Editing");
    char *edit_radios_noms[6] = {"Add curve", "Move curve", "Remove curve", "Add control", 
                    "Move control", "Remove control"};  
    char *bsp_radios_noms[1] = {"Prolongated"};
    
    for (int i = 0; i < 6; i++) {
        my->edit_radios[i] =  gtk_radio_button_new_with_label_from_widget (
                    (i == 0) ? NULL : GTK_RADIO_BUTTON(my->edit_radios[0]), edit_radios_noms[i]);
        g_object_set_data (G_OBJECT(my->edit_radios[i]), "numero", GINT_TO_POINTER(i));
        g_object_set_data (G_OBJECT(my->edit_radios[i]), "mode",  GINT_TO_POINTER(i + 1));
        g_signal_connect (my->edit_radios[i], "toggled", G_CALLBACK(on_radio_toggled), my);  
        gtk_box_pack_start (GTK_BOX (vbox2), my->edit_radios[i], FALSE, FALSE, 0); 
    }
    
	gtk_container_add (GTK_CONTAINER (vbox2), separator);
	
	for (int j = 0; j < 1; j++) {
		my->bsp_radios[j] =  gtk_radio_button_new_with_label_from_widget (
				(j == 0) ? NULL : GTK_RADIO_BUTTON(my->bsp_radios[0]), bsp_radios_noms[j]);
		g_object_set_data (G_OBJECT(my->bsp_radios[j]), "mode",  GINT_TO_POINTER(j + 1));
		g_signal_connect (my->bsp_radios[j], "toggled", G_CALLBACK(on_bsp_radio_toggled), my);
        gtk_box_pack_start (GTK_BOX (vbox2), my->bsp_radios[j], FALSE, FALSE, 0);
	}
	
    gtk_container_add (GTK_CONTAINER (my->frame), vbox2);
}

/**
 * \fn void playerStatsFrame_init(Mydata *data)
 * \brief Fonction d'initialisation de la frame des statistiques du joueur
 *
 * \param self Objet Mydata
 * \return void
 */
void playerStatsFrame_init(Mydata *data){
	Mydata *my = get_mydata(data);
	//Début Déclaration
	GtkWidget *vbox2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 1);
    my->playerStatsFrame = gtk_frame_new ("Player Stats");
    my->levelLabel = gtk_label_new(NULL);
    my->scoreLabel = gtk_label_new(NULL);
    GtkWidget *progressionLabel = gtk_label_new(NULL);
    my->bonusLabel = gtk_label_new(NULL);
	gchar* sUtf8;
	//Fin Déclaration
	
    gtk_box_pack_start (GTK_BOX (vbox2), my->levelLabel, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox2), my->scoreLabel, FALSE, FALSE, 0);
    
	/* Création de la barre de progression */
    //Début progressionLabel
    sUtf8 = g_locale_to_utf8("<span face=\"Courier New\"><big>Progression</big></span>\n",-1, NULL, NULL, NULL);
    gtk_label_set_markup(GTK_LABEL(progressionLabel), sUtf8);
    g_free(sUtf8);
    /* On centre le texte */
    gtk_label_set_justify(GTK_LABEL(progressionLabel), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start (GTK_BOX (vbox2), progressionLabel, FALSE, FALSE, 0);
    //Fin progressionLabel
    
	my->playerProgress = gtk_progress_bar_new();
	gtk_box_pack_start(GTK_BOX(vbox2), my->playerProgress, FALSE, FALSE, 0);
	
	//Zone Bonus
	gtk_box_pack_start (GTK_BOX (vbox2), my->bonusLabel, FALSE, FALSE, 0);
	
    //Affichage update
    update_Player_Frame(my);
    
    gtk_container_add (GTK_CONTAINER (my->playerStatsFrame), vbox2);
}

/**
 * \fn void layout_init (gpointer user_data)
 * \brief Fonction d'initialisation du layout
 *
 * \param self gpointer Objet Mydata
 * \return void
 */
void layout_init (gpointer user_data){
    Mydata *my = get_mydata(user_data);
    my->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
    my->hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_container_add (GTK_CONTAINER (my->window), my->vbox);
    my->scroll = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_add (GTK_CONTAINER (my->scroll), my->area);

    //Menu

    gtk_box_pack_start (GTK_BOX (my->vbox), my->menu_bar, FALSE, FALSE, 0);

    gtk_box_pack_start (GTK_BOX (my->hbox), my->frame, FALSE, FALSE, 2);

    gtk_box_pack_start (GTK_BOX (my->hbox), my->scroll, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (my->vbox), my->hbox, TRUE, TRUE, 0);
    
    gtk_box_pack_start (GTK_BOX (my->hbox), my->playerStatsFrame, FALSE, FALSE, 0);

    //Status
    gtk_box_pack_start (GTK_BOX (my->vbox), my->status, FALSE, FALSE, 0);    
}

/**
 * \fn void status_init (gpointer user_data)
 * \brief Fonction d'initialisation de la barre de status
 *
 * \param self gpointer Objet Mydata
 * \return void
 */
void status_init (gpointer user_data){
    Mydata *my = get_mydata(user_data);        
    my->status = gtk_statusbar_new();
    set_status(my->status, "Game Init !");     
}
