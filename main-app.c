/**
 * \file main-app.c
 * \brief Main App
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Main App, fonctions principale
 *
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "curve.h"
#include "util.h"
#include "game.h"
#include "mydata.h"
#include "drawings.h"
#include "menus.h"
#include "gui.h"

int timeout;
//Chargement de l'appli
void on_app_activate (GtkApplication* app, gpointer user_data){
    g_object_set (gtk_settings_get_default(),"gtk-shell-shows-menubar", FALSE, NULL);
    Mydata *my = get_mydata(user_data);   
    window_init(app, my);    
    menu_init(my);  
	area_init(my);    
	status_init(my);
	editing_init(my);
	playerStatsFrame_init(my);    
	layout_init(my);               
	gtk_widget_show_all (my->window);
	gtk_widget_hide (my->frame);
	gtk_widget_hide (my->playerStatsFrame);
    timeout = g_timeout_add (20, on_timeout, my);
}

int main (int argc, char *argv[]){
    GtkApplication *app;
    setlocale(LC_ALL,getenv("LANG"));//Remet la langue de manière correcte, merci fscanf va te faire recoder !
    int status;
    Mydata my;
    init_mydata(&my);
    app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate",G_CALLBACK(on_app_activate), &my);
    status = g_application_run (G_APPLICATION(app), argc, argv);
    g_source_remove(timeout);
    g_object_unref (app);
    return status;
}
