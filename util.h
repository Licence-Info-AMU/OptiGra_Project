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

/**
 * \fn void refresh_area (GtkWidget *area)
 * \brief Fonction de rafraichissement de l'area
 *
 * \param self Un objet GtkWidget notre area
 * \return void
 */
void refresh_area (GtkWidget *area);

/**
 * \fn void set_status (GtkWidget *status, const char *format, ...)
 * \brief Fonction pour mettre à jour la barre de status
 *
 * \param self Un objet GtkWidget notre barre de status, une chaine, et des paramètres optionnels pour construire la chaine
 * \return void
 */
void set_status (GtkWidget *status, const char *format, ...);

/**
 * \fn int recupTime()
 * \brief Fonction de récupération du l'heure en seconde
 *
 * \param self
 * \return int l'heure actuelle en seconde
 */
int recupTime();

#endif /* UTIL_H */
