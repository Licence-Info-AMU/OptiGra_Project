/**
 * \file curve.c
 * \brief Fonctions utiles de gestion des curves
 * \author Gaëtan Perrot
 * \version 0.1
 * \date 23 avril 2017
 *
 * Fonctions utiles de gestion des curves
 *
 */
 
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util.h"
#include "curve.h"

/**
 * \fn void init_curve_infos(Curve_infos *ci)
 * \brief Fonction d'initialisation d'une Curve_infos
 *
 * \param self Objet Curve_infos
 * \return void
 */
void init_curve_infos(Curve_infos *ci){
	ci->curve_list.curve_count = 0;
	ci->current_control = -1;
	ci->current_curve = -1;
}

/**
 * \fn int add_curve (Curve_infos *ci)
 * \brief Fonction d'ajout d'une courbe
 *
 * \param self Objet Curve_infos
 * \return int nombre de courbe total
 */
int add_curve (Curve_infos *ci) {
    if (ci->curve_list.curve_count == CURVE_MAX) {
        ci->current_curve = -1;
        return -1;
    }
    int n = ci->curve_list.curve_count;
    ci->curve_list.curve_count++;
    ci->curve_list.curves[n].control_count = 0;
    ci->current_curve = n;
    ci->current_control = -1;
    ci->curve_list.curves[n].shift_x = 0.0;
	ci->curve_list.curves[n].shift_y = 0.0;
    return n;
}

/**
 * \fn int add_control (Curve_infos *ci, double x, double y)
 * \brief Fonction d'ajout d'un point de control
 *
 * \param self Objet Curve_infos, double coordonnée x , double coordonnée y
 * \return int nombre de point de control total
 */
int add_control (Curve_infos *ci, double x, double y) {
    int n = ci->current_curve;    
    Curve *curve = &ci->curve_list.curves[n];
    if (n < 0 || n > ci->curve_list.curve_count) return -1;
    if (curve->control_count >= CONTROL_MAX) return -1;
    int k = curve->control_count;
    curve->control_count++;
    curve->controls[k].x = x;
    curve->controls[k].y = y;
    ci->current_control = k;
    return k;
}

int find_control (Curve_infos * ci, double x, double y) {    
    Curve *curve;
    for (int i = 0; i <= ci->curve_list.curve_count; ++i)    {
        curve = &ci->curve_list.curves[i];
        for (int j = 0; j < curve->control_count; ++j) {
            double dx = curve->controls[j].x - x;
            double dy = curve->controls[j].y - y;
            if (dx * dx + dy * dy <= 5 * 5) {
                ci->current_curve = i;
                ci->current_control = j;
                printf("Curve : %d\n", i);
                return 0;
            }
        }
    }
    ci->current_curve = -1;
    ci->current_control = -1;
    return -1;    
}

int move_control (Curve_infos * ci, double dx, double dy) {
    int n = ci->current_curve;
    int k = ci->current_control;
    if ((n < 0 || n > ci->curve_list.curve_count - 1) ||
       (k < 0 || k > ci->curve_list.curves[n].control_count - 1))
        return -1;
    Curve *curve = &ci->curve_list.curves[n];
    curve->controls[k].x = curve->controls[k].x + dx;
    curve->controls[k].y = curve->controls[k].y + dy;
    return 0;
}

int move_curve (Curve_infos * ci, double dx, double dy) {
    int n = ci->current_curve;
    if (n < 0 || n > ci->curve_list.curve_count - 1)
        return -1;
    Curve *curve = &ci->curve_list.curves[n];
    for (int i = 0; i < curve->control_count; ++i) {    
        curve->controls[i].x = curve->controls[i].x + dx;
        curve->controls[i].y = curve->controls[i].y + dy;            
    }
    return 0;
}

int remove_curve (Curve_infos * ci) {
    int n = ci->current_curve;
    printf("current_curve : %d, curve_count : %d\n", n, ci->curve_list.curve_count);
    if (n < 0 || n > ci->curve_list.curve_count - 1) {
        return -1;
    }
    memmove (ci->curve_list.curves+n, ci->curve_list.curves+n+1, 
             sizeof(Curve)*(ci->curve_list.curve_count-1-n));
    ci->curve_list.curve_count--;
    ci->current_curve = -1;
    return 0;
}

