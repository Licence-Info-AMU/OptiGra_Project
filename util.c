/**
 * \file util.c
 * \brief Fonctions utiles
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Fonctions utiles
 *
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include "util.h"

/**
 * \fn void refresh_area (GtkWidget *area)
 * \brief Fonction de rafraichissement de l'area
 *
 * \param self Un objet GtkWidget notre area
 * \return void
 */
void refresh_area (GtkWidget *area){
    GdkWindow *win = gtk_widget_get_window (area);
    if (win == NULL) return;
    gdk_window_invalidate_rect (win, NULL, FALSE);
}

/**
 * \fn void set_status (GtkWidget *status, const char *format, ...)
 * \brief Fonction pour mettre à jour la barre de status
 *  Mise à jour de la barre de statut
 * \param self Un objet GtkWidget notre barre de status, une chaine, et des paramètres optionnels pour construire la chaine
 * \return void
 */
void set_status (GtkWidget *status, const char *format, ...)
{
	char buf[1000];
	va_list ap;
	va_start (ap, format);
	vsnprintf (buf, sizeof(buf)-1, format, ap);
	va_end (ap);
	buf[sizeof(buf)-1] = 0;
	gtk_statusbar_pop  (GTK_STATUSBAR(status), 0);
	gtk_statusbar_push (GTK_STATUSBAR(status), 0, buf);
}

/**
 * \fn int recupTime()
 * \brief Fonction de récupération du l'heure en seconde
 *
 * \param self
 * \return int l'heure actuelle en seconde
 */
int recupTime(){
  time_t timer1;
  int secondes, minutes, heures;
  struct tm *newTime1;
  time(&timer1);
  newTime1 = localtime(&timer1);
  heures = newTime1->tm_hour*3600;
  minutes = newTime1->tm_min*60;
  secondes = (newTime1->tm_sec + heures + minutes);	
  return secondes;
}
