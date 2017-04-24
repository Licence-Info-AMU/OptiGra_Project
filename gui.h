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

/**
 * \fn void window_init (GtkApplication* app, gpointer user_data)
 * \brief Fonction d'initialisation de la fenêtre
 *
 * \param self GtkApplication L'application, gpointer objet Mydata
 * \return void
 */
void window_init (GtkApplication* app, gpointer user_data);

/**
 * \fn void editing_init (Mydata *data)
 * \brief Fonction d'initialisation de la frame d'edition
 *
 * \param self Objet Mydata
 * \return void
 */
void editing_init (Mydata *data);

/**
 * \fn void playerStatsFrame_init(Mydata *data)
 * \brief Fonction d'initialisation de la frame des statistiques du joueur
 *
 * \param self Objet Mydata
 * \return void
 */
void playerStatsFrame_init(Mydata *data);

/**
 * \fn void layout_init (gpointer user_data)
 * \brief Fonction d'initialisation du layout
 *
 * \param self gpointer Objet Mydata
 * \return void
 */
void layout_init (gpointer user_data);

/**
 * \fn void status_init (gpointer user_data)
 * \brief Fonction d'initialisation de la barre de status
 *
 * \param self gpointer Objet Mydata
 * \return void
 */
void status_init (gpointer user_data);

#endif// GUI_H
