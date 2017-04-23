/**
 * \file util.h
 * \brief Fonctions utiles
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Fonctions utiles
 *
 */

#ifndef UTIL_H
#define UTIL_H

#define SAVE_FILE_DEFAULT "Untitled.track"

void refresh_area (GtkWidget *area);

void set_status (GtkWidget *status, const char *format, ...);

int recupTime();

#endif /* UTIL_H */