int remove_control (Curve_infos *ci) {
    int n = ci->current_curve;
    int k = ci->current_control;
    Curve *curve = &ci->curve_list.curves[n];
    if (k < 0 || k > curve->control_count - 1) {
        printf("Suppression échouée\n");
        return -1;
    }    
    memmove (curve->controls+k, curve->controls+k+1, 
             sizeof(Control)*(curve->control_count-1-k));
    curve->control_count--;
    if (curve->control_count < 1) {
        remove_curve(ci);
    }
    ci->current_control = -1;
    printf("Suppression réussie\n");
    return 0;
}

void convert_bsp3_to_bezier(double p[4],double b[4]){
	b[0] = (p[0] + 4*p[1] + p[2])/6.0;
	b[1] = (4*p[1] + 2*p[2])/6.0;
	b[2] = (2*p[1] + 4*p[2])/6.0;
	b[3] = (p[1] + 4*p[2] + p[3])/6.0;
}

void compute_bezier_points_open(Curve * curve, int i,Control bez_points[4]){
	double px[4],py[4],bx[4],by[4];
	for(int j = 0; j < 4;++j){
		px[j] = curve->controls[j+i].x;
		py[j] = curve->controls[j+i].y;
	}
	convert_bsp3_to_bezier(px,bx);
	convert_bsp3_to_bezier(py,by);
	for(int j = 0; j < 4;++j){
		bez_points[j].x = bx[j];
		bez_points[j].y = by[j];
	}
}

void compute_bezier_points_close(Curve * curve, int i,Control bez_points[4]){
	double px[4],py[4],bx[4],by[4];
	for(int j = 0; j < 4;++j){
		px[j] = curve->controls[(j+i)%curve->control_count].x;
		py[j] = curve->controls[(j+i)%curve->control_count].y;
	}
	convert_bsp3_to_bezier(px,bx);
	convert_bsp3_to_bezier(py,by);
	for(int j = 0; j < 4;++j){
		bez_points[j].x = bx[j];
		bez_points[j].y = by[j];
	}
}

double compute_bezier_cubic(double b[4], double t){
	return (pow((1 - t), 3)*b[0] + 3*pow((1 - t), 2)*t*b[1] + 3*(1 - t)*pow(t, 2)*b[2] + pow(t, 3)*b[3]);
}

void convert_bsp3_to_bezier_prolong_first (double p[3],double b[4]){
    b[0] = p[0];
    b[1] = (2*p[0] + p[1])/3.0;
    b[2] = (p[0] + 2*p[1])/3.0;
    b[3] = (p[0] + 4*p[1] + p[2])/6.0;
}

void convert_bsp3_to_bezier_prolong_last (double p[3],double b[4]){
    b[0] = (p[0] + 4*p[1] + p[2])/6.0;
    b[1] = (2*p[1] + p[2])/3.0;
    b[2] = (p[1] + 2*p[2])/3.0;
    b[3] = p[2];
}

void compute_bezier_points_prolong_first (Curve *curve, Control bez_points[4]){
    double px[3],py[3],bx[4],by[4];  
    for (int i = 0; i < 3; i++){
        px[i] = curve->controls [i].x;
        py[i] = curve->controls [i].y;
    }
    convert_bsp3_to_bezier_prolong_first (px, bx);
    convert_bsp3_to_bezier_prolong_first (py, by);
    for (int j = 0; j < 4; j++){
        bez_points[j].x = bx[j];
        bez_points[j].y = by[j];
    }
}
void compute_bezier_points_prolong_last (Curve *curve, Control bez_points[4]){
    double px[3],py[3],bx[4],by[4];
    for (int i = 0; i < 3; i++){
        px[i] = curve->controls [curve->control_count-3+i].x;
        py[i] = curve->controls [curve->control_count-3+i].y;
    }
    convert_bsp3_to_bezier_prolong_last (px, bx);
    convert_bsp3_to_bezier_prolong_last (py, by);
    for (int j = 0; j < 4; j++){
        bez_points[j].x = bx[j];
        bez_points[j].y = by[j];
    }
}

