#include <gtk/gtk.h>
#include "authorization/authorization.h"
#include "board/board.h"
#include "team/team.h"
#include "gui/gui.h"
#include "schedule/schedule.h"

user_t currentUser;
int scheduleCount = 0;
int loggedIn = 0;
char login_name[30];
char projects[10][30];

GtkWidget *entryDate;
GtkWidget *entryEvent;
GtkWidget *entryTime;
GtkWidget *textView;


schedule_t schedules[MAX_SCHEDULES];

// extern post_t posts[MAX_POSTS];
// extern int postCount;

// void createTeam();
// void exitProgram();
// void updateUIAfterLogin(GtkWidget *window);



// #define MAX_SCHEDULES 10
// 이 구조체에 모든 스케줄 저장.


// int isValidDate(const char *date);
// int compareSchedules(const void *a, const void *b);
// void read_schedules();
// // static void viewSchedules(GtkWidget *widget, gpointer data);
// void schedule_GUI();



// 메인 함수
int main(int argc, char *argv[])
{
    login_initGTK(argc, argv);
    return 0;
}
