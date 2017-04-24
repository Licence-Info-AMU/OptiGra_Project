/**
 * \file mydata.h
 * \brief Mydata structure avec les objets principaux
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Mydata structure avec les objets principaux
 *
 */

#ifndef MYDATA_H
#define MYDATA_H

/**
 * \enum const_edit
 * \brief Constantes du choix du mode d'edition.
 *
 * const_edit est une série de constantes prédéfinie pour les choix du mode d'edition
 */
enum const_edit{EDIT_NONE,EDIT_ADD_CURVE,EDIT_MOVE_CURVE,EDIT_REMOVE_CURVE,EDIT_ADD_CONTROL,EDIT_MOVE_CONTROL,EDIT_REMOVE_CONTROL,EDIT_LAST};

/**
 * \enum const_mode
 * \brief Constantes du choix du mode de création des courbes.
 *
 * const_mode est une série de constantes prédéfinie pour les choix du mode de création des courbes
 */
enum const_mode{BSP_FIRST,BSP_PROLONG,BSP_LAST};

/**
 * \struct Mydata
 * \brief Objet contenant les informations pour un Mydata
 *
 * Objet contenant les informations pour un Mydata
 * GtkWidget *window;
 * GtkWidget *status;
 * GtkWidget *vbox;
 * GtkWidget *area;
 * GtkWidget *scroll;
 * GtkWidget *menu_bar;
 * GtkWidget *frame;
 * GtkWidget *playerStatsFrame;
 * GtkWidget *levelLabel;
 * GtkWidget *scoreLabel;
 * GtkWidget *playerProgress;
 * GtkWidget *bonusLabel;
 * GtkWidget *hbox;
 * GtkWidget * add_curve;
 * GtkWidget * move_curve;
 * GtkWidget * remove_curve;
 * GtkWidget * add_control;
 * GtkWidget * move_control;
 * GtkWidget * remove_control;
 * GtkWidget *vertical;
 * GtkWidget *edit_radios[EDIT_LAST];
 * GtkWidget *bsp_radios[BSP_LAST];
 * GdkPixbuf *pixbuf;
 * double click_x;
 * double click_y;
 * double last_x;
 * double last_y;
 * int win_width;
 * int win_height;
 * int show_edit;
 * int edit_mode;
 * int click_n;
 * int bsp_mode;
 * int count;
 * unsigned int magic;
 * char * current_folder;
 * char *title;
 * Curve_infos curve_infos;
 * Game game;
 */
typedef struct {
    GtkWidget *window;					/*!< Fenêtre principale. */ 
    GtkWidget *status;					/*!< Barre de Status. */ 
    GtkWidget *vbox;					/*!< Vertical box. */ 
    GtkWidget *area;					/*!< Area pour dessiner. */ 
    GtkWidget *scroll;					/*!< Scroll bar. */ 
    GtkWidget *menu_bar;				/*!< Barre menu. */ 
    GtkWidget *frame;					/*!< Edit frame. */ 
    GtkWidget *playerStatsFrame;		/*!< Player stats frame. */ 
	GtkWidget *levelLabel;				/*!< Level label. */ 
    GtkWidget *scoreLabel;				/*!< Score label. */ 
    GtkWidget *playerProgress;			/*!< Barre de progression. */ 
    GtkWidget *bonusLabel;				/*!< Bonus label. */ 
    GtkWidget *hbox;					/*!< Horizontal box. */ 
    GtkWidget * add_curve;				
	GtkWidget * move_curve;				
	GtkWidget * remove_curve;			
	GtkWidget * add_control;			
	GtkWidget * move_control;				
	GtkWidget * remove_control;			
	GtkWidget *vertical;					
	GtkWidget *edit_radios[EDIT_LAST];	/*!< Tableau bouton radios pour l'edition. */ 
	GtkWidget *bsp_radios[BSP_LAST];	/*!< Tableau bouton radios pour bsp. */ 
	GdkPixbuf *pixbuf;					/*!< Pixbuf : background. */ 	
	double click_x;						/*!< coordonnÃ©es click x. */ 
	double click_y;						/*!< coordonnÃ©es click y. */ 
	double last_x;						/*!< coordonnÃ©es click x precedent. */ 
	double last_y;						/*!< coordonnÃ©es click y precedent. */ 	
    int win_width;						/*!< Largeur Fenêtre principale. */ 
    int win_height;						/*!< Hauteur Fenêtre principale. */ 
    int show_edit;						/*!< Show Edit 1 pour oui et 0 pour non. */ 
    int edit_mode;						/*!< Edit mode. */ 
	int click_n;						/*!< Nombre de click. */ 
    int bsp_mode;						/*!< Bsp mode. */ 
    int count;							/*!< Compteur de passage dans timeout. */ 
    unsigned int magic;					/*!< Nombre magique. */ 
    char * current_folder;				/*!< Dossier courant. */ 
    char *title;						/*!< Titre de la fenêtre principale. */ 
    Curve_infos curve_infos;			/*!< Curve infos contenant les infos des courbes créé dans l'edition. */ 	
    Game game;							/*!< Objet Game. */
} Mydata;

/**
 * \fn Mydata *get_mydata (gpointer data)
 * \brief Fonction de récupération de l'objet Mydata à partir d'un gpointeur
 *
 * \param self Un Mydata passé sous forme de gpointer
 * \return Mydata * si aucune erreur, NULL sinon.
 */
Mydata *get_mydata (gpointer data);

/**
 * \fn void init_mydata(Mydata *my)
 * \brief Fonction d'initialisation de l'objet Mydata
 *
 * \param self Un objet Mydata
 * \return void
 */
void init_mydata(Mydata *my);

/**
 * \fn void set_edit_mode (Mydata *data, int mode)
 * \brief Fonction de Gestion de l'édition : Curves / Controls
 *
 * \param self Un objet Mydata ainsi que le mode d'edition, un entier.
 * \return void
 */
void set_edit_mode (Mydata *data, int mode);

#endif /* MYDATA_H */
