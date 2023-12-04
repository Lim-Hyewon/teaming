#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <locale.h>
#include <ctype.h>
#include <gtk/gtk.h>

extern char login_name[30];
extern char projects[10][30];

extern int scheduleCount;

extern GtkWidget *entryDate;
extern GtkWidget *entryEvent;
extern GtkWidget *entryTime;
extern GtkWidget *textView;


void login_initGTK(int argc, char *argv[]);
void schedule_GUI(GtkWidget *widget, gpointer data);

void on_window_destroy(GtkWidget *widget, gpointer data);
void end_button_clicked(GtkWidget *widget, gpointer data);
void score_entry_changed(GtkEditable *editable, gpointer user_data);
void coment_entry_changed(GtkEditable *editable, gpointer user_data);

// 회원가입, 로그인, 팀 생성 및 프로그램 종료 버튼에 대한 선언
void register_button_clicked(GtkWidget *widget, gpointer data);
void login_button_clicked(GtkWidget *widget, gpointer data);
void create_team_button_clicked(GtkWidget *widget, gpointer data);
void exit_program_button_clicked(GtkWidget *widget, gpointer data);

void teams_button_clicked(GtkWidget *widget, gpointer data);

void updateUIAfterLogin(GtkWidget *window);

void end_project(GtkWidget *widget, gpointer data);

void member_score(char *(*member), char *file_name);


void score_GUI(int *scores, char *(*member));
void evaluate_GUI(char *member, gchar *coment);

void viewSchedules(GtkWidget *widget, gpointer data);