/**
 * \file gui.h
 * \brief Interface graphique
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Interface graphique
 *
 */

#ifndef GUI_H
#define GUI_H

void window_init (GtkApplication* app, gpointer user_data);

void editing_init (Mydata *data);

void playerStatsFrame_init(Mydata *data);

void layout_init (gpointer user_data);

void status_init (gpointer user_data);

#endif// GUI_H