int move_shift(Curve_infos *ci, double dx, double dy){
	int n = ci->current_curve;    
    if(n < 0 || n > ci->curve_list.curve_count - 1)
		return -1;
	ci->curve_list.curves[n].shift_x += dx;
	ci->curve_list.curves[n].shift_y += dy;
	return 0;
}

int reset_shift(Curve_infos *ci){
	int n = ci->current_curve;    
    if(n < 0 || n > ci->curve_list.curve_count - 1)
		return -1;
	ci->curve_list.curves[n].shift_x = 0.0;
	ci->curve_list.curves[n].shift_y = 0.0;
	return 0;
}

void store_sample (double x, double y,double sx[], double sy[], int *ind, int ind_max){
  if (*ind >= ind_max) {
    fprintf (stderr, "%s: capacity exceeded \n", __func__);
    return;
  }
  sx[*ind] = x;
  sy[*ind] = y;
  *ind += 1;
}

void sample_bezier_curve (Control bez_points[4], double theta, double sx[], double sy[], int *ind, int ind_max, int is_first){
  double x, y, bx[4], by[4], t;

  for (int j = 0; j <= 3 ; j++) {
      bx[j] = bez_points[j].x;
      by[j] = bez_points[j].y;
  }

  for (t = is_first ? 0.0 : theta; t < 1.0; t += theta) {
    x = compute_bezier_cubic (bx, t);
    y = compute_bezier_cubic (by, t);
    store_sample (x, y, sx, sy, ind, ind_max);
  }

  if (t < 1.0) {
    x = compute_bezier_cubic (bx, 1.0);
    y = compute_bezier_cubic (by, 1.0);
    store_sample (x, y, sx, sy, ind, ind_max);
  }
}

int interpolate_samples (double sx[], double sy[], double t, int tmax,double *x, double *y){
  if (t <= 0) {
    *x = sx[0];
    *y = sy[0];
    return t == 0;
  } 
  if (t >= tmax-1) {
    *x = sx[tmax-1];
    *y = sy[tmax-1];
    return t == tmax-1;
  } 
  int i = t;
  double u = t-i;
  *x = sx[i]*(1-u) + sx[i+1]*u;
  *y = sy[i]*(1-u) + sy[i+1]*u;
  return 1;
}

/**
 * \fn double compute_distant_point_forward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB)
 * \brief Fonction compute_distant_point_forward
 *
 * \param self double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB
 * \return double
 */
double compute_distant_point_forward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB){
  double xA, yA;
  interpolate_samples (sx, sy, tA, tmax, &xA, &yA);

  if (dist == 0) {
    *xB = xA; *yB = yA;
    return (tA >= 0 && tA <= tmax-1) ? tA : -1;
  }

  double t, m0, m1, dx, dy, d2 = 0, dist2 = dist*dist, epsilon = 0.1;

  // Recherche un point qui dÃ©passe |dist|.
  // On fait une itÃ©ration de plus (t <= tmax-1 +1) pour atteindre tmax-1
  // en effet si t > tmax-1 alors interpolate_samples prend t = tmax-1
  for (t = tA+1.0; t <= tmax-1 +1; t += 1.0) { 
    interpolate_samples (sx, sy, t, tmax, xB, yB);
    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
    if (d2 > dist2) break;
  }
  if (d2 < dist2) return -1;
  m0 = t-1.0; m1 = t;

  // Recherche dichotomique jusqu'Ã  la prÃ©cision epsilon
  while (m1-m0 > 0) {
    t = (m0+m1)/2.0;
    interpolate_samples (sx, sy, t, tmax, xB, yB);
    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
    if (d2 < dist2-epsilon) m0 = t;
    else if (d2 > dist2+epsilon) m1 = t; 
    else break; // PrÃ©cision suffisante
  }
  return t;
}

/**
 * \fn double compute_distant_point_backward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB)
 * \brief Fonction compute_distant_point_backward
 *
 * \param self double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB
 * \return double
 */
