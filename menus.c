#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"
#include "drawings.h"
#include "menus.h"

// Callbacks menus / boutons

void on_item_about_activate (GtkWidget *widget, gpointer data){
    Mydata *my = get_mydata(data);
    char *auteurs[] = {"Florian Duarte <florian.duarte@etu.univ-amu.fr>","Gaetan Perrot <gaetan.perrot@etu.univ-amu.fr>", NULL};
    gtk_show_about_dialog (NULL, "program-name", my->title, "version", 
                           "0.1", "website", "https://github.com/moonlight83340/OptiGra_Project", 
                           "authors", auteurs, "logo-icon-name", 
                           "face-wink",NULL);    
    printf ("About\n");
}

void on_item_quit_activate (GtkWidget *widget, gpointer data){
    Mydata *my = get_mydata(data);
    gtk_widget_destroy(my->window);
}

void on_item_edit_activate (GtkCheckMenuItem *widget, gpointer data){
    Mydata *my = get_mydata(data);    
    
    my->show_edit = gtk_check_menu_item_get_active (widget);
    if (my->show_edit == TRUE) {
        set_status(my->status, "Editing is on");
        gtk_widget_show (my->frame);
    }
    else {
        set_status(my->status, "Editing is off");
        gtk_widget_hide (my->frame);
    }
        
    refresh_area (my->area);
}

void menu_init (gpointer user_data){
	Mydata *my = get_mydata(user_data);  
    GtkWidget *item_game, *item_level, *item_help, 
				*sub_game, *item_start, *item_pause, *item_new_game, *item_quit, 
				*sub_level, *sub_help, *item_about,*item_edit,*item_new_level,*item_save; 
    my->menu_bar = gtk_menu_bar_new();    
    //Menu    
    item_game = gtk_menu_item_new_with_label ("Game");
    item_level = gtk_menu_item_new_with_label ("Level");
    item_help = gtk_menu_item_new_with_label ("Help");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(my->menu_bar), item_game);
    gtk_menu_shell_append(GTK_MENU_SHELL(my->menu_bar), item_level);
    gtk_menu_shell_append(GTK_MENU_SHELL(my->menu_bar), item_help);
    
    //Game
    sub_game = gtk_menu_new ();
    
    item_new_game = gtk_menu_item_new_with_label ("New");
    item_start = gtk_menu_item_new_with_label ("Start");
    item_pause = gtk_menu_item_new_with_label ("Pause");
    item_quit = gtk_menu_item_new_with_label ("Quit");
           
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_game), item_new_game);                     
	gtk_menu_shell_append(GTK_MENU_SHELL(sub_game), item_start);
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_game), item_pause);
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_game), item_quit);
    
    //Level
    sub_level = gtk_menu_new ();
    
    item_edit = gtk_check_menu_item_new_with_label ("Edit");
    item_new_level = gtk_check_menu_item_new_with_label ("New");
    item_save = gtk_check_menu_item_new_with_label ("Save");

    g_signal_connect (item_edit, "activate",G_CALLBACK(on_item_edit_activate), my);                   
                                              
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_level), item_edit);
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_level), item_new_level);
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_level), item_save);
    
    //Help
    sub_help = gtk_menu_new ();
    
    item_about = gtk_menu_item_new_with_label ("About");
    g_signal_connect (item_about, "activate",G_CALLBACK(on_item_about_activate), my);  
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_help), item_about);
    
    //Sub-menus    
    gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_game), sub_game);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_level), sub_level);
    gtk_menu_item_set_submenu (GTK_MENU_ITEM(item_help), sub_help);    
}
