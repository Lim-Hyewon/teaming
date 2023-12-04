#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <locale.h>
#include <ctype.h>
#include <gtk/gtk.h>

#define MAX_SCHEDULES 10

extern int scheduleCount;

extern GtkWidget *entryDate;
extern GtkWidget *entryEvent;
extern GtkWidget *entryTime;
extern GtkWidget *textView;

typedef struct Schedule
{
    char date[20];
    char event[50];
    char time[10];
} schedule_t;

extern schedule_t schedules[MAX_SCHEDULES];

int isValidDate(const char *date);
int compareSchedules(const void *a, const void *b);
void addScheduleCallback(GtkWidget *widget, gpointer data);
void read_schedules();