double compute_distant_point_backward (double sx[], double sy[], double tA, int tmax,double dist, double *xB, double *yB){
  double xA, yA;
  interpolate_samples (sx, sy, tA, tmax, &xA, &yA);

  if (dist == 0) {
    *xB = xA; *yB = yA;
    return (tA >= 0 && tA <= tmax-1) ? tA : -1;
  }

  double t, m0, m1, dx, dy, d2 = 0, dist2 = dist*dist, epsilon = 0.1;

  // Recherche un point qui dÃ©passe |dist|.
  // On fait une itÃ©ration de plus (t >= 0 -1) pour atteindre 0
  // en effet si t < 0 alors interpolate_samples prend t = 0
  for (t = tA-1.0; t >= 0 -1; t -= 1.0) {
    interpolate_samples (sx, sy, t, tmax, xB, yB);
    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
    if (d2 > dist2) break;
  }
  if (d2 < dist2) return -1;
  m0 = t; m1 = t+1.0;

  // Recherche dichotomique jusqu'Ã  la prÃ©cision epsilon
  while (m1-m0 > 0) {
    t = (m0+m1)/2.0;
    interpolate_samples (sx, sy, t, tmax, xB, yB);
    dx = *xB - xA; dy = *yB - yA; d2 = dx*dx + dy*dy;
    if (d2 < dist2-epsilon) m1 = t;
    else if (d2 > dist2+epsilon) m0 = t; 
    else break; // PrÃ©cision suffisante
  }
  return t;
}

/**
 * \fn int save_curve_to_file (Curve_infos *ci,char *filename)
 * \brief Fonction de sauvegarde d'une curve infos dans un fichier .track
 *
 * \param self Objet Curve_infos, char *filename le nom du fichier
 * \return int retourne 1 si tout c'est bien passé et -1 si la sauvegarde a échoué
 */
int save_curve_to_file (Curve_infos *ci,char *filename){
	if(ci != NULL){
		FILE* file = NULL;
		file = fopen (filename, "w");
		if (file != NULL){
			fprintf(file,"%d\n",ci->curve_list.curve_count); //Nombre de courbe
			for(int i =0; i < ci->curve_list.curve_count;++i){
				fprintf(file,"%d\n%d\n",i,ci->curve_list.curves[i].control_count); // la courbe courrante ainsi que le nombre de point de controle
				for(int j = 0; j < ci->curve_list.curves[i].control_count;++j){
					fprintf(file,"%f %f\n",ci->curve_list.curves[i].controls[j].x,ci->curve_list.curves[i].controls[j].y); // points de controles
				}
			}
			fclose(file);
			return 1;
		}else{
			perror(filename);
			return -1;
		}
	}
	return -1;
}

/**
 * \fn int load_curve_from_file(Curve_infos *ci,char *filename)
 * \brief Fonction de chargement d'une curve infos depuis un fichier .track
 *
 * \param self Objet Curve_infos, char *filename le nom du fichier
 * \return int int retourne 1 si tout c'est bien passé et -1 si le chargement a échoué
 */
int load_curve_from_file(Curve_infos *ci,char *filename){
	FILE* file = NULL;
	file = fopen (filename, "r");
	if (file != NULL){
		if(fscanf(file,"%d",&ci->curve_list.curve_count) != 1){ //Nombre de courbe
			fprintf (stderr, "Fichier mal formé 1\n");
			fclose(file);
			return -1;
		}
		for(int i =0; i < ci->curve_list.curve_count;++i){
			if(fscanf(file,"%d\n%d\n",&ci->current_curve,&ci->curve_list.curves[i].control_count) != 2){ // la courbe courrante ainsi que le nombre de point de controle
				fprintf (stderr, "Fichier mal formé 2\n");
				fclose(file);
				return -1;	
			}
			for(int j = 0; j < ci->curve_list.curves[i].control_count;++j){
				if(fscanf(file,"%lf %lf\n",&ci->curve_list.curves[i].controls[j].x,&ci->curve_list.curves[i].controls[j].y) != 2){ // points de controles
					fprintf (stderr, "Fichier mal formé 3\n");
					fclose(file);
					return -1;
				}
			}
		}
		fclose(file);
		return 1;
	}else{
		printf("B %s\n",filename);
		perror(filename);
		return -1;	
	}
}
