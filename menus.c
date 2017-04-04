#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "curve.h"
#include "game.h"
#include "mydata.h"
#include "drawings.h"
#include <errno.h>
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

void on_item_new_level_activate(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
	init_curve_infos(&my->curve_infos);
}

void on_item_load_level_activate(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    dialog = gtk_file_chooser_dialog_new ("Load Track",
                                          GTK_WINDOW(my->window),action,"Cancel",GTK_RESPONSE_CANCEL,"Open",GTK_RESPONSE_ACCEPT,NULL);
    if (my->current_folder != NULL) 
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), my->current_folder);
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
		if(load_curve_from_file(&my->curve_infos,filename)< 0){
			GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
			GtkWidget * error_dialog = gtk_message_dialog_new (GTK_WINDOW(my->window),
																flags,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Error reading “%s”: %s",filename,g_strerror (errno));
			gtk_window_set_title(GTK_WINDOW(error_dialog),"Load Track Error");
			gtk_dialog_run (GTK_DIALOG (error_dialog));
			g_signal_connect_swapped (error_dialog, "response",G_CALLBACK (gtk_widget_destroy),error_dialog);
		}	
        my->current_folder = NULL;
        my->current_folder = gtk_file_chooser_get_current_folder(chooser);    
        g_free(filename);    
    }
    gtk_widget_destroy (dialog);
}

void on_item_save_activate(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Save File",GTK_WINDOW(my->window),action,("Cancel"),GTK_RESPONSE_CANCEL,("Save"),GTK_RESPONSE_ACCEPT,NULL);
	chooser = GTK_FILE_CHOOSER (dialog);

    if (my->current_folder != NULL) 
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), my->current_folder);
    
	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	gtk_file_chooser_set_current_name (chooser,(SAVE_FILE_DEFAULT));
	  
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
		char *filename;
		filename = gtk_file_chooser_get_filename (chooser);
		save_curve_to_file (&my->curve_infos,filename);
		g_free (filename);
	  }
	my->current_folder = NULL;
	my->current_folder = gtk_file_chooser_get_current_folder(chooser);
	gtk_widget_destroy (dialog);
}

void on_item_new_game_activate(GtkWidget *widget, gpointer data){
	Mydata *my = get_mydata(data);
	init_game(&my->game,my->win_height,my->win_width);
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
				*sub_level, *sub_help, *item_about,*item_edit,*item_new_level,*item_save, *item_load_level;
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
    
    g_signal_connect (item_new_game, "activate",G_CALLBACK(on_item_new_game_activate), my); 
    g_signal_connect (item_quit, "activate",G_CALLBACK(on_item_quit_activate), my); 
    
    //Level
    sub_level = gtk_menu_new ();
    
    item_new_level = gtk_check_menu_item_new_with_label ("New");
    item_edit = gtk_check_menu_item_new_with_label ("Edit");
    item_load_level = gtk_check_menu_item_new_with_label ("Load");
    item_save = gtk_check_menu_item_new_with_label ("Save");
	
	g_signal_connect (item_new_level, "activate",G_CALLBACK(on_item_new_level_activate), my);
    g_signal_connect (item_edit, "activate",G_CALLBACK(on_item_edit_activate), my);
    g_signal_connect (item_load_level, "activate",G_CALLBACK(on_item_load_level_activate), my);   
	g_signal_connect (item_save, "activate",G_CALLBACK(on_item_save_activate), my); 
        
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_level), item_new_level);                                      
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_level), item_edit);
    gtk_menu_shell_append(GTK_MENU_SHELL(sub_level), item_load_level);
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
