#ifndef UTIL_H
#define UTIL_H

#define SAVE_FILE_DEFAULT "Untitled.track"

void refresh_area (GtkWidget *area);

void set_status (GtkWidget *status, const char *format, ...);

int recupTime();

#endif /* UTIL_H */
