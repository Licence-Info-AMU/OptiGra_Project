/**
 * \file curve.h
 * \brief Fonctions utiles de gestion des curves
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Fonctions utiles de gestion des curves
 *
 */
 
#ifndef CURVE_H
#define CURVE_H

#define CONTROL_MAX 100
#define CURVE_MAX 200

/**
 * \struct Control
 * \brief Objet de point de control
 *
 * Control Objet de point de control possède deux membres coordonnée x et y
 */
typedef struct{
	double x;	/*!< Coordonnée x. */
	double y;	/*!< Coordonnée y. */
} Control;

/**
 * \struct Curve
 * \brief Objet contenant une courbe
 *
 * Curve est objet contenant une courbe
 */
typedef struct{
	int control_count;					/*!< Nombre des points de controls. */
	Control controls[CONTROL_MAX];		/*!< Tableau de point de control. */
	double shift_x;						/*!< Shift y. */
	double shift_y;						/*!< Shift y. */	
} Curve;

/**
 * \struct Curve_list
 * \brief Objet contenant un tableau de curve ainsi que le nombre de ces curves.
 *
 * Curve_list est objet contenant un tableau de curve ainsi que le nombre de ces curves.
 */
typedef struct{
	int curve_count;			/*!< Nombre de courbe. */
	Curve curves[CURVE_MAX];	/*!< Tableau des courbes. */
} Curve_list;

/**
 * \struct Curve_infos
 * \brief Objet contenant une curve_list ainsi que la courbe courante et le point de control courant.
 *
 * Curve_infos est un objet contenant une curve_list ainsi que la courbe courante et le point de control courant.
 */
typedef struct{
	Curve_list curve_list;		/*!< Objet curve_list. */
	int current_curve;			/*!< Courbe courante. */
	int current_control;		/*!< Point de control courant. */	
} Curve_infos;

/**
 * \fn void init_curve_infos(Curve_infos *ci)
 * \brief Fonction d'initialisation d'une Curve_infos
 *
 * \param self Objet Curve_infos
 * \return void
 */
void init_curve_infos(Curve_infos *ci);

/**
 * \fn int add_curve (Curve_infos *ci)
 * \brief Fonction d'ajout d'une courbe
 *
 * \param self Objet Curve_infos
 * \return int nombre de courbe total
 */
int add_curve (Curve_infos *ci);

/**
 * \fn int add_control (Curve_infos *ci, double x, double y)
 * \brief Fonction d'ajout d'un point de control
 *
 * \param self Objet Curve_infos, double coordonnée x , double coordonnée y
 * \return int nombre de point de control total
 */
int add_control (Curve_infos *ci, double x, double y);

int find_control (Curve_infos * ci, double x, double y);

int move_control (Curve_infos * ci, double dx, double dy);

int move_curve (Curve_infos * ci, double dx, double dy);

int remove_curve (Curve_infos * ci);

int remove_control (Curve_infos *ci);

void convert_bsp3_to_bezier(double p[4],double b[4]);

void compute_bezier_points_open(Curve * curve, int i,Control bez_points[4]);

void compute_bezier_points_close(Curve * curve, int i,Control bez_points[4]);

double compute_bezier_cubic(double b[4], double t);

void convert_bsp3_to_bezier_prolong_first (double p[3],double b[4]);

void convert_bsp3_to_bezier_prolong_last (double p[3],double b[4]);

void compute_bezier_points_prolong_first (Curve *curve, Control bez_points[4]);

void compute_bezier_points_prolong_last (Curve *curve, Control bez_points[4]);

int move_shift(Curve_infos *ci, double dx, double dy);

int reset_shift(Curve_infos *ci);

void store_sample (double x, double y,double sx[], double sy[], int *ind, int ind_max);

void sample_bezier_curve (Control bez_points[4], double theta, double sx[], double sy[], int *ind, int ind_max, int is_first);

int interpolate_samples (double sx[], double sy[], double t, int tmax,double *x, double *y);

/**
 * \fn double compute_distant_point_forward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB)
 * \brief Fonction compute_distant_point_forward
 *
 * \param self double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB
 * \return double
 */
double compute_distant_point_forward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB);

/**
 * \fn double compute_distant_point_backward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB)
 * \brief Fonction compute_distant_point_backward
 *
 * \param self double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB
 * \return double
 */
double compute_distant_point_backward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB);

/**
 * \fn int save_curve_to_file (Curve_infos *ci,char *filename)
 * \brief Fonction de sauvegarde d'une curve infos dans un fichier .track
 *
 * \param self Objet Curve_infos, char *filename le nom du fichier
 * \return int retourne 1 si tout c'est bien passé et -1 si la sauvegarde a échoué
 */
int save_curve_to_file (Curve_infos *ci,char *filename);

/**
 * \fn int load_curve_from_file(Curve_infos *ci,char *filename)
 * \brief Fonction de chargement d'une curve infos depuis un fichier .track
 *
 * \param self Objet Curve_infos, char *filename le nom du fichier
 * \return int int retourne 1 si tout c'est bien passé et -1 si le chargement a échoué
 */
int load_curve_from_file(Curve_infos *ci,char *filename);

#endif /* CURVE_H */
