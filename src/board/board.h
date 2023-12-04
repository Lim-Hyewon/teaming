#include <gtk/gtk.h>
#include <glib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_POSTS 100
#define MAX_TITLE 100
#define MAX_CONTENT 500

typedef struct {
    char title[MAX_TITLE];
    char content[MAX_CONTENT];
    char author[MAX_TITLE];
} post_t;

void bulletinBoard(GtkWidget *widget, gpointer data);

void writePost(GtkWidget *widget, gpointer data);
void readPosts(GtkWidget *widget, gpointer data);
void searchPosts(GtkWidget *widget, gpointer data);
char *strstr_case_insensitive_word(const char *haystack, const char *needle);
void editPost(GtkWidget *widget, gpointer data);
void deletePost(GtkWidget *widget, gpointer data);
void saveEdit(GtkWidget *widget, gpointer data);
void saveData();
void loadData();

